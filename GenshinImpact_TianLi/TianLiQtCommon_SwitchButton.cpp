#include "TianLiQtCommon_SwitchButton.h"
#include "QPropertyAnimation"

#include "..\GenshinImpact_TianLi_Core\GenshinImpact_TianLi_Core.h"
#pragma comment(lib,"GenshinImpact_TianLi_Core.lib")

TianLiQtCommon_SwitchButton::TianLiQtCommon_SwitchButton(QWidget *parent, QString title)
	: QPushButton(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	setTitle(title);

	connect(this, &TianLiQtCommon_SwitchButton::clicked, this, &TianLiQtCommon_SwitchButton::clicked_switch);
}

TianLiQtCommon_SwitchButton::~TianLiQtCommon_SwitchButton()
{}

void TianLiQtCommon_SwitchButton::paintEvent(QPaintEvent* event)
{
}

void TianLiQtCommon_SwitchButton::setTitle(QString title)
{
	ui.label_Text->setText(title);
}
void TianLiQtCommon_SwitchButton::slot_clicked(bool is_checked)
{
	clicked_switch(is_checked);
}

void TianLiQtCommon_SwitchButton::clicked_switch(bool is_checked)
{
	if (is_checked)
	{
		//ui.label_Switch->move(22, 4);
		//Ê¹ÓÃ¶¯»­
		QPropertyAnimation* change_animation = new QPropertyAnimation(ui.label_Switch, "geometry");
		QSize size = ui.label_Switch->size();
		change_animation->setDuration(200);
		change_animation->setEndValue(QRect(22, 4,size.width(), size.height()));
		change_animation->start();
		// Core.GetTrack().StartServer();
		connect(change_animation, &QPropertyAnimation::finished, [=] {
			emit signal_clicked(is_checked);
			
			ui.label_Background->setStyleSheet(QString::fromUtf8(".QLabel\n"
				"{\n"
				"background-color:rgb(120, 222, 222);\n"
				"border:3px solid rgb(170, 170, 0);\n"
				"border-radius:12px;\n"
				"}"));
			});
	}
	else
	{
		//ui.label_Switch->move(4, 4);
		QPropertyAnimation* change_animation = new QPropertyAnimation(ui.label_Switch, "geometry");
		QSize size = ui.label_Switch->size();
		change_animation->setDuration(200);
		change_animation->setEndValue(QRect(4, 4, size.width(), size.height()));
		change_animation->start();
		// Core.GetTrack().StartServer();
		connect(change_animation, &QPropertyAnimation::finished, [=] {
			emit signal_clicked(is_checked);
			
			ui.label_Background->setStyleSheet(QString::fromUtf8(".QLabel\n"
				"{\n"
				"background-color:rgb(222, 222, 222);\n"
				"border:3px solid rgb(170, 170, 0);\n"
				"border-radius:12px;\n"
				"}"));
			});
	}
}
