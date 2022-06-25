#include "TianLiQtCommon_MapRect.h"

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent> 

#include <opencv2/opencv.hpp>

TianLiQtCommon_MapRect::TianLiQtCommon_MapRect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//ui.label_Render->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	
	QString mapStr = "C:\\Users\\GengG\\source\\repos\\GenshinImpact_TianLi\\GenshinImpact_TianLi\\resource\\TianLiQtCommon_MapRect\\GIMAP.png";
	
	mapMat = cv::Mat(cv::imread(mapStr.toStdString()));
	

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
	if (event->button() == Qt::LeftButton &&
		ui.label_Render->frameRect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		leftBtnClk = true;
	}
	event->ignore();
}

void TianLiQtCommon_MapRect::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		leftBtnClk = false;
	}
	event->ignore();
}

void TianLiQtCommon_MapRect::mouseMoveEvent(QMouseEvent* event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		mapPos=cv::Point(mapPos.x+ m_Move.x() - m_Press.x(),
			mapPos.y+ m_Move.y() - m_Press.y());
		m_Press = m_Move;
	}
	event->ignore();
}
void TianLiQtCommon_MapRect::paintEvent(QPaintEvent* event)
{
	cv::Rect mapRect = cv::Rect(mapPos.x,mapPos.y, ui.label_Render->width(), ui.label_Render->height());
	cv::Mat mapMatRect = mapMat(mapRect);
	
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
