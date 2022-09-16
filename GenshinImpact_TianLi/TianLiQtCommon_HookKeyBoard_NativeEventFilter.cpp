#include "TianLiQtCommon_HookKeyBoard_NativeEventFilter.h"

TianLiQtCommon_HookKeyBoard_NativeEventFilter::TianLiQtCommon_HookKeyBoard_NativeEventFilter(TianLiQtCommon_HookKeyBoard* keyboard_object):hook_keyboard_object(keyboard_object)
{
}

TianLiQtCommon_HookKeyBoard_NativeEventFilter::~TianLiQtCommon_HookKeyBoard_NativeEventFilter()
{
}

bool TianLiQtCommon_HookKeyBoard_NativeEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
{
	if (eventType == "windows_generic_MSG")
	{
		MSG* msg = static_cast<MSG*>(message);
		if (msg->message == WM_HOTKEY)
		{
			const quint32 keycode = HIWORD(msg->lParam);
			const quint32 modifiers = LOWORD(msg->lParam);
			bool res = hook_keyboard_object->hook_keyboard_hash.value(qMakePair(keycode, modifiers));
			if (res)
			{
				hook_keyboard_object->ActivateHookKeyBoard();
				return false;
			}
		}

	}
	return false;
}
