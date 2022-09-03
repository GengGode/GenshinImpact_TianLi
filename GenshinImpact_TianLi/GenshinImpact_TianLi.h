#pragma once

#include <QGraphicsDropShadowEffect>

#include <QtWidgets/QMainWindow>
#include "ui_GenshinImpact_TianLi.h"

class TianLiQtCommon_CardRect;
class TianLiQtCommon_ScrollCardRect;
class TianLiQtCommon_MapRect;
class GenshinImpact_TianLi_Sqlite;
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

// ����϶�
private:
    QPoint m_Press;
    QPoint m_Move;
    bool leftBtnClk = false;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
private:
    //GenshinImpact_TianLi_Core *core;
// �������ݿ�
private:
    GenshinImpact_TianLi_Sqlite* TianLi_Sqlite;
	void loadDataBase();
	// ���¿�ѡ��������
    void updata_Country();
    // ���¿�ѡ��������
    void updata_TypeList();
	// ���¿�ѡ��������
    void updata_ItemList();
	// ���¿�ѡ��Ʒ����
    void updata_ItemsList();
    // �ַ����б� ��ѡ����
    QStringList strList_Addr;
    // �ַ����б� ��ѡ����
    QStringList strList_Type;
    // �ַ����б� ��ѡ����
    QStringList strList_Item;
	// �ַ����б� ��ѡ��Ʒ
    QStringList strList_Items;
	// ѡ�е���
	QString selectedStr_Addr;
	// ѡ������
	QString selectedStr_Type;
	// ѡ������
	QString selectedStr_Item;
	// ѡ����Ʒ
	QString selectedStr_Items;
	// ������ťQMap
    QMap<QString, QPushButton*> pushButtonMap_Addr;
	// ���Ͱ�ťQMap
    QMap<QString, QPushButton*> pushButtonMap_Type;
	// ���ఴťQMap
    QMap<QString, QPushButton*> pushButtonMap_Item;
	// ��Ʒ��ťQMap
    QMap<QString, QPushButton*> pushButtonMap_Items;


    //QVector<QString>  

// Ϊ�ĸ�ҳ�����UI
private:
	// ��ͼҳ������߿�Ƭ����
    QVector<TianLiQtCommon_CardRect*> PageTabMap_LeftCardRects;
	// ��ͼҳ����Ұ�߿�Ƭ����
    QVector<TianLiQtCommon_CardRect*> PageTabMap_RightCardRects;
	// ��ͼҳ����Ұ�߻��鿨Ƭ����
    QVector<TianLiQtCommon_ScrollCardRect*> PageTabMap_ScrollCardRect;
	// ��ͼҳ�Ҳ��ͼ����
    QVector<TianLiQtCommon_MapRect*> PageTabMap_MapRect;
	// HUDҳ��Ƭ����
    QVector<TianLiQtCommon_CardRect*> PageTabHUD_CardRects;

    void addUI_Tab_Map();
	void addUI_Tab_HUD();
	void addUI_Tab_3();
	void addUI_Tab_4();

// ��� UI ����
private:
	void loadUIBase();
	// ��ӵ�ͼҳ�Ŀ�Ƭ����
    void addUI_MapTabCardRects();
    // ��ӵ�ͼҳ�Ĵ��ͼ
    void addUI_MapTabMapRect();
	// ��ӽ���ҳ�Ŀ�Ƭ����
	void addUI_HUDTabCardRects();

// UI ��ֱ�Ӷ�Ӧ�ۺ���
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
	
// UI �ļ�Ӳ���������������鴦��
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
