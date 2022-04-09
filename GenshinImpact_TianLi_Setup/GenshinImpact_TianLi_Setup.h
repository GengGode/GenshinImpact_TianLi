#pragma once

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

private:
    QPoint m_Press;
    QPoint m_Move;
    bool leftBtnClk = false;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;
};
