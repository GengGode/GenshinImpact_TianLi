#include "TianLiQtCommon_ListenKeyBoard.h"


TianLiQtCommon_ListenKeyBoard::TianLiQtCommon_ListenKeyBoard(QObject *parent)
	: QObject(parent)
{}

TianLiQtCommon_ListenKeyBoard::~TianLiQtCommon_ListenKeyBoard()
{}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION) {
		switch (wParam) {
			// º¸≈Ãœ‡πÿ
		case WM_KEYDOWN: {
			//std::cout << std::format("WM_KEYDOWN: {}\n", p->vkCode);
		} break;
		case WM_SYSKEYDOWN: {
			//std::cout << std::format("WM_SYSKEYDOWN: {}\n", p->vkCode);
		} break;
		case WM_KEYUP: {
			//std::cout << std::format("WM_KEYUP: {}\n", p->vkCode);
		} break;
		case WM_SYSKEYUP: {
			//std::cout << std::format("WM_SYSKEYUP: {}\n", p->vkCode);
		} break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

#include <functional>

void TianLiQtCommon_ListenKeyBoard::work()
{
	std::function<LRESULT(int code, WPARAM wParam, LPARAM lParam)> f_proc = std::bind(&TianLiQtCommon_ListenKeyBoard::keyboard_proc, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	static auto f_ptr = *reinterpret_cast<void**>(&f_proc);
	
	//static auto f_ptr = *f_proc.target<HOOKPROC>();
	
	HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)f_ptr, NULL, 0);

	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hHook);
}
#include <QDebug>
LRESULT CALLBACK TianLiQtCommon_ListenKeyBoard::keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
	qDebug() << "TianLiQtCommon_ListenKeyBoard::keyboard_proc";
	
	return LRESULT();
}
