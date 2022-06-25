#pragma once

#include <opencv2/opencv.hpp>

#include <QWidget>
#include "ui_TianLiQtCommon_MapRect.h"



class TianLiQtCommon_MapRect : public QWidget
{
	Q_OBJECT

public:
	TianLiQtCommon_MapRect(QWidget *parent = Q_NULLPTR);
	~TianLiQtCommon_MapRect();

private:
	Ui::TianLiQtCommon_MapRect ui;

private:
	int radius = 32;

//  Û±ÍÕœ∂Ø
private:
	QPoint m_Press;
	QPoint m_Move;
	bool leftBtnClk = false;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private:
	cv::Point mapPos=cv::Point(1234,1234);
	int Fps = 42;//ms
	QTimer* mapMessageLoopTimer;
	QImage mapImage;
	cv::Mat mapMat;
	cv::Mat mapMaskMat;
	
// UI÷ÿªÊ
private:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
};
