#pragma once

#include <QWidget>
#include "ui_TianLiQtCommon_HUD_AzimuthBarWindow.h"

#include <map>
#include <tuple>
#include <windows.h>


class TianLiQtCommon_HUD_AzimuthBarWindow : public QWidget
{
	Q_OBJECT

public:
	TianLiQtCommon_HUD_AzimuthBarWindow(QWidget *parent = Q_NULLPTR);
	~TianLiQtCommon_HUD_AzimuthBarWindow();

private:
	Ui::TianLiQtCommon_HUD_AzimuthBarWindow ui;

// UI÷ÿªÊ
private:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

	// void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
public:
	double avatar_rotate_deg_atom = 0;
	enum AzimuthObjectType
	{
		Star,
		Box,
		Item,
		Work
	};
	struct AzimuthObjectInfo
	{
		QString msg;
		QImage image;
	};
	struct AzimuthObject
	{
		double x;
		double y;
		double a;
		QImage image;
		QString name;
		AzimuthObjectType type;
		AzimuthObjectInfo info;
	};
	QTimer* timer=nullptr;
	std::map<int, AzimuthObject> object_map;
public slots:
	void slot_update();
public:
	void slot_update_avatar_rotate(double avatar_rotate);
	void slot_update_move(RECT gi_client_rect);
signals:
	void signal_close_finished();
};
