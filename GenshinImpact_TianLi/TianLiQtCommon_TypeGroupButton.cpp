#include "TianLiQtCommon_TypeGroupButton.h"

TianLiQtCommon_TypeGroupButton::TianLiQtCommon_TypeGroupButton(QString title, QWidget* parent)
{
	ui.setupUi(this);
	this->setStyleSheet(QString::fromUtf8("QPushButton{background-color: rgb(255, 255, 255);border-radius: 5px;border: 1px solid rgb(0, 0, 0);}QPushButton:checked{ background-color: rgb(128, 128, 128); border-radius: 5px; border: 1px solid rgb(0, 0, 0);}"));
	this->setFont(QFont("HYWenHei", 10, QFont::Bold));
	this->setCheckable(true);
	this->setCursor(QCursor(Qt::PointingHandCursor));
	this->setText(title);
}

TianLiQtCommon_TypeGroupButton::~TianLiQtCommon_TypeGroupButton()
{
}
