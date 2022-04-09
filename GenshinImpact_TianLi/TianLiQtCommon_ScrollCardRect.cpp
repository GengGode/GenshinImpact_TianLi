#include "TianLiQtCommon_ScrollCardRect.h"

#include <QGraphicsDropShadowEffect>
#include <QResizeEvent> 

TianLiQtCommon_ScrollCardRect::TianLiQtCommon_ScrollCardRect(QString title,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.label_Title->setText(title);

	// Ìí¼ÓÒõÓ°
	QGraphicsDropShadowEffect* mainShadow = new QGraphicsDropShadowEffect(this);
	mainShadow->setOffset(2, 2);
	mainShadow->setColor(QColor(0, 0, 0, 26));
	mainShadow->setBlurRadius(5);
	ui.widget->setGraphicsEffect(mainShadow);

	ui.label_Up->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	ui.label_Down->setAttribute(Qt::WA_TransparentForMouseEvents, true);

	ui.verticalLayout->addSpacing(12);
	ui.scrollAreaWidgetContents->layout()->setAlignment(Qt::AlignTop);

}


TianLiQtCommon_ScrollCardRect::~TianLiQtCommon_ScrollCardRect()
{
}

void TianLiQtCommon_ScrollCardRect::setGeometry(int x, int y, int w, int h)
{
	QWidget::setGeometry(x - 10, y - 10, w, h + 20);
}

void TianLiQtCommon_ScrollCardRect::addWidget(QWidget* widget, int stretch, Qt::Alignment alignment)
{
	ui.scrollAreaWidgetContents->layout()->addWidget(widget);
}

void TianLiQtCommon_ScrollCardRect::resizeEvent(QResizeEvent* event)
{
	const int w = event->size().width();
	const int h = event->size().height();
	ui.label_Up->setGeometry(10, 30, w - 20, 24);
	ui.label_Down->setGeometry(10, h-40, w - 20, 24);
	ui.widget->setGeometry(10,10,w-20,h-20);
}
