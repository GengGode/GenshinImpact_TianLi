#pragma execution_character_set("utf-8")

#include "GenshinImpact_TianLi.h"

#include "TianLiQtCommon_MapRect.h"
#include "TianLiQtCommon_CardRect.h"
#include "TianLiQtCommon_ScrollCardRect.h"
#include "TianLiQtCommon_SelectedItemButton.h"
#include "TianLiQtCommon_PickedItemButton.h"
#include "TianLiQtCommon_NearbyItemButton.h"
#include "TianLiQtCommon_TypeGroupButton.h"
#include "TianLiQtCommon_SwitchButton.h"

#include "TianLiQtCommon_Logger.h"

#include "TianLiQtCommon_HUD_CircularMap.h"
#include "TianLiQtCommon_HUD_SquareMap.h"
#include "TianLiQtCommon_HUD_AzimuthBarWindow.h"

#include "TianLiQtCommon_HookKeyBoard.h"
#include "TianLiQtCommon_ListenKeyBoard.h"

#include "TianLiQtCommon_Utils.h"

#include <QFontDatabase>
#include <QMouseEvent>
#include <QTimer>

#include <QNetworkReply>
#include <QNetworkAccessManager>


#include "..\GenshinImpact_TianLi_Core\GenshinImpact_TianLi_Core.h"
#pragma comment(lib,"GenshinImpact_TianLi_Core.lib")

using namespace TianLi;

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


	hud_square_map = new TianLiQtCommon_HUD_SquareMap(NULL);
	hud_square_map->hide();
	hud_circular_map = new TianLiQtCommon_HUD_CircularMap(NULL);
	hud_circular_map->hide();
	hud_azimuth_bar_window = new TianLiQtCommon_HUD_AzimuthBarWindow(NULL);
	hud_azimuth_bar_window->hide();

	//core = Core;
	
	this->loadDataBase();
	this->loadUIBase();
	emit this->ui_updatePusButtonList();

	//GenshinImpact_TianLi_Track tianli_track;
	

	//httplib::Client cli("http://download.api.weixitianli.com");
	//auto res = cli.Get("/GenshinImpactTianLi/Version/Latest");
	//LogInfo(res->body.c_str());
	
	// Qt HttpGet
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
		if (reply->error() == QNetworkReply::NoError) {
			QByteArray bytes = reply->readAll();
			QString result(bytes);
			LogInfo(result.toStdString().c_str());
		}
		else {
			LogError("HttpGet Error");
		}
		reply->deleteLater();
		});
	manager->get(QNetworkRequest(QUrl("http://download.api.weixitianli.com/GenshinImpactTianLi/Version/Latest")));
	

	
	//添加全局快捷键
	// F1 触发 slot_show_or_hide
	//hook_key_board_list.push_back(new TianLiQtCommon_HookKeyBoard("F1", this));
	//connect(hook_key_board_list.back(), &TianLiQtCommon_HookKeyBoard::signal_activated, this, &GenshinImpact_TianLi::slot_show_or_hide);
	
	hook_key_board_list.push_back(new TianLiQtCommon_HookKeyBoard("Alt+T", this));
	connect(hook_key_board_list.back(), &TianLiQtCommon_HookKeyBoard::signal_activated, this, &GenshinImpact_TianLi::slot_auto_track);
	// listen TianLiQtCommon_ListenKeyBoard
	
	TianLiQtCommon_ListenKeyBoard* listen_key_board = new TianLiQtCommon_ListenKeyBoard(this);
	
	
	// listen_key_board->register_key_signal(0x41, this, &GenshinImpact_TianLi::pushButton_Tab_1_clicked);
	// F1 触发 slot_show_or_hide
	listen_key_board->register_key(0x70, this, &GenshinImpact_TianLi::slot_show_or_hide);
	
	
	
	//connect(this, &GenshinImpact_TianLi::show, this, &GenshinImpact_TianLi::slot_show);
	//connect(this, &GenshinImpact_TianLi::hide, this, &GenshinImpact_TianLi::slot_hide);
	
	connect(ui.pushButton_Tab_1, &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButton_Tab_1_clicked);
	connect(ui.pushButton_Tab_2, &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButton_Tab_2_clicked);
	connect(ui.pushButton_Tab_3, &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButton_Tab_3_clicked);
	connect(ui.pushButton_Tab_4, &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButton_Tab_4_clicked);

}

