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
}

TianLiQtCommon_ListenKeyBoard::~TianLiQtCommon_ListenKeyBoard()
{
	UnhookWindowsHookEx(handle_hook_keyboard);
}

void TianLiQtCommon_ListenKeyBoard::register_key_signal(int key, std::function<void(bool)> func, bool is_intercept)
{
	if (key_signal_map.contains(key))
	{
		if (key_signal_map[key].first.first)
		{
			key_signal_map[key].second(false);
			key_signal_map[key].first.first = false;
			key_signal_map[key].first.second = is_intercept;
			key_signal_map[key].second = func;
		}
	}
	else
	{
		key_signal_map.insert(key, { {false, is_intercept}, func});
	}
	
}

void TianLiQtCommon_ListenKeyBoard::register_key(int key, std::function<void()> func, bool is_intercept)
{
	register_key_signal(key, [func](bool is_press) {
		if (is_press)
		{
			LogTrace("key press");
			func();
		}
		}, is_intercept);
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
				if (key_signal_map[p->vkCode].first.first == false)
				{
					key_signal_map[p->vkCode].second(true);
					key_signal_map[p->vkCode].first.first = true;
					if (key_signal_map[p->vkCode].first.second)
						return 1;
				}
			}
		} break;
		case WM_SYSKEYDOWN: {
			LogInfo(QString::fromStdString(std::format("WM_SYSKEYDOWN: {}\n", p->vkCode)));
			key_system = p->vkCode;
			if (key_system == 164 || key_system == 165)
			{
				key_alt = true;
			}
			else if (key_system == 162 || key_system == 163)
			{
				key_ctrl = true;
			}
			else if (key_system == 160 || key_system == 161)
			{
				key_shift = true;
			}
			else if (key_system == 91 || key_system == 92)
			{
				key_win = true;
			}
			
		} break;
		case WM_KEYUP: {
			//LogInfo(QString::fromStdString(std::format("WM_KEYUP: {}\n", p->vkCode)));
			if (key_signal_map.contains(p->vkCode))
			{
				if (key_signal_map[p->vkCode].first.first == true)
				{
					key_signal_map[p->vkCode].second(false);
					key_signal_map[p->vkCode].first.first = false;
					if (key_signal_map[p->vkCode].first.second)
						return 1;
				}
			}
		} break;
		case WM_SYSKEYUP: {
			key_system = -1;
			if (p->vkCode == 164 || p->vkCode == 165)
			{
				key_alt = false;
			}
			else if (p->vkCode == 162 || p->vkCode == 163)
			{
				key_ctrl = false;
			}
			else if (p->vkCode == 160 || p->vkCode == 161)
			{
				key_shift = false;
			}
			else if (p->vkCode == 91 || p->vkCode == 92)
			{
				key_win = false;
			}
		} break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
