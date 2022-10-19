#include "TianLiQtCommon_ListenKeyBoard.h"

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
}

TianLiQtCommon_ListenKeyBoard::~TianLiQtCommon_ListenKeyBoard()
{
	UnhookWindowsHookEx(handle_hook_keyboard);
}

void TianLiQtCommon_ListenKeyBoard::work()
{
	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
#include <QDebug>
#include <iostream>
LRESULT CALLBACK TianLiQtCommon_ListenKeyBoard::keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
	qDebug() << "TianLiQtCommon_ListenKeyBoard::keyboard_proc";

	return LRESULT();
}
