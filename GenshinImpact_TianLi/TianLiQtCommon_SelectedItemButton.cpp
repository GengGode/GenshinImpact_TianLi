#include "TianLiQtCommon_SelectedItemButton.h"

#include <QGraphicsDropShadowEffect>

TianLiQtCommon_SelectedItemButton::TianLiQtCommon_SelectedItemButton(QString item_name , QString type_name, QString area_name, QImage item_image , QImage type_image , QWidget *parent)
	: QPushButton(parent)
{
	ui.setupUi(this);
	// QSS qlineargradient(spread:repeat, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 0), stop:0.227273 rgba(255, 255, 255, 82), stop:0.539773 rgba(255, 255, 255, 255), stop:0.789773 rgba(255, 255, 255, 149), stop:1 rgba(255, 255, 255, 0));
	item = item_name;
	type = type_name;
	area = area_name;

	setTitle(item);
	setRegion(area);
	setImage(item_image);
	setLabelImage(type_image);

	QGraphicsDropShadowEffect* titleShadow = new QGraphicsDropShadowEffect();
	titleShadow->setOffset(0, 4);
	titleShadow->setColor(QColor(0, 0, 0, 64));
	titleShadow->setBlurRadius(15);
	this->setGraphicsEffect(titleShadow);

	// 添加进度自发光阴影
	QGraphicsDropShadowEffect* progressAShadow = new QGraphicsDropShadowEffect();
	progressAShadow->setOffset(0,0);
	progressAShadow->setColor(QColor(0, 171, 255, 245));
	progressAShadow->setBlurRadius(5);
	ui.label_Progress_A->setGraphicsEffect(progressAShadow);
	// 添加进度自发光阴影
	QGraphicsDropShadowEffect* progressBShadow = new QGraphicsDropShadowEffect();
	progressBShadow->setOffset(0, 0);
	progressBShadow->setColor(QColor(98, 255, 255, 255));
	progressBShadow->setBlurRadius(15);
	ui.label_Progress_B->setGraphicsEffect(progressBShadow);

	ui.label_Image->setScaledContents(true);
	
	QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect;
	opacity->setOpacity(0.2); 
	ui.label_LabelImage->setGraphicsEffect(opacity);
}

TianLiQtCommon_SelectedItemButton::~TianLiQtCommon_SelectedItemButton()
{
}

void TianLiQtCommon_SelectedItemButton::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit signal_double_click(isChecked());
}

QString TianLiQtCommon_SelectedItemButton::item_name()
{
	return item;
}

QString TianLiQtCommon_SelectedItemButton::type_name()
{
	return type;
}

QString TianLiQtCommon_SelectedItemButton::area_name()
{
	return area;
}

void TianLiQtCommon_SelectedItemButton::setTitle(QString title)
{
	ui.label_Title->setText(title);
}

void TianLiQtCommon_SelectedItemButton::setRegion(QString region)
{
	ui.label_Region->setText(region);
}

void TianLiQtCommon_SelectedItemButton::setImage(QImage image)
{
	ui.label_Image->setPixmap(QPixmap::fromImage(image));
}

void TianLiQtCommon_SelectedItemButton::setLabelImage(QImage image)
{
	ui.label_LabelImage->setPixmap(QPixmap::fromImage(image));
}

void TianLiQtCommon_SelectedItemButton::setProgressCount(int number)
{
	if(number<0)
	{
		progress_count = 0;
	}
	else if (number > progress_max_number)
	{
		progress_count = progress_max_number;
	}
	else
	{
		progress_count = number;
	}
	const int w = 85;
	double i = (double)progress_count / (double)progress_max_number;
	
	ui.label_Progress_A->setGeometry(133, 18, static_cast<int>(w * i), 2);
	ui.label_Progress_B->setGeometry(133, 18, static_cast<int>(w * i), 2);
}

void TianLiQtCommon_SelectedItemButton::setProgressMaxNumber(int number)
{
	if (number <= 0)
	{
		progress_max_number = 1;
	}
	else if (progress_count > number)
	{
		progress_count = number;
		progress_max_number = number;
	}
	else
	{
		progress_max_number = number;
	}
	const int w = 85;

	double i = (double)progress_count / (double)progress_max_number;

	ui.label_Progress_A->setGeometry(133, 18, static_cast<int>(w * i), 2);
	ui.label_Progress_B->setGeometry(133, 18, static_cast<int>(w * i), 2);
}

