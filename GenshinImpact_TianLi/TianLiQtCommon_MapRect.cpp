#include "TianLiQtCommon_MapRect.h"

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent> 

#include <opencv2/opencv.hpp>

#include "TianLiQtCommon_Logger.h"

#include "..\GenshinImpact_TianLi_Resource\GenshinImpact_TianLi_Resource.h"
#pragma comment(lib,"GenshinImpact_TianLi_Resource.lib")

/// <summary>
/// 
/// </summary>
/// <param name="GIMAP">原地图</param>
/// <param name="viewMapSize">输出图片大小</param>
/// <param name="viewMapCenter">输入视口中心坐标</param>
/// <param name="viewMapScale">输入地图缩放</param>
/// <returns></returns>
cv::Mat GetViewMap(cv::Mat& GIMAP,cv::Size viewMapSize,cv::Point2d viewMapCenter,double viewMapScale)
{
	static cv::Mat viewMap;
	static cv::Rect viewMapRect;
	//static cv::Point2d viewMapCenter;
	
	static int mapSizeWidth = GIMAP.size().width;
	static int mapSizeHeight = GIMAP.size().height;
	const static cv::Point2d originGIMAP(1428, 2937);
	//需要能越过边界，否则大范围显示时无法保证角色箭头处于正确位置

	cv::Point minMapPoint = cv::Point(0, 0);

	cv::Size reMapSize = viewMapSize;
	cv::Point2d reAutoMapCenter = viewMapCenter;
	reMapSize.width = (reMapSize.width * viewMapScale);
	reMapSize.height = (reMapSize.height * viewMapScale);
	reAutoMapCenter = viewMapCenter * viewMapScale;

	cv::Point2d LT = originGIMAP - reAutoMapCenter;
	cv::Point2d RB = originGIMAP + cv::Point2d(reMapSize) - reAutoMapCenter;

	minMapPoint = LT;

	if (LT.x < 0)
	{
		minMapPoint.x = 0;
	}
	if (LT.y < 0)
	{
		minMapPoint.y = 0;
	}
	if (RB.x > mapSizeWidth)
	{
		minMapPoint.x = mapSizeWidth - reMapSize.width;
	}
	if (RB.y > mapSizeHeight)
	{
		minMapPoint.y = mapSizeHeight - reMapSize.height;
	}
	viewMapRect = cv::Rect(minMapPoint, reMapSize);

	resize(GIMAP(viewMapRect), viewMap, viewMapSize);
	return viewMap;
}

TianLiQtCommon_MapRect::TianLiQtCommon_MapRect(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.label_Render->setVisible(false);
	{
		LogTraceFunction;

		//QString mapStr = "C:\\Users\\GengG\\source\\repos\\GenshinImpact_TianLi\\GenshinImpact_TianLi\\resource\\TianLiQtCommon_MapRect\\maplite.png";
		//mapMat = cv::imread(mapStr.toStdString());
		HBITMAP _hBmp = TianLi::LoadPNG_GIMAP();

		BITMAP bmp;
		GetObject(_hBmp, sizeof(BITMAP), &bmp);
		int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
		int depth = bmp.bmBitsPixel == 1 ? 1 : 8;
		cv::Mat v_mat;
		v_mat.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8UC3, nChannels));
		GetBitmapBits(_hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, v_mat.data);
		if (nChannels == 4)
		{
			cv::cvtColor(v_mat, v_mat, cv::COLOR_RGBA2RGB);
		}		
		mapMat = v_mat;
	}
	//创建刷新定时器
	mapMessageLoopTimer = new QTimer(this);
	mapMessageLoopTimer->start(Fps);//1000/30=33.3,1000/24=42
	connect(mapMessageLoopTimer, SIGNAL(timeout()), this, SLOT(update()));
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
	if (event->button() == Qt::LeftButton) 
	{
		//leftBtnClk = false;
	}
}

void TianLiQtCommon_MapRect::mouseMoveEvent(QMouseEvent* event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		mapPos=cv::Point(mapPos.x+( m_Move.x() - m_Press.x())*mapScale,
			mapPos.y+(m_Move.y()- m_Press.y())*mapScale);
		m_Press = m_Move;
		
		LogInfo(QString::number(mapPos.x)+","+QString::number(mapPos.y));
	}
	update();
}

