#include "TianLiQtCommon_NearbyItemButton.h"

#include <QGraphicsDropShadowEffect>

TianLiQtCommon_NearbyItemButton::TianLiQtCommon_NearbyItemButton(QString title, QImage image, QWidget* parent)
	: QPushButton(parent)
{
	ui.setupUi(this);
	setTitle(title);
	setImage(image);
	ui.label_Image->setScaledContents(true);

	QGraphicsDropShadowEffect* titleShadow = new QGraphicsDropShadowEffect();
	titleShadow->setOffset(0, 4);
	titleShadow->setColor(QColor(0,0,0,191));
	titleShadow->setBlurRadius(15);

	ui.label_Title->setGraphicsEffect(titleShadow);
}

TianLiQtCommon_NearbyItemButton::~TianLiQtCommon_NearbyItemButton()
{
}

void TianLiQtCommon_NearbyItemButton::setTitle(QString title)
{
	ui.label_Title->setText(title);
}

void TianLiQtCommon_NearbyItemButton::setImage(QImage image)
{
	ui.label_Image->setPixmap(QPixmap::fromImage(image));
}