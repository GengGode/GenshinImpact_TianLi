#pragma once

#include <QGraphicsDropShadowEffect>

#include <QtWidgets/QMainWindow>
#include "ui_GenshinImpact_TianLi.h"

class TianLiQtCommon_CardRect;
class TianLiQtCommon_ScrollCardRect;
class TianLiQtCommon_MapRect;
class TianLiQtCommon_SwitchButton;
class TianLiQtCommon_HookKeyBoard;
class TianLiQtCommon_HUD_SquareMap;
class TianLiQtCommon_HUD_CircularMap;
class TianLiQtCommon_HUD_AzimuthBarWindow;

class GenshinImpact_TianLi_Core;
class GenshinImpact_TianLi : public QMainWindow
{
    Q_OBJECT

public:
    GenshinImpact_TianLi(QWidget *parent = Q_NULLPTR);
    ~GenshinImpact_TianLi();

private:
    Ui::GenshinImpact_TianLiClass ui;
    QTimer* timer;
private:
    // 主要阴影
    QGraphicsDropShadowEffect *mainShadow;
private:
    // 快捷键对象列表
	std::vector<TianLiQtCommon_HookKeyBoard*> hook_key_board_list;
// 鼠标拖动
private:
    QPoint m_Press;
    QPoint m_Move;
    bool leftBtnClk = false;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
private:
    //GenshinImpact_TianLi_Core *core;
// 加载数据库
private:
	void loadDataBase();
	// 更新可选地区数据
    void updata_Country();
    // 更新可选类型数据
    void updata_TypeList();
	// 更新可选种类数据
    void updata_ItemList();
	// 更新可选物品数据
    void updata_ItemsList();
    // 字符串列表 可选地区
    QStringList strList_Addr;
    // 字符串列表 可选类型
    QStringList strList_Type;
    // 字符串列表 可选种类
    QStringList strList_Item;
	// 字符串列表 可选物品
    QStringList strList_Items;
	// 选中地区
	QString selectedStr_Addr;
	// 选中类型
	QString selectedStr_Type;
	// 选中种类
	QString selectedStr_Item;
	// 选中物品
	QString selectedStr_Items;
	// 地区按钮QMap
    QMap<QString, QPushButton*> pushButtonMap_Addr;
	// 类型按钮QMap
    QMap<QString, QPushButton*> pushButtonMap_Type;
	// 种类按钮QMap
    QMap<QString, QPushButton*> pushButtonMap_Item;
	// 物品按钮QMap
    QMap<QString, QPushButton*> pushButtonMap_Items;


    //QVector<QString>  

// 为四个页面添加UI
private:
	// 地图页左侧左半边卡片区域
    QVector<TianLiQtCommon_CardRect*> PageTabMap_LeftCardRects;
	// 地图页左侧右半边卡片区域
    QVector<TianLiQtCommon_CardRect*> PageTabMap_RightCardRects;
	// 地图页左侧右半边滑块卡片区域
    QVector<TianLiQtCommon_ScrollCardRect*> PageTabMap_ScrollCardRect;
	// 地图页右侧地图区域
    QVector<TianLiQtCommon_MapRect*> PageTabMap_MapRect;
	// HUD页卡片区域
    QVector<TianLiQtCommon_CardRect*> PageTabHUD_CardRects;

    // 按钮相关
    QVector<QPushButton*> PageTabMap_RightCard_Buttons;
	QVector<QPushButton*> PageTabMap_ScrollCard_Buttons;
    void addUI_Tab_Map();
	void addUI_Tab_HUD();
	void addUI_Tab_3();
	void addUI_Tab_4();

// 添加 UI 部分
private:
	void loadUIBase();
	// 添加地图页的卡片区域
    void addUI_MapTabCardRects();
    // 添加地图页的大地图
    void addUI_MapTabMapRect();
	// 添加界面页的卡片区域
	void addUI_HUDTabCardRects();

// UI 的直接对应槽函数
private slots:
    void pushButton_Tab_1_clicked(bool checked = false);
    void pushButton_Tab_2_clicked(bool checked = false);
    void pushButton_Tab_3_clicked(bool checked = false);
    void pushButton_Tab_4_clicked(bool checked = false);
    void ui_updatePusButtonList();
    void ui_updataCountryButtonList();
	void ui_updataTypeButtonList();
    void ui_updataItemButtonList();
    void ui_updataItemsButtonList();
private:
	QWidget* main_bebind_widget=nullptr;
	bool is_visible = false;
private slots:
    void slot_show_or_hide();
    void slot_auto_track();
private:
	TianLiQtCommon_HUD_SquareMap* hud_square_map = nullptr;
	TianLiQtCommon_HUD_CircularMap* hud_circular_map = nullptr;
	TianLiQtCommon_HUD_AzimuthBarWindow* hud_azimuth_bar_window = nullptr;
private:
    void slot_show();
    void slot_hide();
	
// UI 的间接操作函数，方便成组处理
private:
    void setCurrentIndex_MainTabPages(int index = 0);

#pragma region Temp
private slots:
    void pushButtonGroup_SelectCountry(bool checked = false);
    void pushButtonGroup_SelectType(bool checked = false);
	void pushButtonGroup_SelectItem(bool checked = false);
    void pushButtonGroup_SelectItems(bool checked = false);
#pragma endregion

};
