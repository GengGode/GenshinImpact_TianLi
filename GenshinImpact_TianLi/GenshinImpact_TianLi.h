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
    // ��Ҫ��Ӱ
    QGraphicsDropShadowEffect *mainShadow;
private:
    // ��ݼ������б�
	std::vector<TianLiQtCommon_HookKeyBoard*> hook_key_board_list;
// ����϶�
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
	
// Ϊ�ĸ�ҳ�����UI
private:
	// ��ͼҳ������߿�Ƭ����
    QVector<TianLiQtCommon_CardRect*> PageTabMap_LeftCardRects;
	// ��ͼҳ����Ұ�߿�Ƭ����
    QVector<TianLiQtCommon_CardRect*> PageTabMap_RightCardRects;
	// ��ͼҳ����Ұ�߻��鿨Ƭ����
    QVector<TianLiQtCommon_ScrollCardRect*> PageTabMap_ScrollCardRect;
	// ��ͼҳ�Ҳ��ͼ����
    TianLiQtCommon_MapRect* PageTabMap_MapRect;
	// HUDҳ��Ƭ����
    QVector<TianLiQtCommon_CardRect*> PageTabHUD_CardRects;

    // ��ť���
    QVector<QPushButton*> PageTabMap_RightCard_Buttons;
	QVector<QPushButton*> PageTabMap_ScrollCard_Buttons;
    void addUI_Tab_Map();
	void addUI_Tab_HUD();
	void addUI_Tab_3();
    void addUI_Tab_4();
    void addUI_Tab_Set();

// ��� UI ����
private:
	// ��ӵ�ͼҳ�Ŀ�Ƭ����
    void addUI_MapTabCardRects();
    // ��ӵ�ͼҳ�Ĵ��ͼ
    void addUI_MapTabMapRect();
	// ��ӽ���ҳ�Ŀ�Ƭ����
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
