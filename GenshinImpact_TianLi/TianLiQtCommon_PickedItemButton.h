#pragma once

#include <QPushButton>
#include "ui_TianLiQtCommon_PickedItemButton.h"

class TianLiQtCommon_PickedItemButton : public QPushButton
{
	Q_OBJECT

public:
	TianLiQtCommon_PickedItemButton(QString title = "", QImage image = QImage(), QWidget* parent = Q_NULLPTR); // : TianLiQtCommon_SelectedItemButton("") {}
	//TianLiQtCommon_PickedItemButton(QString title, QWidget* parent = Q_NULLPTR) :TianLiQtCommon_SelectedItemButton(title, QImage()) {}
	//TianLiQtCommon_PickedItemButton(QString title, QImage image, QWidget* parent = Q_NULLPTR);

	~TianLiQtCommon_PickedItemButton();

private:
	Ui::TianLiQtCommon_PickedItemButton ui;

public:
	void setTitle(QString title);
	void setImage(QImage image);
};
