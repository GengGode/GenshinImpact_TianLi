#pragma once

#include <QObject>
#include <QMap>

#include <Windows.h>

class TianLiQtCommon_ListenKeyBoard  : public QObject
{
	Q_OBJECT

public:
	TianLiQtCommon_ListenKeyBoard(QObject *parent);
	~TianLiQtCommon_ListenKeyBoard();
public:
	QMap<int, std::function<void(bool)>> key_slot_map;
	QMap<int, std::pair<bool,std::function<void(bool)>>> key_signal_map;
private:
	HHOOK handle_hook_keyboard;
	LRESULT CALLBACK keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam);
};