GenshinImpact_TianLi::~GenshinImpact_TianLi()
{
}

void GenshinImpact_TianLi::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton &&
		ui.label_Main->frameRect().contains(event->globalPos() - this->frameGeometry().topLeft()))
	{
		{

			m_Press = event->globalPos();
			leftBtnClk = true;
		}
	}
	//event->ignore();
}
void GenshinImpact_TianLi::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		leftBtnClk = false;
	}
	//event->ignore();
}
void GenshinImpact_TianLi::mouseMoveEvent(QMouseEvent* event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	//event->ignore();
}

void GenshinImpact_TianLi::closeEvent(QCloseEvent* event)
{
	//event->ignore();
	slot_hide();
	event->ignore();
}

void GenshinImpact_TianLi::loadDataBase()
{
	TianLi::SqliteDbMem SqliteDB_Mem = Core.GetResource().LoadSqlite_KYJGDB();
	Core.GetSqlite().OpenSqlite(SqliteDB_Mem.ptr, SqliteDB_Mem.size);
	

	updata_Country();
	
	updata_TypeList();
	
	updata_ItemList();
	
	updata_ItemsList();
}


void GenshinImpact_TianLi::updata_Country()
{
	TextVector countryTextVector;
	
	// 清空 可选地区
	strList_Addr.clear();

	// 加载国家地区
	Core.GetSqlite().ReadCountry(countryTextVector);
	// 如果读取到的数据是空的
	if (countryTextVector.size == 0)
	{
		return;
	}
	for (int i = 0; i < countryTextVector.size; i++)
	{
		strList_Addr << countryTextVector[i];
	}
	

	// 选中第一个国家地区
	selectedStr_Addr = strList_Addr[0];
}

void GenshinImpact_TianLi::updata_TypeList()
{
	TextVector typeTextVector;
	
	// 清空 可选类型
	strList_Type.clear();

	if (selectedStr_Addr == "")
	{
		selectedStr_Type = "";
		return;
	}
	// 加载该国家地区下的类型
	Core.GetSqlite().ReadType(selectedStr_Addr.toStdString().c_str(), typeTextVector);
	// 如果读取到的数据是空的
	if (typeTextVector.size == 0)
	{
		return;
	}
	for (int i = 0; i < typeTextVector.size; i++)
	{
		strList_Type << typeTextVector[i];
	}

	// 选中第一个类型
	selectedStr_Type = strList_Type[0];
}

void GenshinImpact_TianLi::updata_ItemList()
{
	TextVector itemTextVector;
	
	// 清空 可选种类
	strList_Item.clear();

	// 加载该类型下的种类
	Core.GetSqlite().ReadItem(selectedStr_Addr.toStdString().c_str(), selectedStr_Type.toStdString().c_str(), itemTextVector);
	// 如果读取到的数据是空的
	if (itemTextVector.size == 0)
	{
		return;
	}
	for (int i = 0; i < itemTextVector.size; i++)
	{
		strList_Item << itemTextVector[i];
	}

	// 选中第一个种类
	selectedStr_Item = strList_Item[0];
}

void GenshinImpact_TianLi::updata_ItemsList()
{
	ItemsVector itemsItemsVector;

	// 清空 可选物品
	strList_Items.clear();

	// 加载该种类下的物品
	Core.GetSqlite().ReadItems(selectedStr_Addr.toStdString().c_str(), selectedStr_Type.toStdString().c_str(), selectedStr_Item.toStdString().c_str(), itemsItemsVector);
	// 如果读取到的数据是空的
	if (itemsItemsVector.size == 0)
	{
		return;
	}
	BadgeInfo::BadgeBlock legend_block;
	legend_block.name = itemsItemsVector[0].name;
	legend_block.image = cv::Mat(cv::Size(64, 64), CV_8UC4,cv::Scalar(100,200,200,128));//itemsItemsVector[0].image;
	for (int i = 0; i < itemsItemsVector.size; i++)
	{
		BadgeInfo::BadgeBlock::Badge legend;
		legend.x = itemsItemsVector[i].x;
		legend.y = itemsItemsVector[i].x;
		legend.z = itemsItemsVector[i].x;
		legend.message = itemsItemsVector[i].msg;
		
		strList_Items << itemsItemsVector[i].name;
		// LogTrace(itemsItemsVector[i].msg);
		
		legend_block.badge_list.push_back(legend);
	}
	Core.GetMap().badge_info.badge_block_list.push_back(legend_block);
	
}

