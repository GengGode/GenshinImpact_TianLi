#pragma once

#include <QObject>

#include <Windows.h>

class TianLiQtCommon_ListenKeyBoard  : public QObject
{
	Q_OBJECT

public:
	TianLiQtCommon_ListenKeyBoard(QObject *parent);
	~TianLiQtCommon_ListenKeyBoard();
public:
	void work();
private:
	LRESULT keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam);
};
