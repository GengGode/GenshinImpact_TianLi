#include "TianLiQtCommon_MapRect.h"

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent> 

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
		LogTraceFunction;
		
		//Core.GetResource().GiMap();
		cv::cvtColor(Core.GetResource().GiMap(), mapMat, cv::COLOR_RGBA2RGB);
	
	}
	//创建刷新定时器
	mapMessageLoopTimer = new QTimer(this);
	mapMessageLoopTimer->start(Fps);//1000/30=33.3,1000/24=42
	connect(mapMessageLoopTimer, SIGNAL(timeout()), this, SLOT(slot_update()));
}

TianLiQtCommon_MapRect::~TianLiQtCommon_MapRect()
{
	//delete mapMat;
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
			Core.GetTrack().StopServer();

		}
		else
		{
			LogInfo("double click down");
			is_double_click_old = true;
			Core.GetTrack().StartServer();
			
		}
		//leftBtnClk = false;
		//mapScale += deltaMapScale;
	}
}

void TianLiQtCommon_MapRect::mouseMoveEvent(QMouseEvent* event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		cv::Point map_move = cv::Point( (m_Move.x() - m_Press.x()) * mapScale,
			(m_Move.y() - m_Press.y()) * mapScale);
		mapPos= mapPos-map_move;
		if (map_move.x != 0 || map_move.y != 0)
		{
			m_Press = m_Move;
			update();
			//LogInfo(QString::number(mapPos.x) + "," + QString::number(mapPos.y));
		}
	}

}

void TianLiQtCommon_MapRect::wheelEvent(QWheelEvent* event)
{
	//TianLi_Logger.Info(__FUNCTION__, QString::number(event->delta()));
	//LogInfo(QString::number(event->delta() / 120));
	//LogInfo(QString::number(mapScale));
	if (event->delta() < 0) {
		if (mapScale > 10)return;
		mapScale += deltaMapScale;
	}
	else if(event->delta() > 0) {
		if (mapScale <= 0.3)return;
		mapScale -= deltaMapScale;
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

	cv::Mat mapMatRect;// = mapMat(mapRect);

	mapMatRect = TianLi::Utils::get_view_map(mapMat, cv::Size(ui.label_Render->width(), ui.label_Render->height()), mapPos, mapScale);
	
	std::vector<cv::Mat> mv;
	cv::split(mapMatRect, mv);
	mv.push_back(mapMaskMat);
	cv::merge(mv,mapMatRect);

	mapImage =  QImage((uchar*)(mapMatRect.data), mapMatRect.cols, mapMatRect.rows, mapMatRect.cols * (mapMatRect.channels()), QImage::Format_ARGB32);

	//设置画面为地图
	QPainter painter(this);
	painter.drawImage(0, 0,mapImage);
}

void TianLiQtCommon_MapRect::resizeEvent(QResizeEvent* event)
{
	const int w = event->size().width();
	const int h = event->size().height();
	//ui.label_Mask->setGeometry(0, 0, w, h);
	ui.label_Render->setGeometry(0, 0, w, h);
	QImage Mask = TianLi::Utils::border_image(QImage(":/TianLiQtCommon_MapRect/resource/TianLiQtCommon_MapRect/TianLiQtCommon_MapRect_MapMask.png"), w, h, 30, 30, 30, 30);
	mapMaskMat = cv::Mat(Mask.height(), Mask.width(), CV_8UC4, Mask.bits(), Mask.bytesPerLine());

	std::vector<cv::Mat> mv;
	cv::split(mapMaskMat, mv);
	mapMaskMat = mv[3];
}

void TianLiQtCommon_MapRect::slot_update()
{
	if (Core.GetTrack().GetResult().is_paimon_visial)
	{
		mapPos = cv::Point(Core.GetTrack().GetResult().x, Core.GetTrack().GetResult().y);
		update();
	}
}
