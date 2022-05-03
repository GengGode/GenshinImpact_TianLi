#include "TianLiQtCommon_PickedItemButton.h"

#include <QGraphicsDropShadowEffect>

TianLiQtCommon_PickedItemButton::TianLiQtCommon_PickedItemButton(QString title, QImage image, QWidget* parent)
	: QPushButton(parent)
{
	ui.setupUi(this);
	// QSS qlineargradient(spread:repeat, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 0), stop:0.227273 rgba(255, 255, 255, 82), stop:0.539773 rgba(255, 255, 255, 255), stop:0.789773 rgba(255, 255, 255, 149), stop:1 rgba(255, 255, 255, 0));
	setTitle(title);
	setImage(image);
	ui.label_Image->setScaledContents(true);

	// 0 4 4 25%
	QGraphicsDropShadowEffect *titleShadow = new QGraphicsDropShadowEffect();
	titleShadow->setOffset(0, 4);
	titleShadow->setColor(QColor(0, 0, 0, 64));
	titleShadow->setBlurRadius(15);

	ui.label_Title->setGraphicsEffect(titleShadow);
}

TianLiQtCommon_PickedItemButton::~TianLiQtCommon_PickedItemButton()
{
}

void TianLiQtCommon_PickedItemButton::setTitle(QString title)
{
	ui.label_Title->setText(title);
}

void TianLiQtCommon_PickedItemButton::setImage(QImage image)
{
	ui.label_Image->setPixmap(QPixmap::fromImage(image));
}