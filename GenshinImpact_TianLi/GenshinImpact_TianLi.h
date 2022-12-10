#pragma once

#include <QGraphicsDropShadowEffect>
#include <QMap> 
#include <QButtonGroup> 
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

#include <map>

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
	bool close_is_mini = false;
private:
    QButtonGroup area_button_group;
    std::map<std::string, int> area_button_index_map;
    QButtonGroup type_button_group;
    std::map<std::string, int> type_button_index_map;
    QButtonGroup item_button_group;
    std::map<std::string, int> item_button_index_map;
    std::map<std::tuple<std::string,std::string,std::string>,bool> item_button_checked_map;

    QButtonGroup object_button_group;
    std::map<std::string, int> object_button_index_map;

    void init_area_list();
    QString get_selected_area();
    void init_type_list();
    QString get_selected_type();
	
    //void updata_selectable_type(std::string area);
    void updata_selectable_item(std::string area, std::string type);
	
// 为四个页面添加UI
private:
	// 地图页左侧左半边卡片区域
    QVector<TianLiQtCommon_CardRect*> PageTabMap_LeftCardRects;
	// 地图页左侧右半边卡片区域
    QVector<TianLiQtCommon_CardRect*> PageTabMap_RightCardRects;
	// 地图页左侧右半边滑块卡片区域
    QVector<TianLiQtCommon_ScrollCardRect*> PageTabMap_ScrollCardRect;
	// 地图页右侧地图区域
    TianLiQtCommon_MapRect* PageTabMap_MapRect;
	// HUD页卡片区域
    QVector<TianLiQtCommon_CardRect*> PageTabHUD_CardRects;

    // 按钮相关
    QVector<QPushButton*> PageTabMap_RightCard_Buttons;
	QVector<QPushButton*> PageTabMap_ScrollCard_Buttons;
    void addUI_Tab_Map();
	void addUI_Tab_HUD();
	void addUI_Tab_3();
    void addUI_Tab_4();
    void addUI_Tab_Set();

// 添加 UI 部分
private:
	// 添加地图页的卡片区域
    void addUI_MapTabCardRects();
    // 添加地图页的大地图
    void addUI_MapTabMapRect();
	// 添加界面页的卡片区域
	void addUI_HUDTabCardRects();
	
private:
	QWidget* main_bebind_widget=nullptr;
	bool is_visible = false;
private slots:
    void slot_show_or_hide();
    void slot_auto_track();
    void slot_updata_pickable_items(std::vector<std::string> item_tags);
private:
	TianLiQtCommon_HUD_SquareMap* hud_square_map = nullptr;
	TianLiQtCommon_HUD_CircularMap* hud_circular_map = nullptr;
	TianLiQtCommon_HUD_AzimuthBarWindow* hud_azimuth_bar_window = nullptr;
private:
    void slot_show();
    void slot_hide();

#pragma region Temp
private slots:
    void pushButtonGroup_SelectItem(bool checked = false);
    void slot_delete_object();

#pragma endregion

};
