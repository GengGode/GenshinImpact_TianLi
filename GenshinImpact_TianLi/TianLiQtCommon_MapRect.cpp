#include "TianLiQtCommon_MapRect.h"

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent> 
#include <QFontDatabase> 

#include <opencv2/opencv.hpp>

#include "TianLiQtCommon_Logger.h"
#include "TianLiQtCommon_Utils.h"

#include "..\GenshinImpact_TianLi_Map\GenshinImpact_TianLi_Map.h"
#pragma comment(lib,"GenshinImpact_TianLi_Map.lib")

TianLiQtCommon_MapRect::TianLiQtCommon_MapRect(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
		
	//创建刷新定时器
	mapMessageLoopTimer = new QTimer(this);
	mapMessageLoopTimer->start(Fps);//1000/30=33.3,1000/24=42
	connect(mapMessageLoopTimer, SIGNAL(timeout()), this, SLOT(slot_update()));
}

TianLiQtCommon_MapRect::~TianLiQtCommon_MapRect()
{
	//delete gi_map;
	delete mapMessageLoopTimer;
}

void TianLiQtCommon_MapRect::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton ) 
	{
		m_Press = event->globalPos();
		leftBtnClk = true;
	}
}

void TianLiQtCommon_MapRect::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) 
	{
		leftBtnClk = false;
	}
}

void TianLiQtCommon_MapRect::mouseDoubleClickEvent(QMouseEvent* event)
{
	static bool is_double_click_old = false;
	if (event->button() == Qt::LeftButton) 
	{
		if (is_double_click_old)
		{
			LogInfo("double click up");
			is_double_click_old = false;
			emit signal_double_click(false);
		}
		else
		{
			LogInfo("double click down");
			is_double_click_old = true;
			emit signal_double_click(true);
		}
	}
}

void TianLiQtCommon_MapRect::mouseMoveEvent(QMouseEvent* event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		cv::Point map_move = cv::Point( (m_Move.x() - m_Press.x()) * render_map_scale,
			(m_Move.y() - m_Press.y()) * render_map_scale);
		render_map_pos= render_map_pos-map_move;
		if (map_move.x != 0 || map_move.y != 0)
		{
			m_Press = m_Move;
			update();
		}
	}

}

void TianLiQtCommon_MapRect::wheelEvent(QWheelEvent* event)
{
	if (event->delta() < 0) {
		if (render_map_scale > 10)return;
		render_map_scale += deltaMapScale;
	}
	else if(event->delta() > 0) {
		if (render_map_scale <= 0.3)return;
		render_map_scale -= deltaMapScale;
	}
	else
	{
		return;
	}
	update();
}

void TianLiQtCommon_MapRect::paintEvent(QPaintEvent* event)
{
	//LogTraceFunction;
	QPainter painter(this);

	static cv::Mat mapMatRect;// = gi_map(mapRect);
	static cv::Point old_map_center_pos;
	static double old_map_scale = 0;
	
	// LogInfo(QString("进入 重绘地图 state: ")+QString::number(is_need_rerender));

	// 如果 old pos 和 old scale 与当前的一样，就不用重新计算了
	if (old_map_center_pos != render_map_pos || old_map_scale != render_map_scale || is_need_rerender)
	{

		is_need_rerender = false;
		old_map_center_pos = render_map_pos;
		old_map_scale = render_map_scale;
		
		CoreMap.map_info.is_show_map = true;

		CoreMap.map_info.center_x = render_map_pos.x;
		CoreMap.map_info.center_y = render_map_pos.y;
		CoreMap.map_info.viewer_width = ui.label_Render->width();
		CoreMap.map_info.viewer_height = ui.label_Render->height();

		//CoreMap.map_info.map_rect = map_rect;
		CoreMap.map_info.scale_form_gimap = render_map_scale;
		// 渲染图例
		CoreMap.render_legend(mapMatRect);
		//***


		std::vector<cv::Mat> mv;
		cv::split(mapMatRect, mv);
		mv[3] = render_map_mask;
		cv::merge(mv, mapMatRect);

		render_map_image = QImage((uchar*)(mapMatRect.data), mapMatRect.cols, mapMatRect.rows, mapMatRect.cols * (mapMatRect.channels()), QImage::Format_ARGB32);

		//设置画面为地图

	}
	painter.drawImage(0, 0, render_map_image);
}

void TianLiQtCommon_MapRect::resizeEvent(QResizeEvent* event)
{
	const int w = event->size().width();
	const int h = event->size().height();
	//ui.label_Mask->setGeometry(0, 0, w, h);
	ui.label_Render->setGeometry(0, 0, w, h);
	ui.label_UID->setGeometry(w - 230,h- 40, 200, 24);
	QImage Mask = TianLi::Utils::border_image(QImage(":/TianLiQtCommon_MapRect/resource/TianLiQtCommon_MapRect/TianLiQtCommon_MapRect_MapMask.png"), w, h, 30, 30, 30, 30);
	render_map_mask = cv::Mat(Mask.height(), Mask.width(), CV_8UC4, Mask.bits(), Mask.bytesPerLine());

	std::vector<cv::Mat> mv;
	cv::split(render_map_mask, mv);
	render_map_mask = mv[3];
	
	is_need_rerender = true;
}

void TianLiQtCommon_MapRect::slot_update()
{
	if (Core.GetTrack().ServerState() && Core.GetTrack().GetResult().is_find_paimon)
	{
		auto pos = cv::Point(CoreMap.Core.GetTrack().GetResult().position_x, CoreMap.Core.GetTrack().GetResult().position_y);
		render_map_pos = pos;
		CoreMap.avatar_info.x = pos.x;
		CoreMap.avatar_info.y = pos.y;
		CoreMap.avatar_info.a = CoreMap.Core.GetTrack().GetResult().avatar_angle;
	


		ui.label_UID->setText(QString("UID: %1").arg(CoreMap.Core.GetTrack().GetResult().uid, 9, 10, QLatin1Char('0')));
		// 获取距离最近的一个物品的详细信息
		// 1. 遍历所有距离，为了降低计算量，先使用曼哈顿距离？好像这么叫的
		// TODO: 需要改为欧式距离，等优化数据源
		auto show_objects = CoreMap.map_show_objects;
		std::string show_info_min_ids = "";
		// 最小距离的起始点应该从边框处距离计算，即对角顶点的距离
		double dis = 80;
		for (auto& objects : show_objects.types)
		{
			auto& x = pos.x;
			auto& y = pos.y;
			for (auto& object : objects.badge_list)
			{
				// 计算公式为 x2-x1 + y2-y1
				double object_dis = std::abs(object.x - x) + std::abs(object.y - y);
				if (object_dis < dis)
				{
					dis = object_dis;
					show_info_min_ids = object.message;
				}

			}
		}
		emit signle_send_mini_object_info_text(QString::fromStdString(show_info_min_ids));


		// 更新可捡取物品列表
		static std::vector<std::string> item_tags_buf;
		auto item_tags = CoreMap.Core.GetTrack().GetResult().item_tags;
		if (item_tags_buf != item_tags)
		{
			item_tags_buf = item_tags;
			emit singal_updata_pickable_items(item_tags);
		}
		
		update();
	}
}

void TianLiQtCommon_MapRect::slot_force_update()
{
	is_need_rerender = true;
	update();
}
