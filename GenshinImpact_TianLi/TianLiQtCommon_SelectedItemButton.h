#pragma once

#include <QPushButton>
#include "ui_TianLiQtCommon_SelectedItemButton.h"

class TianLiQtCommon_SelectedItemButton : public QPushButton
{
	Q_OBJECT

public:
	TianLiQtCommon_SelectedItemButton(QString title = "", QImage image = QImage(), QWidget* parent = Q_NULLPTR); // : TianLiQtCommon_SelectedItemButton("") {}
	//TianLiQtCommon_SelectedItemButton(QString title, QWidget* parent = Q_NULLPTR) :TianLiQtCommon_SelectedItemButton(title, QImage()) {}
	//TianLiQtCommon_SelectedItemButton(QString title, QImage image, QWidget* parent = Q_NULLPTR);

	~TianLiQtCommon_SelectedItemButton();

private:
	Ui::TianLiQtCommon_SelectedItemButton ui;

public:
	void setTitle(QString title);
	void setImage(QImage image);
};