void GenshinImpact_TianLi::addUI_Tab_Map()
{
	this->addUI_MapTabCardRects();
	this->addUI_MapTabMapRect();
	this->update();
}
void GenshinImpact_TianLi::addUI_Tab_HUD()
{
	this->addUI_HUDTabCardRects();
	this->update();
}
void GenshinImpact_TianLi::addUI_Tab_3()
{

}
void GenshinImpact_TianLi::addUI_Tab_4()
{
	auto button = new TianLiQtCommon_SwitchButton(ui.page_4, "最小化");
	
	button->setParent(ui.page_4);
	button->move(30, ui.page_4->height() - 35);

	//connect(PageTabMap_MapRect[0], &TianLiQtCommon_MapRect::signal_double_click, dynamic_cast<TianLiQtCommon_SwitchButton*>(PageTabMap_RightCard_Buttons[0]), &TianLiQtCommon_SwitchButton::slot_clicked);
}


void GenshinImpact_TianLi::loadUIBase()
{
	this->addUI_Tab_Map();
	this->addUI_Tab_HUD();
	this->addUI_Tab_3();
	this->addUI_Tab_4();
	this->setCurrentIndex_MainTabPages(0);
}

void GenshinImpact_TianLi::addUI_MapTabCardRects()
{
	// 地图页面左侧的左栏上的卡片
	PageTabMap_LeftCardRects.append(new TianLiQtCommon_CardRect(this));
	PageTabMap_LeftCardRects[0]->setParent(ui.widget_MapTab_Left);
	PageTabMap_LeftCardRects[0]->setGeometry(36, 28, 302, 148);

	PageTabMap_LeftCardRects.append(new TianLiQtCommon_CardRect(this));
	PageTabMap_LeftCardRects[1]->setParent(ui.widget_MapTab_Left);
	PageTabMap_LeftCardRects[1]->setGeometry(36, 112, 302, 397);
	
	PageTabMap_LeftCardRects.append(new TianLiQtCommon_CardRect(this));
	PageTabMap_LeftCardRects[2]->setParent(ui.widget_MapTab_Left);
	PageTabMap_LeftCardRects[2]->setGeometry(36, 352, 302, 355); 
	
	// 对地图页面左侧的左栏上的卡片进行遮盖排序
	PageTabMap_LeftCardRects[2]->raise();
	PageTabMap_LeftCardRects[1]->raise();
	PageTabMap_LeftCardRects[0]->raise();
	
	// 地图页面左侧的右栏上的卡片
	PageTabMap_RightCardRects.append(new TianLiQtCommon_CardRect(this));
	PageTabMap_RightCardRects[0]->setParent(ui.widget_MapTab_Left);
	PageTabMap_RightCardRects[0]->setGeometry(350, 28, 260, 42);
	
	// 地图页面左侧的右栏上的卡片
	PageTabMap_ScrollCardRect.append(new TianLiQtCommon_ScrollCardRect("选中物品", this));
	PageTabMap_ScrollCardRect[0]->setParent(ui.widget_MapTab_Left);
	PageTabMap_ScrollCardRect[0]->setGeometry(350, 81, 260, 288);
	
	PageTabMap_ScrollCardRect.append(new TianLiQtCommon_ScrollCardRect("附近物品日志", this));
	PageTabMap_ScrollCardRect[1]->setParent(ui.widget_MapTab_Left);
	PageTabMap_ScrollCardRect[1]->setGeometry(350, 380, 260, 160);

	PageTabMap_ScrollCardRect.append(new TianLiQtCommon_ScrollCardRect("捡取物品日志", this));
	PageTabMap_ScrollCardRect[2]->setParent(ui.widget_MapTab_Left);
	PageTabMap_ScrollCardRect[2]->setGeometry(350, 551, 260, 160);

}

