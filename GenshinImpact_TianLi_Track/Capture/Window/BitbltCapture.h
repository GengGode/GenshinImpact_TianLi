#pragma once
#include "../Capture.h"
class BitbltCapture : public Capture
{
public:
	BitbltCapture();
	~BitbltCapture();

	bool init() override;
	bool uninit() override;
	bool capture(cv::Mat& frame) override;
	bool setHandle(HWND handle = 0) override;
private:
	HWND giHandle = nullptr;
	RECT giRect = { 0,0,0,0 };
	RECT giClientRect = { 0,0,0,0 };
	cv::Size giClientSize;
	cv::Mat giFrame;
};


BitbltCapture::BitbltCapture()
{
	type = TianLi::Track::CaptureType::Bitblt;
}

BitbltCapture::~BitbltCapture()
{
}

bool BitbltCapture::init()
{
	return true;
}

bool BitbltCapture::uninit()
{
	return true;
}

bool BitbltCapture::capture(cv::Mat& frame)
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	if (giHandle == NULL)
	{
		err = { 12, "窗口句柄失效" };
		return false;
	}
	if (!IsWindow(giHandle))
	{
		err = { 11, "无效句柄或指定句柄所指向窗口不存在" };
		return false;
	}
	if (!GetWindowRect(giHandle, &giRect))
	{
		err = { 11, "无效句柄或指定句柄所指向窗口不存在" };
		return false;
	}
	if (!GetClientRect(giHandle, &giClientRect))
	{
		err = { 11, "无效句柄或指定句柄所指向窗口不存在" };
		return false;
	}

	//获取屏幕缩放比例
	HWND hWnd = GetDesktopWindow();
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	// 获取监视器逻辑宽度与高度
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
	//int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

	// 获取监视器物理宽度与高度
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;
	//int cyPhysical = dm.dmPelsHeight;

	double horzScale = ((double)cxPhysical / (double)cxLogical);
	double screen_scale = horzScale;

	giClientSize.width = (int)(screen_scale * (giClientRect.right - giClientRect.left));
	giClientSize.height = (int)(screen_scale * (giClientRect.bottom - giClientRect.top));

	//获取目标句柄的窗口大小RECT
	GetWindowRect(giHandle, &giRect);/* 对原神窗口的操作 */

	//获取目标句柄的DC
	HDC hScreen = GetDC(giHandle);/* 对原神窗口的操作 */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//获取目标句柄的宽度和高度
	int	nWidth = (int)((screen_scale) * (giRect.right - giRect.left));
	int	nHeight = (int)((screen_scale) * (giRect.bottom - giRect.top));

	//创建Bitmap对象
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//得到位图

	SelectObject(hCompDC, hBmp); //不写就全黑

	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	////释放对象
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//类型转换
	//这里获取位图的大小信息,事实上也是兼容DC绘图输出的范围
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	//int depth = bmp.bmBitsPixel == 1 ? 1 : 8;

	//mat操作
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

	giFrame = giFrame(cv::Rect(giClientRect.left, giClientRect.top, giClientSize.width, giClientSize.height));


	if (giFrame.empty())
	{
		err = { 3, "窗口画面为空" };
		return false;
	}

	if (giFrame.cols < 480 || giFrame.rows < 360)
	{
		err = { 14, "窗口画面大小小于480x360，无法使用" };
		return false;
	}
	frame = giFrame;
	return true;
}

bool BitbltCapture::setHandle(HWND handle)
{
	if (giHandle != handle)
	{
		uninit();
		giHandle = handle;
		init();
	}
	return true;
}
