#include "TianLiQtCommon_MapRect.h"

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent> 
#include <QFontDatabase> 

#include <opencv2/opencv.hpp>

#include "TianLiQtCommon_Logger.h"
#include "TianLiQtCommon_Utils.h"

#include "..\GenshinImpact_TianLi_Core\GenshinImpact_TianLi_Core.h"
#pragma comment(lib,"GenshinImpact_TianLi_Core.lib")

TianLiQtCommon_MapRect::TianLiQtCommon_MapRect(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
		
	ui.label_Render->setVisible(false);
	{		
		//gi_map = Core.GetResource().GiMap();
	}
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

	// 如果 old pos 和 old scale 与当前的一样，就不用重新计算了
	if (old_map_center_pos != render_map_pos || old_map_scale != render_map_scale || is_need_rerender)
	{
		is_need_rerender = false;
		old_map_center_pos = render_map_pos;
		old_map_scale = render_map_scale;
		
		//---
		cv::Rect viewer_rect;
		mapMatRect = TianLi::Utils::get_view_map(Core.GetResource().GiMap(), cv::Size(ui.label_Render->width(), ui.label_Render->height()), render_map_pos, render_map_scale, viewer_rect);
		
		Core.GetMap().map_info.is_overlay = true;

		Core.GetMap().map_info.viewer_rect = viewer_rect;
		Core.GetMap().map_info.scale_form_gimap = render_map_scale;
		// 渲染图例
		Core.GetMap().render_legend(mapMatRect);
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
	ui.label_UID->setGeometry(w - 220,h- 40, 200, 24);
	QImage Mask = TianLi::Utils::border_image(QImage(":/TianLiQtCommon_MapRect/resource/TianLiQtCommon_MapRect/TianLiQtCommon_MapRect_MapMask.png"), w, h, 30, 30, 30, 30);
	render_map_mask = cv::Mat(Mask.height(), Mask.width(), CV_8UC4, Mask.bits(), Mask.bytesPerLine());

	std::vector<cv::Mat> mv;
	cv::split(render_map_mask, mv);
	render_map_mask = mv[3];
	
	is_need_rerender = true;
}

void TianLiQtCommon_MapRect::slot_update()
{
	if (Core.GetTrack().GetResult().is_find_paimon)
	{
		render_map_pos = cv::Point(Core.GetTrack().GetResult().position_x, Core.GetTrack().GetResult().position_y);
		ui.label_UID->setText(QString("UID: %1").arg(Core.GetTrack().GetResult().uid, 9, 10, QLatin1Char('0')));
		
		update();
	}
}