void GenshinImpact_TianLi::addUI_MapTabMapRect()
{
	// 地图页面右侧的地图区域
	PageTabMap_MapRect.append(new TianLiQtCommon_MapRect(this));
	PageTabMap_MapRect[0]->setParent(ui.widget_MapTab_Right);
	PageTabMap_MapRect[0]->setGeometry(10, 10, ui.widget_MapMask->width() - 20, ui.widget_MapMask->height() - 20);

	PageTabMap_RightCard_Buttons.append(new TianLiQtCommon_SwitchButton(this,"定位"));
	PageTabMap_RightCard_Buttons[0]->setParent(ui.widget_MapTab_Right);
	PageTabMap_RightCard_Buttons[0]->move(30, PageTabMap_MapRect[0]->height() - 35);

	
	
	connect(dynamic_cast<TianLiQtCommon_SwitchButton*>(PageTabMap_RightCard_Buttons[0]), &TianLiQtCommon_SwitchButton::signal_clicked, [](bool is_checked){
		if (is_checked)
		{
			Core.GetTrack().StartServer();
		}
		else
		{
			Core.GetTrack().StopServer();
		}
		});
	connect(PageTabMap_MapRect[0], &TianLiQtCommon_MapRect::signal_double_click, dynamic_cast<TianLiQtCommon_SwitchButton*>(PageTabMap_RightCard_Buttons[0]), &TianLiQtCommon_SwitchButton::slot_clicked);
	
}

