#pragma execution_character_set("utf-8")

#include "GenshinImpact_TianLi.h"

#include "TianLiQtCommon_MapRect.h"
#include "TianLiQtCommon_CardRect.h"
#include "TianLiQtCommon_ScrollCardRect.h"
#include "TianLiQtCommon_SelectedItemButton.h"
#include "TianLiQtCommon_PickedItemButton.h"
#include "TianLiQtCommon_NearbyItemButton.h"

#include <QMouseEvent>

GenshinImpact_TianLi::GenshinImpact_TianLi(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	mainShadow = new QGraphicsDropShadowEffect(this);
	mainShadow->setOffset(0, 0);
	mainShadow->setColor(QColor(0, 0, 0, 255));
	mainShadow->setBlurRadius(15);

	ui.label_Main->setGraphicsEffect(mainShadow);

	this->addUI_MapRect();
	this->addUI_CardRects();

	connect(ui.pushButton_Tab_1, &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButton_Tab_1_clicked);
	connect(ui.pushButton_Tab_2, &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButton_Tab_2_clicked);
	connect(ui.pushButton_Tab_3, &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButton_Tab_3_clicked);
	connect(ui.pushButton_Tab_4, &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButton_Tab_4_clicked);

}

void GenshinImpact_TianLi::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton &&
		ui.label_Main->frameRect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		leftBtnClk = true;
	}
	event->ignore();
}

void GenshinImpact_TianLi::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		leftBtnClk = false;
	}
	event->ignore();
}
void GenshinImpact_TianLi::mouseMoveEvent(QMouseEvent* event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}

void GenshinImpact_TianLi::addUI_MapRect()
{
	TianLiQtCommon_MapRect* page1_mapRect = new TianLiQtCommon_MapRect(this);

	page1_mapRect->setParent(ui.widget_3);
	page1_mapRect->setGeometry(10,10, ui.widget_MapMask->width()-20, ui.widget_MapMask->height()-20);

}

void GenshinImpact_TianLi::addUI_CardRects()
{
	TianLiQtCommon_CardRect* page1_cardRect_1_3 = new TianLiQtCommon_CardRect(this);
	page1_cardRect_1_3->setParent(ui.widget_2);
	page1_cardRect_1_3->setGeometry(36, 352, 302, 355);

	TianLiQtCommon_CardRect* page1_cardRect_1_2 = new TianLiQtCommon_CardRect(this);
	page1_cardRect_1_2->setParent(ui.widget_2);
	page1_cardRect_1_2->setGeometry(36, 112, 302, 397);

	TianLiQtCommon_CardRect* page1_cardRect_1_1 = new TianLiQtCommon_CardRect(this);
	page1_cardRect_1_1->setParent(ui.widget_2);
	page1_cardRect_1_1->setGeometry(36, 28, 302, 148);

	TianLiQtCommon_ScrollCardRect* page1_cardRect_2_4 = new TianLiQtCommon_ScrollCardRect("捡取物品日志", this);
	page1_cardRect_2_4->setParent(ui.widget_2);
	page1_cardRect_2_4->setGeometry(350, 551, 260, 160);
	for (int i = 0; i < 2; i++)
	{
		QImage im;
		im.load(":/Test/resource/Test/Tex_0537_0.png");
		TianLiQtCommon_PickedItemButton* asd = new TianLiQtCommon_PickedItemButton(QString::number(i) + "号物品xxx",im);
		page1_cardRect_2_4->addWidget(asd);
	}

	TianLiQtCommon_ScrollCardRect* page1_cardRect_2_3 = new TianLiQtCommon_ScrollCardRect("附近物品日志", this);
	page1_cardRect_2_3->setParent(ui.widget_2);
	page1_cardRect_2_3->setGeometry(350, 380, 260, 160);
	for (int i = 0; i < 2; i++)
	{
		QImage im;
		im.load(":/Test/resource/Test/Tex_0537_0.png");
		TianLiQtCommon_NearbyItemButton* asd = new TianLiQtCommon_NearbyItemButton(QString::number(i) + "号物品xxx", im);
		
		page1_cardRect_2_3->addWidget(asd);
	}


	TianLiQtCommon_ScrollCardRect* page1_cardRect_2_2 = new TianLiQtCommon_ScrollCardRect("选中物品", this);
	page1_cardRect_2_2->setParent(ui.widget_2);
	page1_cardRect_2_2->setGeometry(350, 81, 260, 288);
	for (int i = 0; i < 10; i++)
	{
		TianLiQtCommon_SelectedItemButton* asd = new TianLiQtCommon_SelectedItemButton(QString::number(i) + "号物品xxx",QImage(":/Test/resource/Test/Tex_0537_0.png"));
		page1_cardRect_2_2->addWidget(asd);
	}


	TianLiQtCommon_CardRect* page1_cardRect_2_1 = new TianLiQtCommon_CardRect(this);
	page1_cardRect_2_1->setParent(ui.widget_2);
	page1_cardRect_2_1->setGeometry(350, 28, 260, 42);

}

void GenshinImpact_TianLi::pushButton_Tab_1_clicked(bool checked)
{
	if (checked == true)
	{
		this->setCurrentIndex_MainTabPages(0);
	}
}

void GenshinImpact_TianLi::pushButton_Tab_2_clicked(bool checked)
{
	if (checked == true)
	{
		this->setCurrentIndex_MainTabPages(1);
	}
}

void GenshinImpact_TianLi::pushButton_Tab_3_clicked(bool checked)
{
	if (checked == true)
	{
		this->setCurrentIndex_MainTabPages(2);
	}
}

void GenshinImpact_TianLi::pushButton_Tab_4_clicked(bool checked)
{
	if (checked == true)
	{
		this->setCurrentIndex_MainTabPages(3);
	}
}

void GenshinImpact_TianLi::setCurrentIndex_MainTabPages(int index)
{
	ui.stackedWidget_MainTabPages->setCurrentIndex(index);
}
