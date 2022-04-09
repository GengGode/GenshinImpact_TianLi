#include "TianLiQtCommon_MapRect.h"

#include <QMouseEvent>
#include <QResizeEvent> 

TianLiQtCommon_MapRect::TianLiQtCommon_MapRect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

TianLiQtCommon_MapRect::~TianLiQtCommon_MapRect()
{
}

void TianLiQtCommon_MapRect::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton &&
		ui.label->frameRect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		leftBtnClk = true;
	}
	event->ignore();
}

void TianLiQtCommon_MapRect::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		leftBtnClk = false;
	}
	event->ignore();
}

void TianLiQtCommon_MapRect::mouseMoveEvent(QMouseEvent* event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}

void TianLiQtCommon_MapRect::resizeEvent(QResizeEvent* event)
{
	const int w = event->size().width();
	const int h = event->size().height();
	ui.widget_MapMask->setGeometry(0, 0, w, h);
}
