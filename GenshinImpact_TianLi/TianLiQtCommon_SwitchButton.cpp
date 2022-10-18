#include "TianLiQtCommon_SwitchButton.h"
#include "QPropertyAnimation"

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

void TianLiQtCommon_SwitchButton::setTitle(QString title)
{
	ui.label_Text->setText(title);
}

void TianLiQtCommon_SwitchButton::clicked_switch(bool is_checked)
{
	if (this->isChecked())
	{
		//ui.label_Switch->move(22, 4);
		//Ê¹ÓÃ¶¯»­
		QPropertyAnimation* change_animation = new QPropertyAnimation(ui.label_Switch, "geometry");
		QSize size = ui.label_Switch->size();
		change_animation->setDuration(200);
		change_animation->setEndValue(QRect(22, 4,size.width(), size.height()));
		change_animation->start();
	}
	else
	{
		//ui.label_Switch->move(4, 4);
		QPropertyAnimation* change_animation = new QPropertyAnimation(ui.label_Switch, "geometry");
		QSize size = ui.label_Switch->size();
		change_animation->setDuration(200);
		change_animation->setEndValue(QRect(4, 4, size.width(), size.height()));
		change_animation->start();
	}
}
