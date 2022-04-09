#pragma once

#include <QWidget>
#include "ui_TianLiQtCommon_ScrollCardRect.h"

class TianLiQtCommon_ScrollCardRect : public QWidget
{
	Q_OBJECT

public:
	TianLiQtCommon_ScrollCardRect(QWidget* parent = Q_NULLPTR) :TianLiQtCommon_ScrollCardRect("", parent) {}
	TianLiQtCommon_ScrollCardRect(QString title,QWidget *parent = Q_NULLPTR);
	~TianLiQtCommon_ScrollCardRect();

private:
	Ui::TianLiQtCommon_ScrollCardRect ui;

public:
	void setGeometry(int x, int y, int w, int h);
public:
	void addWidget(QWidget* widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());

private:
	void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
};
