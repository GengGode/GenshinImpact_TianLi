#pragma once

#include <QPushButton>
#include "ui_TianLiQtCommon_TypeGroupButton.h"

class TianLiQtCommon_TypeGroupButton : public QPushButton
{
	Q_OBJECT

public:
	TianLiQtCommon_TypeGroupButton(QString title, QWidget *parent = Q_NULLPTR);
	~TianLiQtCommon_TypeGroupButton();

private:
	Ui::TianLiQtCommon_TypeGroupButton ui;
};
