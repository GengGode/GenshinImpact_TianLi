#pragma once
#include <Windows.h>
#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")

#include "GenshinImpact_TianLi_Track_Utils_Struct.h"

inline void get_genshin_handle(GenshinHandle& genshin_handle)
{
	if (genshin_handle.config.is_auto_find_genshin)
	{
		//genshin_handle.handle = FindWindowA(NULL, "窗口位置调整");
		
		auto& giHandle = genshin_handle.handle;
		LPCWSTR giWindowName = { L"原神" };
		/* 对原神窗口的操作 */
		giHandle = FindWindowA("UnityWndClass", "原神");
		if (giHandle == NULL)
		{
			giWindowName = L"Genshin Impact";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：原神 */
		}
		if (giHandle == NULL)
		{
			giWindowName = L"원신";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：원신 */
		}
		if (giHandle == NULL)
		{
			giWindowName = L"\u539F\u795E";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：原神 */
		}
		if (giHandle == NULL)
		{
			giWindowName = L"\uC6D0\uC2E0";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：원신 */
		}
		
	}
	else
	{
		genshin_handle.handle = genshin_handle.config.genshin_handle;
	}
	if (genshin_handle.handle != 0)
	{
		genshin_handle.is_exist = true;
	}
	else
	{
		genshin_handle.is_exist = false;
#ifdef TEST_LOCAL
#else
		return;

#endif
	}
	// 判断窗口是否存在标题栏
	if (GetWindowLong(genshin_handle.handle, GWL_STYLE) & WS_CAPTION)
	{
		genshin_handle.is_exist_title_bar = true;
	}
	else
	{
		genshin_handle.is_exist_title_bar = false;
	}
	// 获取窗口大小
	GetWindowRect(genshin_handle.handle, &genshin_handle.rect);
	// 获取除标题栏区域大小
	GetClientRect(genshin_handle.handle, &genshin_handle.rect_client);
	// 获取缩放比例
	HMONITOR hMonitor = MonitorFromWindow(genshin_handle.handle, MONITOR_DEFAULTTONEAREST);
	UINT dpiX, dpiY;
	GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
	genshin_handle.scale = dpiX / 96.0;

	{
#ifdef TEST_LOCAL
		static cv::Mat img = cv::imread("C:\\Users\\GengG\\source\\repos\\Cv测试\\OpencvConsole\\img3.png", -1);
		int x = img.cols;
		int y = img.rows;
#else
		int x = genshin_handle.rect_client.right - genshin_handle.rect_client.left;
		int y = genshin_handle.rect_client.bottom - genshin_handle.rect_client.top;
#endif
		double f = 1, fx = 1, fy = 1;

		if (static_cast<double>(x) / static_cast<double>(y) == 16.0 / 9.0)
		{
				genshin_handle.size_frame=cv::Size(1920, 1080);
		}
		else if (static_cast<double>(x) / static_cast<double>(y) > 16.0 / 9.0)
		{

			//高型，以宽为比例

			// x = (y * 16) / 9;
			f = y / 1080.0;
			//将giFrame缩放到1920*1080的比例
			fx = x / f;
			// 将图片缩放
			genshin_handle.size_frame = cv::Size(static_cast<int>(fx), 1080);

		}
		else if (static_cast<double>(x) / static_cast<double>(y) < 16.0 / 9.0)
		{

			//宽型，以高为比例

			// x = (y * 16) / 9;
			f = x / 1920.0;
			//将giFrame缩放到1920*1080的比例
			fy = y / f;
			// 将图片缩放
			genshin_handle.size_frame = cv::Size(1920, static_cast<int>(fy));
		}
		x = genshin_handle.size_frame.width;
		y = genshin_handle.size_frame.height;
		// 派蒙可能性区域计算参数
		int paimon_mayArea_left = 0;
		int paimon_mayArea_top = 0;
		int paimon_mayArea_width = static_cast<int>(x * 0.10);
		int paimon_mayArea_height = static_cast<int>(y * 0.10);
		// 派蒙可能性区域
		cv::Rect Area_Paimon_mayArea(
			paimon_mayArea_left,
			paimon_mayArea_top,
			paimon_mayArea_width,
			paimon_mayArea_height);
		genshin_handle.rect_paimon_maybe = Area_Paimon_mayArea;
		
		// 小地图标定可能性区域计算参数
		int miniMap_Cailb_mayArea_left = static_cast<int>(x * 0.10);
		int miniMap_Cailb_mayArea_top =0;
		int miniMap_Cailb_mayArea_width = static_cast<int>(x * 0.10);
		int miniMap_Cailb_mayArea_height = static_cast<int>(y * 0.10);
		// 小地图标定可能性区域
		cv::Rect Area_MiniMap_Cailb_mayArea(
			miniMap_Cailb_mayArea_left,
			miniMap_Cailb_mayArea_top,
			miniMap_Cailb_mayArea_width,
			miniMap_Cailb_mayArea_height);
		genshin_handle.rect_minimap_cailb_maybe = Area_MiniMap_Cailb_mayArea;

		// 小地图可能性区域计算参数
		int miniMap_mayArea_left = 0;
		int miniMap_mayArea_top = 0;
		int miniMap_mayArea_width = static_cast<int>(x * 0.18);
		int miniMap_mayArea_height = static_cast<int>(y * 0.22);
		// 小地图可能性区域
		cv::Rect Area_MiniMap_mayArea(
			miniMap_mayArea_left,
			miniMap_mayArea_top,
			miniMap_mayArea_width,
			miniMap_mayArea_height);
		genshin_handle.rect_minimap_maybe = Area_MiniMap_mayArea;

		// UID可能性区域计算参数
		int UID_mayArea_left = static_cast<int>(x * 0.88);
		int UID_mayArea_top = static_cast<int>(y * 0.97);
		int UID_mayArea_width = x - UID_mayArea_left;
		int UID_mayArea_height = y - UID_mayArea_top;
		// UID可能性区域
		cv::Rect Area_UID_mayArea(
			UID_mayArea_left,
			UID_mayArea_top,
			UID_mayArea_width,
			UID_mayArea_height);
		genshin_handle.rect_uid_maybe = Area_UID_mayArea;
		int UID_Rect_x = cvCeil(x - x * (1.0 - 0.865));
		int UID_Rect_y = cvCeil(y - 1080.0 * (1.0 - 0.9755));
		int UID_Rect_w = cvCeil(1920 * 0.11);
		int UID_Rect_h = cvCeil(1920 * 0.0938 * 0.11);


		genshin_handle.rect_uid=cv::Rect(UID_Rect_x, UID_Rect_y, UID_Rect_w, UID_Rect_h);

		// 左侧已获取物品可能性区域计算参数
		int leftGetItems_mayArea_left   = static_cast<int>(x * 0.050);
		int leftGetItems_mayArea_top    = static_cast<int>(y * 0.460);
		int leftGetItems_mayArea_width  = static_cast<int>(x * 0.160);
		int leftGetItems_mayArea_height = static_cast<int>(y * 0.480);
		// 左侧已获取物品可能性区域
		cv::Rect Area_LeftGetItems_mayArea(
			leftGetItems_mayArea_left,
			leftGetItems_mayArea_top,
			leftGetItems_mayArea_width,
			leftGetItems_mayArea_height);
		genshin_handle.rect_left_give_items_maybe = Area_LeftGetItems_mayArea;
		/*
		auto left_rect(const cv::Mat& frame)
{
	auto rect = cv::Rect(static_cast<int>(frame.cols / 20.0), static_cast<int>(frame.rows / 2.0), static_cast<int>(frame.cols / 6.0), static_cast<int>(frame.rows / 3.0));
	return rect;
}
auto right_rect(const cv::Mat& frame)
{
	auto rect = cv::Rect(static_cast<int>(frame.cols / 5.0 * 3.0), static_cast<int>(frame.rows / 4.0), static_cast<int>(frame.cols / 6.0), static_cast<int>(frame.rows / 2.0));
	return rect;
}
		*/
		// 左侧已获取物品区域计算参数
		int leftGetItems_Area_left    = static_cast<int>(x * 0.050);
		int leftGetItems_Area_top     = static_cast<int>(y * 0.500);
		int leftGetItems_Area_width   = static_cast<int>(x * 0.166);
		int leftGetItems_Area_height  = static_cast<int>(y * 0.333);
		// 左侧已获取物品区域
		cv::Rect Area_LeftGetItems_Rect(
			leftGetItems_Area_left   ,
			leftGetItems_Area_top    ,
			leftGetItems_Area_width  ,
			leftGetItems_Area_height );
		genshin_handle.rect_left_give_items = Area_LeftGetItems_Rect;
		
		
		// 右侧可捡取物品可能性区域计算参数
		int rightGetItems_mayArea_left   = static_cast<int>(x * 0.570);
		int rightGetItems_mayArea_top    = static_cast<int>(y * 0.250);
		int rightGetItems_mayArea_width  = static_cast<int>(x * 0.225);
		int rightGetItems_mayArea_height = static_cast<int>(y * 0.500);
		// 右侧可捡取物品可能性区域
		cv::Rect Area_RightGetItems_mayArea(
			rightGetItems_mayArea_left,
			rightGetItems_mayArea_top,
			rightGetItems_mayArea_width,
			rightGetItems_mayArea_height);
		genshin_handle.rect_right_pick_items_maybe = Area_RightGetItems_mayArea;
		// 右侧可捡取物品区域计算参数
		int rightGetItems_Area_left = static_cast<int>(x * 0.600);
		int rightGetItems_Area_top = static_cast<int>(y * 0.250);
		int rightGetItems_Area_width = static_cast<int>(x * 0.166);
		int rightGetItems_Area_height = static_cast<int>(y * 0.500);
		// 右侧可捡取物品区域
		cv::Rect Area_RightGetItems_Rect(
			rightGetItems_Area_left,
			rightGetItems_Area_top,
			rightGetItems_Area_width,
			rightGetItems_Area_height);
		genshin_handle.rect_right_pick_items = Area_RightGetItems_Rect;
		
	}
}

inline void update_genshin_handle(const HWND& old_handle, GenshinHandle& out_genshin_handle)
{
	static unsigned char tick_count = 0;
	if (IsWindowVisible(old_handle))
	{
		if (tick_count < 30)
		{
			tick_count++;
		}
		else
		{
			tick_count = 0;
			get_genshin_handle(out_genshin_handle);
		}
	}
	else
	{
		get_genshin_handle(out_genshin_handle);
	}
	return;
}
