#pragma once

#include <QWidget>
#include "ui_TianLiQtCommon_TimeLineLabel.h"

class TianLiQtCommon_TimeLineLabel : public QWidget
{
	Q_OBJECT

public:
	TianLiQtCommon_TimeLineLabel(QWidget *parent = Q_NULLPTR);
	~TianLiQtCommon_TimeLineLabel();

private:
	Ui::TianLiQtCommon_TimeLineLabel ui;

private:
	void paintEvent(QPaintEvent* event);

private:
	void UI_NoCheck();
	void UI_Checked();

private:
	QTimer *timer;
private:
	// 一些参数 属性

	// 是否被选中
	bool m_isChecked = false;
	// 是否激活
	bool m_isAction = false;
	// 是否是第一个
	bool m_isBegin = false;
	// 是否是最后一个
	bool m_isEnd = false;
	// 角度
	int alpha = 0;
public:
	void setAction(bool value);
	void setChecked(bool value);
	void setBegin(bool value);
	void setEnd(bool value);
	bool isAction();
	bool isChecked();
private slots:
	void timeout();
signals:
	void checkChange();
private:
	// 一些参数 绘图用
	float m_c_Width = 21;
	float m_c_Height = 31;
	float m_c_MiniD = 7;
	float m_c_D = 13;
	float m_c_Magin = 5.0;
	float m_Angle = 0;

};
