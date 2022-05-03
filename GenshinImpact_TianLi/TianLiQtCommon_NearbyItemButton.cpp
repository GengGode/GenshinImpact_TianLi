#include "TianLiQtCommon_NearbyItemButton.h"

TianLiQtCommon_NearbyItemButton::TianLiQtCommon_NearbyItemButton(QString title, QImage image, QWidget* parent)
	: QPushButton(parent)
{
	ui.setupUi(this);
	setTitle(title);
	setImage(image);
	ui.label_Image->setScaledContents(true);
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