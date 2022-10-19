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
	HHOOK handle_hook_keyboard;
	LRESULT CALLBACK keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam);
};
