#pragma once
#include <Windows.h>
#include <QApplication>
#include <QAbstractNativeEventFilter>
#include "TianLiQtCommon_HookKeyBoard.h"

class TianLiQtCommon_HookKeyBoard;
class TianLiQtCommon_HookKeyBoard_NativeEventFilter : public QAbstractNativeEventFilter
{
public:
	TianLiQtCommon_HookKeyBoard_NativeEventFilter(TianLiQtCommon_HookKeyBoard* keyboard_object);
	~TianLiQtCommon_HookKeyBoard_NativeEventFilter();
public:
	virtual bool nativeEventFilter(const QByteArray& eventType, void* message, long* result);
private:
	TianLiQtCommon_HookKeyBoard* hook_keyboard_object;
};
