#pragma once

#include <QWidget>
#include "ui_TabMap.h"

class TabMap : public QWidget
{
	Q_OBJECT

public:
	TabMap(QWidget *parent = nullptr);
	~TabMap();

private:
	Ui::TabMapClass ui;
};
