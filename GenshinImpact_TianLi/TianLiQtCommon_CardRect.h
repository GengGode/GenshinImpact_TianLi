#pragma once

#include <QWidget>
#include "ui_TianLiQtCommon_CardRect.h"

class TianLiQtCommon_CardRect : public QWidget
{
	Q_OBJECT

public:
	TianLiQtCommon_CardRect(QWidget* parent = Q_NULLPTR);
	~TianLiQtCommon_CardRect();

private:
	Ui::TianLiQtCommon_CardRect ui;

public:
	void setGeometry(int x, int y, int w, int h);
};