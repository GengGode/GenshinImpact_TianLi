#include "TianLiQtCommon_HUD_SquareMap.h"
#include <QTimer>
#include <QPainter>
#include <QCloseEvent>
#include <QWindow>

#include "TianLiQtCommon_Logger.h"
#include "TianLiQtCommon_Utils.h"

#include "..\GenshinImpact_TianLi_Core\GenshinImpact_TianLi_Core.h"
#pragma comment(lib,"GenshinImpact_TianLi_Core.lib")

TianLiQtCommon_HUD_SquareMap::TianLiQtCommon_HUD_SquareMap(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_QuitOnClose, false);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
		WS_EX_TRANSPARENT | WS_EX_LAYERED);
	
	{
		LogTraceFunction;
		cv::cvtColor(Core.GetResource().GiMap(), gi_map, cv::COLOR_RGBA2RGB);
	}
	
	timer = new QTimer;
	timer->setInterval(42);
	static int i = 0;
	connect(timer, &QTimer::timeout, this, &TianLiQtCommon_HUD_SquareMap::slot_update);
	timer->start();
}

TianLiQtCommon_HUD_SquareMap::~TianLiQtCommon_HUD_SquareMap()
{
	delete timer;	
}

void TianLiQtCommon_HUD_SquareMap::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	// 清空
	painter.setCompositionMode(QPainter::CompositionMode_Clear);
	painter.fillRect(this->rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	
	const int w = width();
	const int h = height();
	
	static cv::Mat mapMatRect;
	static cv::Point old_map_center_pos;
	static double old_map_scale = 0;

	// 如果 old pos 和 old scale 与当前的一样，就不用重新计算了
	if (old_map_center_pos != render_map_pos || old_map_scale != render_map_scale || is_need_rerender)
	{
		is_need_rerender = false;
		old_map_center_pos = render_map_pos;
		old_map_scale = render_map_scale;

		mapMatRect = TianLi::Utils::get_view_map(gi_map, cv::Size(width(), height()), render_map_pos, render_map_scale);

		std::vector<cv::Mat> mv;
		cv::split(mapMatRect, mv);
		mv.push_back(render_map_mask);
		cv::merge(mv, mapMatRect);

		render_map_image = QImage((uchar*)(mapMatRect.data), mapMatRect.cols, mapMatRect.rows, mapMatRect.cols * (mapMatRect.channels()), QImage::Format_ARGB32);

	}
	painter.drawImage(0, 0, render_map_image);
}

void TianLiQtCommon_HUD_SquareMap::closeEvent(QCloseEvent* event)
{
	event->accept();
	emit signal_close_finished();
}

void TianLiQtCommon_HUD_SquareMap::resizeEvent(QResizeEvent* event)
{
	const int w = event->size().width();
	const int h = event->size().height();
	render_map_mask = TianLi::Utils::create_square_mask(w, h, 20);
	is_need_rerender = true;
}

void TianLiQtCommon_HUD_SquareMap::slot_update()
{
	if (is_visible)
	{
		if (Core.GetTrack().GetResult().is_find_paimon)
		{
			render_map_pos = cv::Point(Core.GetTrack().GetResult().position_x, Core.GetTrack().GetResult().position_y);
			RECT gi_minimap_rect = Core.GetTrack().GetResult().minimap_rect;
			slot_update_move(gi_minimap_rect);
		}
		else
		{
			update();
		}
	}
}

void TianLiQtCommon_HUD_SquareMap::slot_show()
{
	is_visible = true;
	this->show();
	timer->start();
}

void TianLiQtCommon_HUD_SquareMap::slot_hide()
{
	is_visible = false;
	this->hide();
	timer->stop();	
}

void TianLiQtCommon_HUD_SquareMap::slot_update_move(RECT& gi_minimap_rect)
{
	static RECT gi_paimon_rect_old;
	if (gi_paimon_rect_old.left != gi_minimap_rect.left || gi_paimon_rect_old.top != gi_minimap_rect.top
		|| gi_paimon_rect_old.right != gi_minimap_rect.right || gi_paimon_rect_old.bottom != gi_minimap_rect.bottom)
	{
		gi_paimon_rect_old = gi_minimap_rect;
		
		int move_x = gi_minimap_rect.right;
		int move_y = gi_minimap_rect.top+ (gi_minimap_rect.bottom - gi_minimap_rect.top) * 0.25;
		int resize_w = (gi_minimap_rect.right - gi_minimap_rect.left)*1.5;
		int resize_h = (gi_minimap_rect.bottom - gi_minimap_rect.top)*1.5;
		this->setGeometry(move_x, move_y, resize_w, resize_h);
		
		update();
	}
}

