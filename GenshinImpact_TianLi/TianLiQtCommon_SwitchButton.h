#pragma once

#include <QPushButton>
#include "ui_TianLiQtCommon_SwitchButton.h"

class TianLiQtCommon_SwitchButton : public QPushButton
{
	Q_OBJECT

public:
	TianLiQtCommon_SwitchButton(QWidget *parent = nullptr,QString title = "");
	~TianLiQtCommon_SwitchButton();

private:
	Ui::TianLiQtCommon_SwitchButtonClass ui;

public:
	void setTitle(QString title);
private:
	void clicked_switch(bool is_checked);
};
