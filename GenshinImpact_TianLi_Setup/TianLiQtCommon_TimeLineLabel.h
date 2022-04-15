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
	// һЩ���� ����

	// �Ƿ�ѡ��
	bool m_isChecked = false;
	// �Ƿ񼤻�
	bool m_isAction = false;
	// �Ƿ��ǵ�һ��
	bool m_isBegin = false;
	// �Ƿ������һ��
	bool m_isEnd = false;
	// �Ƕ�
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
	// һЩ���� ��ͼ��
	float m_c_Width = 21;
	float m_c_Height = 31;
	float m_c_MiniD = 7;
	float m_c_D = 13;
	float m_c_Magin = 5.0;
	float m_Angle = 0;

};
