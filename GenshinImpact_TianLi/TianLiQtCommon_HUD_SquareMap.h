#pragma once

#include <QWidget>
#include "ui_TianLiQtCommon_HUD_SquareMap.h"

class TianLiQtCommon_HUD_SquareMap : public QWidget
{
	Q_OBJECT

public:
	TianLiQtCommon_HUD_SquareMap(QWidget *parent = Q_NULLPTR);
	~TianLiQtCommon_HUD_SquareMap();

private:
	Ui::TianLiQtCommon_HUD_SquareMap ui;

// UI÷ÿªÊ
private:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

	// void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
public slots:
	void slot_update();
signals:
	void signal_close_finished();
	
};

