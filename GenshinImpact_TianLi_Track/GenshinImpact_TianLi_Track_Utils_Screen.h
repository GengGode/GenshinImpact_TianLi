#pragma once

#include "GenshinImpact_TianLi_Track_Utils_Struct.h"

inline void get_genshin_screen(const GenshinHandle& genshin_handle, GenshinScreen& out_genshin_screen)
{
	static HBITMAP hBmp;

	auto& giHandle = genshin_handle.handle;
	auto& giRect = genshin_handle.rect;
	auto& giRectClient = genshin_handle.rect_client;
	auto& giScale = genshin_handle.scale;
	auto& giFrame = out_genshin_screen.img_screen;

#ifdef TEST_LOCAL
	static cv::Mat img = cv::imread("C:\\Users\\GengG\\source\\repos\\Cv测试\\OpencvConsole\\img3.png", -1);
	giFrame = img;
#else
	if (genshin_handle.handle == NULL)
		return;

	//获取目标句柄的DC
	HDC hScreen = GetDC(giHandle);/* 对原神窗口的操作 */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//获取目标句柄的宽度和高度
	int	nWidth = static_cast<int>(std::round((giRectClient.right - giRectClient.left)));
	int	nHeight = static_cast<int>(std::round((giRectClient.bottom - giRectClient.top)));

	DeleteObject(hBmp);

	//创建Bitmap对象
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//得到位图

	SelectObject(hCompDC, hBmp);
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//释放对象
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	BITMAP bmp;

	//类型转换
	GetObject(hBmp, sizeof(BITMAP), &bmp);
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? 1 : 8;

	//mat操作

	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

	if (giFrame.channels() == 3)
	{
		cvtColor(giFrame, giFrame, cv::COLOR_RGB2RGBA);
	}

	cv::resize(giFrame, giFrame, genshin_handle.size_frame);

#endif // TEST_LOCAL


	{
		if (giFrame.empty())return;
		
		out_genshin_screen.rect_client = cv::Rect(giRect.left, giRect.top, giRectClient.right - giRectClient.left, giRectClient.bottom - giRectClient.top);
		
		// 获取maybe区域
		out_genshin_screen.img_paimon_maybe = giFrame(genshin_handle.rect_paimon_maybe);
		out_genshin_screen.img_minimap_cailb_maybe = giFrame(genshin_handle.rect_minimap_cailb_maybe);
		out_genshin_screen.img_minimap_maybe = giFrame(genshin_handle.rect_minimap_maybe);
		out_genshin_screen.img_uid_maybe = giFrame(genshin_handle.rect_uid_maybe);
		out_genshin_screen.img_left_give_items_maybe = giFrame(genshin_handle.rect_left_give_items_maybe);
		out_genshin_screen.img_right_pick_items_maybe = giFrame(genshin_handle.rect_right_pick_items_maybe);

		out_genshin_screen.config.rect_paimon_maybe = genshin_handle.rect_paimon_maybe;
		out_genshin_screen.config.rect_minimap_cailb_maybe = genshin_handle.rect_minimap_cailb_maybe;
		out_genshin_screen.config.rect_minimap_maybe = genshin_handle.rect_minimap_maybe;


		out_genshin_screen.img_uid = giFrame(genshin_handle.rect_uid);

	}
}
