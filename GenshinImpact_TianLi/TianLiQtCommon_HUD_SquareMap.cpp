#include "TianLiQtCommon_HUD_SquareMap.h"
#include <QPainter>
#include <QCloseEvent>
#include <QWindow>
#include <Windows.h>

TianLiQtCommon_HUD_SquareMap::TianLiQtCommon_HUD_SquareMap(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_QuitOnClose, false);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	
}

TianLiQtCommon_HUD_SquareMap::~TianLiQtCommon_HUD_SquareMap()
{
}

void TianLiQtCommon_HUD_SquareMap::paintEvent(QPaintEvent* event)
{
	//QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing);
	//painter.setBrush(Qt::blue);
	//painter.drawRect(0, 0, this->width(), this->height());
	
}

void TianLiQtCommon_HUD_SquareMap::closeEvent(QCloseEvent* event)
{
	event->accept();
	emit signal_close_finished();
}

void TianLiQtCommon_HUD_SquareMap::slot_update()
{
}
