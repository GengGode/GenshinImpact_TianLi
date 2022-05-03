#pragma once

#include <QPushButton>
#include "ui_TianLiQtCommon_NearbyItemButton.h"

class TianLiQtCommon_NearbyItemButton : public QPushButton
{
	Q_OBJECT

public:
	TianLiQtCommon_NearbyItemButton(QString title = "", QImage image = QImage(), QWidget* parent = Q_NULLPTR);
	~TianLiQtCommon_NearbyItemButton();

private:
	Ui::TianLiQtCommon_NearbyItemButton ui;

public:
	void setTitle(QString title);
	void setImage(QImage image);
};
