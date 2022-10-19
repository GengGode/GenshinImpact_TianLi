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
	void register_key_signal(int key, std::function<void(bool)> func, bool is_intercept = false);
	void register_key(int key, std::function<void()> func, bool is_intercept = false);

	template<typename T>
	void register_key_signal(int key, typename T* reciver, void(T::* slot)(bool), bool is_intercept = false)
	{
		register_key_signal(key, std::bind(slot, reciver, std::placeholders::_1), is_intercept);
	}
	
	template<typename T>
	void register_key(int key, typename T* reciver, void(T::* slot)(), bool is_intercept = false)
	{
		register_key(key, std::bind(slot, reciver), is_intercept);
	}

private:
	// key: <<is_intercept, is_running>, func>
	QMap<int, std::pair<std::pair<bool, bool>, std::function<void(bool)>>> key_signal_map;
	HHOOK handle_hook_keyboard;
	LRESULT CALLBACK keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam);
};
