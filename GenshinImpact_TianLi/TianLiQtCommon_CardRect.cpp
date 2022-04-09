#include "TianLiQtCommon_CardRect.h"

#include <QGraphicsDropShadowEffect>

TianLiQtCommon_CardRect::TianLiQtCommon_CardRect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// Ìí¼ÓÒõÓ°
	QGraphicsDropShadowEffect* mainShadow = new QGraphicsDropShadowEffect(this);
	mainShadow->setOffset(2, 2);
	mainShadow->setColor(QColor(0, 0, 0, 26));
	mainShadow->setBlurRadius(5);

	ui.widget->setGraphicsEffect(mainShadow);
}

TianLiQtCommon_CardRect::~TianLiQtCommon_CardRect()
{
}

void TianLiQtCommon_CardRect::setGeometry(int x, int y, int w, int h)
{
	QWidget::setGeometry(x - 10, y - 10, w, h+20);
}