void TianLiQtCommon_MapRect::wheelEvent(QWheelEvent* event)
{
	//TianLi_Logger.Info(__FUNCTION__, QString::number(event->delta()));
	//LogInfo(QString::number(event->delta() / 120));
	LogInfo( QString::number(mapScale));
	if (event->delta() > 0) {
		if (mapScale > 10)return;
		mapScale += deltaMapScale;
	}
	else {
		if (mapScale <= 0.3)return;
		mapScale -= deltaMapScale;
	}
}

void TianLiQtCommon_MapRect::paintEvent(QPaintEvent* event)
{
	// LogTraceFunction;
	
	//// 计算缩放可视窗口
	//cv::Point mapSize(ui.label_Render->width() * mapScale, ui.label_Render->height() * mapScale);
	//cv::Rect mapRect = cv::Rect(mapPos.x, mapPos.y, mapSize.x, mapSize.y);
	//
	////cv::Rect mapRect = cv::Rect(mapPos.x,mapPos.y, ui.label_Render->width(), ui.label_Render->height());
	//
	cv::Mat mapMatRect;// = mapMat(mapRect);
	////cv::resize(mapMat(mapRect), mapMatRect, cv::Size(ui.label_Render->width(), ui.label_Render->height()));

	//// 生成仿射变换参数矩阵
	//cv::Mat mapMatAffine = cv::Mat::eye(2, 3, CV_32FC1);
	//mapMatAffine.at<float>(0, 0) = mapScale;
	//mapMatAffine.at<float>(1, 1) = mapScale;
	//mapMatAffine.at<float>(0, 2) = ui.label_Render->width() / 2.0 - mapPos.x / mapScale;
	//mapMatAffine.at<float>(1, 2) = ui.label_Render->height() / 2.0 - mapPos.y / mapScale;
	//
	//// 变换图像
	//cv::Mat mapMatAffineResult;
	//cv::warpAffine(mapMat, mapMatAffineResult, mapMatAffine, cv::Size(ui.label_Render->width(), ui.label_Render->height()));
	//
	//// 将变换后的图像显示到界面
	//mapMatRect = mapMatAffineResult;
	mapMatRect = GetViewMap(mapMat, cv::Size(ui.label_Render->width(), ui.label_Render->height()), mapPos, mapScale);
	
	std::vector<cv::Mat> mv;
	cv::split(mapMatRect, mv);
	mv.push_back(mapMaskMat);
	cv::merge(mv,mapMatRect);

	mapImage =  QImage((uchar*)(mapMatRect.data), mapMatRect.cols, mapMatRect.rows, mapMatRect.cols * (mapMatRect.channels()), QImage::Format_ARGB32);

	//设置画面为地图
	QPainter painter(this);
	painter.drawImage(0, 0,mapImage);
}

