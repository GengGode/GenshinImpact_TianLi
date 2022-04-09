#pragma once

#include <QWidget>
#include "ui_TianLiQtCommon_MapRect.h"

class TianLiQtCommon_MapRect : public QWidget
{
	Q_OBJECT

public:
	TianLiQtCommon_MapRect(QWidget *parent = Q_NULLPTR);
	~TianLiQtCommon_MapRect();

private:
	Ui::TianLiQtCommon_MapRect ui;

private:
	int radius = 32;

	//  Û±ÍÕœ∂Ø
private:
	QPoint m_Press;
	QPoint m_Move;
	bool leftBtnClk = false;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private:
	void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
};
