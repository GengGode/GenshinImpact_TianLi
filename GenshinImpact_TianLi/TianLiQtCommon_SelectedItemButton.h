#pragma once

#include <QPushButton>
#include "ui_TianLiQtCommon_SelectedItemButton.h"

class TianLiQtCommon_SelectedItemButton : public QPushButton
{
	Q_OBJECT

public:
	TianLiQtCommon_SelectedItemButton(QString item_name = "", QString type_name = "", QString area_name = "", QImage item_image = QImage(), QImage type_image = QImage(), QWidget* parent = Q_NULLPTR);

	~TianLiQtCommon_SelectedItemButton();
	
private:
	Ui::TianLiQtCommon_SelectedItemButton ui;
	
private:
	void mouseDoubleClickEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	
	// UI÷ÿªÊ
private:
	//void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	int progress_max_number = 100;
	int progress_count = 5;
private:
	QString item;
	QString type;
	QString area;
public:
	QString item_name();
	QString type_name();
	QString area_name();

	void setTitle(QString title);
	void setRegion(QString region);
	void setImage(QImage image);
	void setLabelImage(QImage image);
	void setProgressCount(int number);
	void setProgressMaxNumber(int number);
signals:
	void signal_double_click(bool is_checked);
};
