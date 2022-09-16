#pragma once

#include <QObject>
#include <QHash>
#include <QApplication>
#include <QKeySequence>
#include <Windows.h>
#include "TianLiQtCommon_HookKeyBoard_NativeEventFilter.h"

class TianLiQtCommon_HookKeyBoard_NativeEventFilter;
class TianLiQtCommon_HookKeyBoard : public QObject
{
	Q_OBJECT

public:
	TianLiQtCommon_HookKeyBoard(QString keyboard, QObject *parent);
	~TianLiQtCommon_HookKeyBoard();

public:
	void ActivateHookKeyBoard();
	bool RegisterHookKeyBoard();
	bool UnregisterHookKeyBoard();
	QHash<QPair<quint32, quint32>, TianLiQtCommon_HookKeyBoard*> hook_keyboard_hash;
private:
	QApplication* app;
	TianLiQtCommon_HookKeyBoard_NativeEventFilter* keyboard_event_filter;
	QKeySequence hook_keyboard;
	Qt::Key hook_keyboard_code;
	Qt::KeyboardModifiers hook_keyboard_mods;
	static quint32 native_keyboard_code(Qt::Key hook_keyboard_code);
	static quint32 native_keyboard_modifiers(Qt::KeyboardModifiers modifiers);
signals:
	void signal_activated();
};