// 九宫格扩展图片算法
QImage border_image(QImage image,int w,int h,int clip_top,int clip_right,int clip_bottom,int clip_left)
{
	QImage new_image(w, h, QImage::Format_ARGB32);
	new_image.fill(Qt::transparent);
	QRect target;
	QRect source;
	QPainter painter(&new_image);
	
	// left_top
	// target 范围为 起点：0,0 大小：clip_left, clip_top
	target = QRect(0, 0, clip_left, clip_top);
	// source 范围为 起点：0,0 大小：clip_left, clip_top
	source = QRect(0, 0, clip_left, clip_top);
	painter.drawImage(target, image, source);
	
	// left_bottom
	// target 范围为 起点：0,h-clip_bottom 大小：clip_left, clip_bottom
	target = QRect(0, h-clip_bottom, clip_left, clip_bottom);
	// source 范围为 起点：0,image.height()-clip_bottom 大小：clip_left, clip_bottom
	source = QRect(0, image.height()-clip_bottom, clip_left, clip_bottom);
	painter.drawImage(target, image, source);
	
	// right_top
	// target 范围为 起点：w-clip_right,0 大小：clip_right, clip_top
	target = QRect(w-clip_right, 0, clip_right, clip_top);
	// source 范围为 起点：image.width()-clip_right,0 大小：clip_right, clip_top
	source = QRect(image.width()-clip_right, 0, clip_right, clip_top);
	painter.drawImage(target, image, source);
	
	// right_bottom
	// target 范围为 起点：w-clip_right,h-clip_bottom 大小：clip_right, clip_bottom
	target = QRect(w-clip_right, h-clip_bottom, clip_right, clip_bottom);
	// source 范围为 起点：image.width()-clip_right,image.height()-clip_bottom 大小：clip_right, clip_bottom
	source = QRect(image.width()-clip_right, image.height()-clip_bottom, clip_right, clip_bottom);
	painter.drawImage(target, image, source);
	
	// top
	// target 范围为 起点：clip_left,0 大小：w-clip_left-clip_right, clip_top
	target = QRect(clip_left, 0, w-clip_left-clip_right, clip_top);
	// source 范围为 起点：clip_left,0 大小：image.width()-clip_left-clip_right, clip_top
	source = QRect(clip_left, 0, image.width()-clip_left-clip_right, clip_top);
	painter.drawImage(target, image, source);
	
	// bottom
	// target 范围为 起点：clip_left,h-clip_bottom 大小：w-clip_left-clip_right, clip_bottom
	target = QRect(clip_left, h-clip_bottom, w-clip_left-clip_right, clip_bottom);
	// source 范围为 起点：clip_left,image.height()-clip_bottom 大小：image.width()-clip_left-clip_right, clip_bottom
	source = QRect(clip_left, image.height()-clip_bottom, image.width()-clip_left-clip_right, clip_bottom);
	painter.drawImage(target, image, source);
	
	// left
	// target 范围为 起点：0,clip_top 大小：clip_left, h-clip_top-clip_bottom
	target = QRect(0, clip_top, clip_left, h-clip_top-clip_bottom);
	// source 范围为 起点：0,clip_top 大小：clip_left, image.height()-clip_top-clip_bottom
	source = QRect(0, clip_top, clip_left, image.height()-clip_top-clip_bottom);
	painter.drawImage(target, image, source);
	
	// right
	// target 范围为 起点：w-clip_right,clip_top 大小：clip_right, h-clip_top-clip_bottom
	target = QRect(w-clip_right, clip_top, clip_right, h-clip_top-clip_bottom);
	// source 范围为 起点：image.width()-clip_right,clip_top 大小：clip_right, image.height()-clip_top-clip_bottom
	source = QRect(image.width()-clip_right, clip_top, clip_right, image.height()-clip_top-clip_bottom);
	painter.drawImage(target, image, source);

	// center
	// target 范围为 起点：clip_left,clip_top 大小：w-clip_left-clip_right, h-clip_top-clip_bottom
	target = QRect(clip_left, clip_top, w-clip_left-clip_right, h-clip_top-clip_bottom);
	// source 范围为 起点：clip_left,clip_top 大小：image.width()-clip_left-clip_right, image.height()-clip_top-clip_bottom
	source = QRect(clip_left, clip_top, image.width()-clip_left-clip_right, image.height()-clip_top-clip_bottom);
	painter.drawImage(target, image, source);

	painter.end();
	return new_image;
}

void TianLiQtCommon_MapRect::resizeEvent(QResizeEvent* event)
{
	const int w = event->size().width();
	const int h = event->size().height();
	//ui.label_Mask->setGeometry(0, 0, w, h);
	ui.label_Render->setGeometry(0, 0, w, h);
	QImage Mask = border_image(QImage(":/TianLiQtCommon_MapRect/resource/TianLiQtCommon_MapRect/TianLiQtCommon_MapRect_MapMask.png"), w, h, 30, 30, 30, 30);
	mapMaskMat = cv::Mat(Mask.height(), Mask.width(), CV_8UC4, Mask.bits(), Mask.bytesPerLine());

	std::vector<cv::Mat> mv;
	cv::split(mapMaskMat, mv);
	mapMaskMat = mv[3];
}
