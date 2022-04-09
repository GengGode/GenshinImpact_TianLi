#pragma once

#include <QGraphicsDropShadowEffect>

#include <QtWidgets/QMainWindow>
#include "ui_GenshinImpact_TianLi.h"

class GenshinImpact_TianLi : public QMainWindow
{
    Q_OBJECT

public:
    GenshinImpact_TianLi(QWidget *parent = Q_NULLPTR);

private:
    Ui::GenshinImpact_TianLiClass ui;

private:
    // 主要阴影
    QGraphicsDropShadowEffect *mainShadow;

// 鼠标拖动
private:
    QPoint m_Press;
    QPoint m_Move;
    bool leftBtnClk = false;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

// 添加 UI 部分
private:
    void addUI_MapRect();
    void addUI_CardRects();

// UI 的直接对应槽函数
private slots:
    void pushButton_Tab_1_clicked(bool checked = false);
    void pushButton_Tab_2_clicked(bool checked = false);
    void pushButton_Tab_3_clicked(bool checked = false);
    void pushButton_Tab_4_clicked(bool checked = false);

// UI 的间接操作函数，方便成组处理
private:
    void setCurrentIndex_MainTabPages(int index = 0);
};
