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
	int radius = 50;

//  Û±ÍÕœ∂Ø
private:
	QPoint m_Press;
	QPoint m_Move;
	bool leftBtnClk = false;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseDoubleClickEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
	
private:
	cv::Point render_map_pos=cv::Point(1428, 2937);
	double render_map_scale =1.0;
	const double deltaMapScale=0.1;
	int Fps = 42;//ms
	QTimer* mapMessageLoopTimer;
	QImage render_map_image;
	// cv::Mat gi_map;
	cv::Mat render_map_mask;
	bool is_need_rerender = false;

// UI÷ÿªÊ
private:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
	
public slots:
	void slot_update();
	void slot_force_update();

signals:
	void signal_double_click(bool is_checked);
	void singal_updata_pickable_items(std::vector<std::string> item_tags);
	void signle_send_mini_object_info_text(QString text);
};
