#pragma once

#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>

#include <QtWidgets/QWidget>
#include "ui_GenshinImpact_TianLi_Setup.h"

class GenshinImpact_TianLi_Setup : public QWidget
{
    Q_OBJECT

public:
    GenshinImpact_TianLi_Setup(QWidget *parent = Q_NULLPTR);

private:
    Ui::GenshinImpact_TianLi_SetupClass ui;

private:
    QGraphicsDropShadowEffect* mainShadow_A;
    QGraphicsDropShadowEffect* mainShadow_B;
    QGraphicsDropShadowEffect* mainShadow;
    QPropertyAnimation* mainShadowAnimation;
    QPropertyAnimation* exitAnimation;
private:
    QPoint m_Press;
    QPoint m_Move;
    bool leftBtnClk = false;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;

private slots:
    void pushButton_UI_Close();
    void pushButton_UI_Mini();

    void pushButton_FastInstall();
    void pushButton_CustomizeInstall();
    void pushButton_Finishing_Cancel();
    void pushButton_Finished_Run();
    void pushButton_Finished_Exit();
};