void GenshinImpact_TianLi::addUI_HUDTabCardRects()
{
	// 1259 * 739 三份均分
	// 可用宽度为 1259 - 36*2 = 1223
	// 可用高度为 739 - 28*2 = 671
	// 三分之一宽度为 1223 / 3 = 396


	// 右边
	PageTabHUD_CardRects.append(new TianLiQtCommon_CardRect(this));
	PageTabHUD_CardRects.last()->setParent(ui.widget_HUDTab);
	PageTabHUD_CardRects.last()->setGeometry(36, 28, 396, 671);
	
	// 中间
	PageTabHUD_CardRects.append(new TianLiQtCommon_CardRect(this));
	PageTabHUD_CardRects.last()->setParent(ui.widget_HUDTab);
	PageTabHUD_CardRects.last()->setGeometry(36+396, 28, 396, 671);
	
	// 左边
	PageTabHUD_CardRects.append(new TianLiQtCommon_CardRect(this));
	PageTabHUD_CardRects.last()->setParent(ui.widget_HUDTab);
	PageTabHUD_CardRects.last()->setGeometry(36+396*2, 28, 396, 671);
	
	//添加标题文字 放在中间
	QLabel* page2_label_1_1 = new QLabel(this);
	page2_label_1_1->setParent(PageTabHUD_CardRects[0]);
	page2_label_1_1->setGeometry(0, 0, 396, 100);
	// 设置文字上下左右居中
	page2_label_1_1->setAlignment(Qt::AlignCenter);
	// 设置文字为 悬浮窗
	page2_label_1_1->setText("悬浮窗");
	// 字体设置为 HYWenHei 字号为 32pt
	page2_label_1_1->setFont(QFont("HYWenHei", 32));
	// 设置文字颜色为 棕色
	page2_label_1_1->setStyleSheet("color:rgb(153,102,0)");
	
	

	//添加标题文字 放在中间
	QLabel* page2_label_1_2 = new QLabel(this);
	page2_label_1_2->setParent(PageTabHUD_CardRects[1]);
	page2_label_1_2->setGeometry(0, 0, 396, 100);
	// 设置文字上下左右居中
	page2_label_1_2->setAlignment(Qt::AlignCenter);
	// 设置文字为 导航方位条
	page2_label_1_2->setText("导航方位条");
	// 字体设置为 HYWenHei 字号为 32pt
	page2_label_1_2->setFont(QFont("HYWenHei", 32));
	// 设置文字颜色为 棕色
	page2_label_1_2->setStyleSheet("color:rgb(153,102,0)");



#define Temp
#ifdef Temp


	QPushButton* page2_button_1_1 = new QPushButton(this);
	page2_button_1_1->setParent(PageTabHUD_CardRects[0]);
	page2_button_1_1->setGeometry(10, 120, 376, 80);
	page2_button_1_1->setText("未开启");
	// 字体设置为 HYWenHei 字号为 32pt
	page2_button_1_1->setFont(QFont("HYWenHei", 32));
	// 设置文字颜色为 棕色
	page2_button_1_1->setStyleSheet("color:rgb(153,102,0)");
	// 设置按钮不会弹起
	page2_button_1_1->setAttribute(Qt::WA_Hover, false);
	// 设置按钮不会获取焦点
	page2_button_1_1->setFocusPolicy(Qt::NoFocus);
	// 连接按钮点击事件
	connect(hud_square_map, &TianLiQtCommon_HUD_SquareMap::signal_close_finished, page2_button_1_1, [=]() {
		// 改变按钮文字为 未开启
		page2_button_1_1->setText("未开启");
		// 设置按钮文字颜色为 红色
		page2_button_1_1->setStyleSheet("color:rgb(255,0,0)");

		});
	connect(page2_button_1_1, &QPushButton::clicked, [=]() {
		// 如果按钮文字为 未开启
		if (page2_button_1_1->text() == "未开启")
		{
			// 改变按钮文字为 已开启
			page2_button_1_1->setText("已开启");
			// 设置按钮文字颜色为 绿色
			page2_button_1_1->setStyleSheet("color:rgb(0,255,0)");

			// 显示空白无边框窗口
			hud_square_map->slot_show();

		}
		// 如果按钮文字为 已开启
		else if (page2_button_1_1->text() == "已开启")
		{
			// 改变按钮文字为 未开启
			page2_button_1_1->setText("未开启");
			// 设置按钮文字颜色为 红色
			page2_button_1_1->setStyleSheet("color:rgb(255,0,0)");

			// 隐藏空白无边框窗口
			hud_square_map->slot_hide();
		}
		});
	



	QPushButton* page2_button_1_2 = new QPushButton(this);
	page2_button_1_2->setParent(PageTabHUD_CardRects[0]);
	page2_button_1_2->setGeometry(10, 300 , 376, 80);
	page2_button_1_2->setText("未开启");
	// 字体设置为 HYWenHei 字号为 32pt
	page2_button_1_2->setFont(QFont("HYWenHei", 32));
	// 设置文字颜色为 棕色
	page2_button_1_2->setStyleSheet("color:rgb(153,102,0)");
	// 设置按钮不会弹起
	page2_button_1_2->setAttribute(Qt::WA_Hover, false);
	// 设置按钮不会获取焦点
	page2_button_1_2->setFocusPolicy(Qt::NoFocus);
	// 连接按钮点击事件
	connect(hud_circular_map, &TianLiQtCommon_HUD_CircularMap::signal_close_finished, page2_button_1_2, [=]() {
		// 改变按钮文字为 未开启
		page2_button_1_2->setText("未开启");
		// 设置按钮文字颜色为 红色
		page2_button_1_2->setStyleSheet("color:rgb(255,0,0)");

		});
	connect(page2_button_1_2, &QPushButton::clicked, [=]() {
		// 如果按钮文字为 未开启
		if (page2_button_1_2->text() == "未开启")
		{
			// 改变按钮文字为 已开启
			page2_button_1_2->setText("已开启");
			// 设置按钮文字颜色为 绿色
			page2_button_1_2->setStyleSheet("color:rgb(0,255,0)");

			// 显示空白无边框窗口
			hud_circular_map->slot_show();

		}
		// 如果按钮文字为 已开启
		else if (page2_button_1_2->text() == "已开启")
		{
			// 改变按钮文字为 未开启
			page2_button_1_2->setText("未开启");
			// 设置按钮文字颜色为 红色
			page2_button_1_2->setStyleSheet("color:rgb(255,0,0)");

			// 隐藏空白无边框窗口
			hud_circular_map->slot_hide();
		}
		});




	
	QPushButton* page2_button_1_3 = new QPushButton(this);
	page2_button_1_3->setParent(PageTabHUD_CardRects[1]);
	page2_button_1_3->setGeometry(10, 120, 376, 80);
	page2_button_1_3->setText("未开启");
	// 字体设置为 HYWenHei 字号为 32pt
	page2_button_1_3->setFont(QFont("HYWenHei", 32));
	// 设置文字颜色为 棕色
	page2_button_1_3->setStyleSheet("color:rgb(153,102,0)");
	// 设置按钮不会弹起
	page2_button_1_3->setAttribute(Qt::WA_Hover, false);
	// 设置按钮不会获取焦点
	page2_button_1_3->setFocusPolicy(Qt::NoFocus);
	// 连接按钮点击事件
	connect(hud_azimuth_bar_window, &TianLiQtCommon_HUD_AzimuthBarWindow::signal_close_finished, page2_button_1_3, [=]() {
		// 改变按钮文字为 未开启
		page2_button_1_3->setText("未开启");
		// 设置按钮文字颜色为 红色
		page2_button_1_3->setStyleSheet("color:rgb(255,0,0)");

		});
	connect(page2_button_1_3, &QPushButton::clicked, [=]() {
		// 如果按钮文字为 未开启
		if (page2_button_1_3->text() == "未开启")
		{
			// 改变按钮文字为 已开启
			page2_button_1_3->setText("已开启");
			// 设置按钮文字颜色为 绿色
			page2_button_1_3->setStyleSheet("color:rgb(0,255,0)");

			// 显示空白无边框窗口
			hud_azimuth_bar_window->slot_show();

		}
		// 如果按钮文字为 已开启
		else if (page2_button_1_3->text() == "已开启")
		{
			// 改变按钮文字为 未开启
			page2_button_1_3->setText("未开启");
			// 设置按钮文字颜色为 红色
			page2_button_1_3->setStyleSheet("color:rgb(255,0,0)");

			// 隐藏空白无边框窗口
			hud_azimuth_bar_window->slot_hide();

		}
		});
#endif

	
	//添加标题文字 放在中间
	QLabel* page2_label_1_3 = new QLabel(this);
	page2_label_1_3->setParent(PageTabHUD_CardRects[2]);
	page2_label_1_3->setGeometry(0, 0, 396, 100);
	// 设置文字上下左右居中
	page2_label_1_3->setAlignment(Qt::AlignCenter);
	// 设置文字为 信息提示
	page2_label_1_3->setText("信息提示");
	// 字体设置为 HYWenHei 字号为 32pt
	page2_label_1_3->setFont(QFont("HYWenHei", 32));
	// 设置文字颜色为 棕色
	page2_label_1_3->setStyleSheet("color:rgb(153,102,0)");
	
	
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

void GenshinImpact_TianLi::ui_updatePusButtonList()
{
	emit ui_updataCountryButtonList();

	for (int i = 0; i < 5; i++)
	{
		//TianLiQtCommon_SelectedItemButton* asd = new TianLiQtCommon_SelectedItemButton("test", QImage(":/Test/resource/Test/Tex_0537_0.png"));
		//PageTabMap_ScrollCardRect[0]->addWidget(asd);
	}
	
	for (int i = 0; i < 2; i++)
	{
		QImage im;
		im.load(":/Test/resource/Test/Tex_0537_0.png");
		TianLiQtCommon_NearbyItemButton* asd = new TianLiQtCommon_NearbyItemButton(QString::number(i) + "号物品xxx", im);

		PageTabMap_ScrollCardRect[1]->addWidget(asd);
	}
	
	for (int i = 0; i < 2; i++)
	{
		QImage im;
		im.load(":/Test/resource/Test/Tex_0537_0.png");
		TianLiQtCommon_PickedItemButton* asd = new TianLiQtCommon_PickedItemButton(QString::number(i) + "号物品xxx", im);
		PageTabMap_ScrollCardRect[2]->addWidget(asd);
	}


}

void GenshinImpact_TianLi::ui_updataCountryButtonList()
{
	// 清空地区按钮QMap
	for (auto btn : pushButtonMap_Addr)
	{
		// 删除按钮
		delete btn;
	}
	pushButtonMap_Addr.clear();
	
	// 根据 字符串列表 可选地区 创建 地区按钮QMap
	for (auto str : strList_Addr)
	{
		// 创建按钮到 地区按钮QMap 中
		pushButtonMap_Addr.insert(str, new TianLiQtCommon_TypeGroupButton(str, PageTabMap_LeftCardRects[0]));
		// 设置按钮父对象
		pushButtonMap_Addr[str]->setParent(PageTabMap_LeftCardRects[0]);
		// 设置按钮位置
		pushButtonMap_Addr[str]->setGeometry(pushButtonMap_Addr.size() % 5 * 60, pushButtonMap_Addr.size() / 5 * 40, 60, 30);
		// 显示按钮
		pushButtonMap_Addr[str]->show();
		connect(pushButtonMap_Addr[str], &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButtonGroup_SelectCountry);
		
	}
}

void GenshinImpact_TianLi::ui_updataTypeButtonList()
{
	// 清空 类型按钮QMap
	for (auto btn : pushButtonMap_Type)
	{
		// 删除按钮
		delete btn;
	}
	pushButtonMap_Type.clear();
	
	// 根据 字符串列表 可选类型 创建 类型按钮QMap
	for (auto str : strList_Type)
	{
		// 创建按钮到 类型按钮QMap 中
		pushButtonMap_Type.insert(str, new TianLiQtCommon_TypeGroupButton(str, PageTabMap_LeftCardRects[1]));
		// 设置按钮父对象
		pushButtonMap_Type[str]->setParent(PageTabMap_LeftCardRects[1]);
		// 设置按钮位置
		pushButtonMap_Type[str]->setGeometry(pushButtonMap_Type.size() % 5 * 60, 120+pushButtonMap_Type.size() / 5 * 40, 60, 30);
		// 显示按钮
		pushButtonMap_Type[str]->show();
		connect(pushButtonMap_Type[str], &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButtonGroup_SelectType);
	}
}

void GenshinImpact_TianLi::ui_updataItemButtonList()
{
	// 清空 种类按钮QMap
	for (auto btn : pushButtonMap_Item)
	{
		// 删除按钮
		delete btn;
	}
	pushButtonMap_Item.clear();

	// 根据 字符串列表 可选种类 创建 种类按钮QMap
	for (auto str : strList_Item)
	{
		// 创建按钮到 种类按钮QMap 中
		pushButtonMap_Item.insert(str, new TianLiQtCommon_TypeGroupButton(str, PageTabMap_LeftCardRects[2]));
		// 设置按钮父对象
		pushButtonMap_Item[str]->setParent(PageTabMap_LeftCardRects[2]);
		// 设置按钮位置
		pushButtonMap_Item[str]->setGeometry(pushButtonMap_Item.size() % 5 * 60, 200+pushButtonMap_Item.size() / 5 * 40, 60, 30);
		// 显示按钮
		pushButtonMap_Item[str]->show();
		connect(pushButtonMap_Item[str], &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButtonGroup_SelectItem);
	}
}

void GenshinImpact_TianLi::ui_updataItemsButtonList()
{
	// 清空 物品按钮QMap
	//for (auto btn : pushButtonMap_Items)
	//{
	//	// 删除按钮
	//	delete btn;
	//}
	//pushButtonMap_Items.clear();
	
	// 根据 字符串列表 可选物品 创建 物品按钮QMap
	//for (auto str : strList_Items)
	//{
	//	QImage im;
	//	im.load(":/Test/resource/Test/Tex_0537_0.png");
	//	
	//	// 创建按钮到 物品按钮QMap 中
	//	pushButtonMap_Items.insert(str, new TianLiQtCommon_SelectedItemButton(str,im, PageTabMap_ScrollCardRect[0]));
	//	// 设置按钮父对象
	//	pushButtonMap_Items[str]->setParent(PageTabMap_ScrollCardRect[0]);
	//	PageTabMap_ScrollCardRect[0]->addWidget(pushButtonMap_Items[str]);
	//	// 设置按钮位置
	//	pushButtonMap_Items[str]->setGeometry(pushButtonMap_Items.size() % 5 * 60, 280 + pushButtonMap_Items.size() / 5 * 40, 60, 30);
	//	// 显示按钮
	//	pushButtonMap_Items[str]->show();
	//	//connect(pushButtonMap_Items[str], &QPushButton::clicked, this, &GenshinImpact_TianLi::pushButtonGroup_SelectItems);
	//}
}

void GenshinImpact_TianLi::slot_auto_track()
{
	if (Core.GetTrack().ServerState())
	{
		LogInfo("Track状态为已运行，执行停止");
		Core.GetTrack().StopServer();
	}
	else
	{
		LogInfo("Track状态为未运行，执行启动");
		Core.GetTrack().StartServer();
	}
}

void GenshinImpact_TianLi::slot_show()
{
	LogInfo("显示主窗口");
	if (Core.GetTrack().GetResult().is_find_paimon)
	{
		if (main_bebind_widget == nullptr)
		{
			main_bebind_widget = new QWidget();
			RECT gi_client_rect = Core.GetTrack().GetResult().client_rect;
			main_bebind_widget->setGeometry(gi_client_rect.left, gi_client_rect.top, gi_client_rect.right - gi_client_rect.left, gi_client_rect.bottom - gi_client_rect.top);
			main_bebind_widget->setAttribute(Qt::WA_QuitOnClose, false);
			main_bebind_widget->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
			main_bebind_widget->setAttribute(Qt::WA_TranslucentBackground, true);

			/*SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)main_bebind_widget->winId(), GWL_EXSTYLE) |
				WS_EX_TRANSPARENT);*/
			TianLi::Utils::set_window_blur_bebind((HWND)main_bebind_widget->winId());
		}
		
		main_bebind_widget->show();
		main_bebind_widget->activateWindow();
	}
	this->showNormal();
	this->show();
	// 激活到最前面
	this->activateWindow();	
	is_visible = true;
}

void GenshinImpact_TianLi::slot_show_or_hide()
{
	
	if (is_visible)
	{
		this->slot_hide();
	}
	else
	{
		this->slot_show();
	}
}

void GenshinImpact_TianLi::slot_hide()
{
	LogInfo("隐藏主窗口");
	if (main_bebind_widget != nullptr)
	{
		main_bebind_widget->hide();
	}

	this->showMinimized();
	is_visible = false;
	//this->hide();
}

void GenshinImpact_TianLi::setCurrentIndex_MainTabPages(int index)
{
	ui.stackedWidget_MainTabPages->setCurrentIndex(index);
}

void GenshinImpact_TianLi::pushButtonGroup_SelectCountry(bool checked)
{
	if (checked == true)
	{
		// 自身sender
		QPushButton *button = qobject_cast<QPushButton *>(sender());
		// 获取 选中地区文字
		QString str = button->text();
		// 检查 选中地区文字 是否与之前 选中地区 一致
		if (str == selectedStr_Addr)
		{
			// 如果一致 则清除下一级按钮
			selectedStr_Addr = "";
			
			return;
		}
		else
		{
			// 如果不一致 则更新 选中地区
			selectedStr_Addr = str;
		}

		// 更新当前地区可选类型数据
		updata_TypeList();
		// 更新 地区按钮QMap
		ui_updataTypeButtonList();
		
		
	}
}

void GenshinImpact_TianLi::pushButtonGroup_SelectType(bool checked)
{
	if (checked == true)
	{
		// 自身sender
		QPushButton *button = qobject_cast<QPushButton *>(sender());
		// 获取 选中类型文字
		QString str = button->text();
		// 检查 选中类型文字 是否与之前 选中类型 一致
		if (str == selectedStr_Type)
		{
			// 如果一致 则什么都不做
			return;
		}
		else
		{
			// 如果不一致 则更新 选中类型
			selectedStr_Type = str;
			// 更新当前类型可选种类数据
			updata_ItemList();
			// 更新 类型按钮QMap
			ui_updataItemButtonList();
		}

	}
}

void GenshinImpact_TianLi::pushButtonGroup_SelectItem(bool checked)
{
	if (checked == true)
	{
		// 自身sender
		QPushButton* button = qobject_cast<QPushButton*>(sender());
		// 获取 选中种类文字
		QString str = button->text();
		// 检查 选中种类文字 是否与之前 选中种类 一致
		if (str == selectedStr_Item)
		{
			// 如果一致 则什么都不做
			return;
		}
		else
		{
			// 如果不一致 则更新 选中种类
			selectedStr_Item = str;
			// 更新当前种类可选物品数据
			updata_ItemsList();
			// 更新 物品按钮QMap
			// ui_updataItemsButtonList();
			
			QImage im;
			im.load(":/Test/resource/Test/Tex_0537_0.png");

			// 创建按钮到 物品按钮QMap 中
			pushButtonMap_Items.insert(str, new TianLiQtCommon_SelectedItemButton(str,selectedStr_Addr, im,im, PageTabMap_ScrollCardRect[0]));
			// 设置按钮父对象
			pushButtonMap_Items[str]->setParent(PageTabMap_ScrollCardRect[0]);
			PageTabMap_ScrollCardRect[0]->addWidget(pushButtonMap_Items[str]);
			
			
			// 显示按钮
			pushButtonMap_Items[str]->show();
		}
	}
}

void GenshinImpact_TianLi::pushButtonGroup_SelectItems(bool checked)
{
	if (checked == true)
	{
		// 自身sender
		QPushButton* button = qobject_cast<QPushButton*>(sender());
		// 获取 选中种类文字
		QString str = button->text();
		
			//selectedStr_Item = str;
			// 更新当前种类可选物品数据
			//updata_ItemList();
			// 更新 物品按钮QMap
			//ui_updataItemsButtonList();
	}
	else
	{
		// 删除对应按钮
	}
}
