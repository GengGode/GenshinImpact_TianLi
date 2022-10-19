#include "TianLiQtCommon_ListenKeyBoard.h"

#include "TianLiQtCommon_Logger.h"

#include <functional>
namespace Callbacks
{
	template <typename T>
	struct Callback;

	template <typename Ret, typename... Params>
	struct Callback<Ret(Params...)>
	{
		template <typename... Args>
		static Ret callback(Args... args) {
			return func(args...);
		}
		static std::function<Ret(Params...)> func;
	};

	template <typename Ret, typename... Params>
	std::function<Ret(Params...)> Callback<Ret(Params...)>::func;
}

TianLiQtCommon_ListenKeyBoard::TianLiQtCommon_ListenKeyBoard(QObject *parent)
	: QObject(parent)
{
	Callbacks::Callback<LRESULT(int, WPARAM, LPARAM)>::func = std::bind(&TianLiQtCommon_ListenKeyBoard::keyboard_proc, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	HOOKPROC f_ptr = static_cast<HOOKPROC>(Callbacks::Callback<LRESULT(int, WPARAM, LPARAM)>::callback);

	handle_hook_keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, f_ptr, NULL, NULL);
	
	if (handle_hook_keyboard == NULL)
	{
		LogWarn("SetWindowsHookEx failed!");
	}
	else
	{
		LogInfo("SetWindowsHookEx success!");
	}
	
#define TEST
#ifdef TEST
	//test
	key_signal_map.insert(0x41, { false,[](bool is_down) {
		if (is_down)
		{
			LogInfo("A is down");
		}
		else
		{
			LogInfo("A is up");
		}
		} });
#endif // TEST
}

TianLiQtCommon_ListenKeyBoard::~TianLiQtCommon_ListenKeyBoard()
{
	UnhookWindowsHookEx(handle_hook_keyboard);
}

LRESULT CALLBACK TianLiQtCommon_ListenKeyBoard::keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION) 
	{
		switch (wParam) {
		case WM_KEYDOWN: {
			LogInfo(QString::fromStdString(std::format("WM_KEYDOWN: {}\n", p->vkCode)));
			if (key_signal_map.contains(p->vkCode))
			{
				if (key_signal_map[p->vkCode].first == false)
				{
					key_signal_map[p->vkCode].second(true);
					key_signal_map[p->vkCode].first = true;
				}
			}
		} break;
		case WM_SYSKEYDOWN: {
			LogInfo(QString::fromStdString(std::format("WM_SYSKEYDOWN: {}\n", p->vkCode)));
			
		} break;
		case WM_KEYUP: {
			LogInfo(QString::fromStdString(std::format("WM_KEYUP: {}\n", p->vkCode)));
			if (key_signal_map.contains(p->vkCode))
			{
					key_signal_map[p->vkCode].second(false);
					key_signal_map[p->vkCode].first = false;
			}
		} break;
		case WM_SYSKEYUP: {
			LogInfo(QString::fromStdString(std::format("WM_SYSKEYUP: {}\n", p->vkCode)));
			
		} break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
