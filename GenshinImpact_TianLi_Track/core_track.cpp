#include "pch.h"
#include "core_track.h"
#include "core_match_thread.h"
/*
void core_track_continuity::setState(bool state)
{
	continuity[0] = continuity[1];
	continuity[1] = continuity[2];
	continuity[2] = state;
}

bool core_track_continuity::getIsContinuity()
{
	return continuity[0] && continuity[1] && continuity[2];
}

core_track::core_track()
{
	core_match_thread<int, cv::Mat> t;
	core_match_thread<int, cv::Mat>::init_input input;
	input.in = 1;
	t.set_init_input(input);
	bool out;
	auto f = [](auto& out) {return 0; };
	t.init(f);
}

core_track::~core_track()
{
	if (tSurfMapInit != nullptr)
	{
		tSurfMapInit->join();
		delete tSurfMapInit;
	}
	if (tSurfMapMatch != nullptr)
	{
		tSurfMapMatch->join();
		delete tSurfMapMatch;
	}
	if (tTemplatePaimonMatch != nullptr)
	{
		tTemplatePaimonMatch->join();
		delete tTemplatePaimonMatch;
	}
	if (tOrbAvatarInit != nullptr)
	{
		tOrbAvatarInit->join();
		delete tOrbAvatarInit;
	}
	if (tOrbAvatarMatch != nullptr)
	{
		tOrbAvatarMatch->join();
		delete tOrbAvatarMatch;
	}
	if (tTemplateUIDInit != nullptr)
	{
		tTemplateUIDInit->join();
		delete tTemplateUIDInit;
	}
	if (tTemplateUIDMatch != nullptr)
	{
		tTemplateUIDMatch->join();
		delete tTemplateUIDMatch;
	}
}
*/

#include <opencv2/imgproc/imgproc_c.h>

TianLi_ThreadMatch::~TianLi_ThreadMatch()
{
	if (tSurfMapInit != nullptr)
	{
		tSurfMapInit->join();
		delete tSurfMapInit;
	}
	if (tSurfMapMatch != nullptr)
	{
		tSurfMapMatch->join();
		delete tSurfMapMatch;
	}
	if (tTemplatePaimonMatch != nullptr)
	{
		tTemplatePaimonMatch->join();
		delete tTemplatePaimonMatch;
	}
	if (tOrbAvatarInit != nullptr)
	{
		tOrbAvatarInit->join();
		delete tOrbAvatarInit;
	}
	if (tOrbAvatarMatch != nullptr)
	{
		tOrbAvatarMatch->join();
		delete tOrbAvatarMatch;
	}
	if (tTemplateUIDInit != nullptr)
	{
		tTemplateUIDInit->join();
		delete tTemplateUIDInit;
	}
	if (tTemplateUIDMatch != nullptr)
	{
		tTemplateUIDMatch->join();
		delete tTemplateUIDMatch;
	}
}

void TianLi_ThreadMatch::cThreadInit()
{
}

void TianLi_ThreadMatch::cThreadSurfMapInit(Mat& Map)
{
	if (tSurfMapInit == nullptr && surfMap.isInit == false)
	{
		cvtColor(Map, mapGray, CV_RGB2GRAY);

		tSurfMapInit = new thread(&TianLi_ThreadMatch::thread_SurfMapInit, this, ref(mapGray));
		tIsEndSurfMapInit = false;
	}
}

void TianLi_ThreadMatch::cThreadSurfMapMatch()
{
	if (tSurfMapMatch == nullptr && tIsEndSurfMapInit && isExistObjMinMap && isPaimonVisial)
	{
		tSurfMapMatch = new thread(&TianLi_ThreadMatch::thread_SurfMapMatch, this, ref(objMinMap));
		tIsEndSurfMapMatch = false;
	}
}

void TianLi_ThreadMatch::setSurfMap(Mat mapMat)
{
	surfMap.setMap(mapMat);
}

void TianLi_ThreadMatch::cThreadTemplatePaimonMatch(Mat& Template)
{
	if (tTemplatePaimonMatch == nullptr && isExistObjPaimon)
	{
		cvtColor(Template, templatePaimon, CV_RGB2GRAY);
		tTemplatePaimonMatch = new thread(&TianLi_ThreadMatch::thread_TemplatePaimonMatch, this, ref(templatePaimon), ref(objPaimon));
		tIsEndTemplatePaimonMatch = false;
	}
}

void TianLi_ThreadMatch::setTemplatePaimon(Mat TemplatePaimonMat)
{
	TemplatePaimonMat.copyTo(templatePaimon);
}

void TianLi_ThreadMatch::setPaimon(Mat PaimonMat)
{
	PaimonMat.copyTo(objPaimon);
}

void TianLi_ThreadMatch::cThreadOrbAvatarInit(Mat& TemplateAvatar)
{
	if (tOrbAvatarInit == nullptr && orbAvatar.isInit == false)
	{
		//templateAvatar = Mat(300, 300, CV_8UC1,Scalar(0,0,0));
		//cvtColor(TemplateAvatar(Rect(TemplateAvatar.cols/2-16, TemplateAvatar.rows/2-16,32,32)), templateAvatar(Rect(134,134,32,32)), CV_RGB2GRAY);
		//threshold(templateAvatar, templateAvatar, 185, 255, THRESH_TOZERO);
		////resize(templateAvatar, templateAvatar, Size(0, 0), 1, 1, 3);//INTER_CUBIC INTER_AREAz
		resize(TemplateAvatar, templateAvatar, Size(150, 150), 0, 0, INTER_LANCZOS4);//INTER_CUBIC INTER_AREAz
		tOrbAvatarInit = new thread(&TianLi_ThreadMatch::thread_OrbAvatarInit, this, ref(templateAvatar));
		tIsEndOrbAvatarInit = false;
	}
}

void TianLi_ThreadMatch::cThreadOrbAvatarMatch()
{
	if (tOrbAvatarMatch == nullptr && tIsEndOrbAvatarInit && isExistObjMinMap && isPaimonVisial)
	{
		//Mat temp = Mat(300, 300, CV_8UC1, Scalar(0, 0, 0));
		//objAvatar.copyTo(temp(Rect(138, 138, 24, 24)));
		//objAvatar = temp;
		//threshold(objAvatar, objAvatar, 185, 255, THRESH_TOZERO);
		////resize(objAvatar, objAvatar, Size(0, 0), 1*1.3, 1*1.3, 3);//INTER_CUBIC INTER_AREAz

		//resize(objAvatar, objAvatar, Size(150, 150), 0, 0, INTER_LANCZOS4);//INTER_CUBIC INTER_AREAz
		tOrbAvatarMatch = new thread(&TianLi_ThreadMatch::thread_OrbAvatarMatch, this, ref(objAvatar));
		tIsEndOrbAvatarMatch = false;
	}
}

void TianLi_ThreadMatch::setAvatat(Mat AvatarMat)
{
	AvatarMat.copyTo(objAvatar);
}

void TianLi_ThreadMatch::cThreadTemplateUIDInit(Mat* TemplateUID)
{
	if (tTemplateUIDInit == nullptr && tempUID.isInit == false)
	{
		templateUID = TemplateUID;
		tTemplateUIDInit = new thread(&TianLi_ThreadMatch::thread_TemplateUIDInit, this, ref(templateUID));
		tIsEndTemplateUIDInit = false;
	}
}

void TianLi_ThreadMatch::cThreadTemplateUIDMatch()
{
	if (tTemplateUIDMatch == nullptr && isExistObjUID)
	{
		tTemplateUIDMatch = new thread(&TianLi_ThreadMatch::thread_TemplateUIDMatch, this, ref(objUID));
		tIsEndTemplateUIDMatch = false;
	}
}

void TianLi_ThreadMatch::setUID(Mat UIDMat)
{
	UIDMat.copyTo(objUID);
}

void TianLi_ThreadMatch::cThreadTemplateStarInit(Mat& TemplateStar)
{
	if (tTemplateStarInit == nullptr && tempStar.isInit == false)
	{
		//templateStar = TemplateStar;
		cvtColor(TemplateStar, templateStar, CV_RGB2GRAY);
		tTemplateStarInit = new thread(&TianLi_ThreadMatch::thread_TemplateStarInit, this, ref(templateStar));
		tIsEndTemplateStarInit = false;
	}
}

void TianLi_ThreadMatch::cThreadTemplateStarMatch()
{
	if (tTemplateStarMatch == nullptr && isExistObjMinMap && isStarExist)
	{
		tTemplateStarMatch = new thread(&TianLi_ThreadMatch::thread_TemplateStarMatch, this, ref(objStar));
		tIsEndTemplateStarMatch = false;
	}
}

void TianLi_ThreadMatch::setTemplateStar(Mat TemplateStarMat)
{
	TemplateStarMat.copyTo(templateStar);
}

void TianLi_ThreadMatch::setStar(Mat StarMat)
{
	StarMat.copyTo(objStar);
}

void TianLi_ThreadMatch::getObjMinMap(Mat& obj)
{
	//obj.copyTo(objMinMap);
	cvtColor(obj, objMinMap, CV_RGB2GRAY);
	int Avatar_Rect_x = cvRound(obj.cols * 0.4);
	int Avatar_Rect_y = cvRound(obj.rows * 0.4);
	int Avatar_Rect_w = cvRound(obj.cols * 0.2);
	int Avatar_Rect_h = cvRound(obj.rows * 0.2);

	obj(cv::Rect(Avatar_Rect_x, Avatar_Rect_y, Avatar_Rect_w, Avatar_Rect_h)).copyTo(objAvatar);

	//obj(Rect(obj.cols / 2 - 14, obj.rows / 2 - 14, 28, 28)).copyTo(objAvatar);
	//obj(Rect(36, 36, obj.cols - 72, obj.rows - 72)).copyTo(objStar);
	cvtColor(obj(Rect(36, 36, obj.cols - 72, obj.rows - 72)), objStar, CV_RGBA2GRAY);
	isExistObjMinMap = true;
}

void TianLi_ThreadMatch::getObjPaimon(Mat& obj)
{
	obj.copyTo(objPaimon);
	isExistObjPaimon = true;
}

void TianLi_ThreadMatch::getObjUID(Mat& obj)
{
	obj.copyTo(objUID);
	isExistObjUID = true;
}

void TianLi_ThreadMatch::CheckThread()
{
	if (tIsEndSurfMapInit == false)
	{
		CheckThread_SurfMapInit();
	}
	if (tIsEndSurfMapMatch == false)
	{
		CheckThread_SurfMapMatch();
	}
	if (tIsEndTemplatePaimonMatch == false)
	{
		CheckThread_TemplatePaimonMatch();
	}
	if (tIsEndOrbAvatarInit == false)
	{
		CheckThread_OrbAvatarInit();
	}
	if (tIsEndOrbAvatarMatch == false)
	{
		CheckThread_OrbAvatarMatch();
	}
	if (tIsEndTemplateUIDInit == false)
	{
		CheckThread_TemplateUIDInit();
	}
	if (tIsEndTemplateUIDMatch == false)
	{
		CheckThread_TemplateUIDMatch();
	}
	if (tIsEndTemplateStarInit == false)
	{
		CheckThread_TemplateStarInit();
	}
	if (tIsEndTemplateStarMatch == false)
	{
		CheckThread_TemplateStarMatch();
	}
}

void TianLi_ThreadMatch::CheckThread_SurfMapInit()
{
	DWORD exitCode;
	if (tSurfMapInit != nullptr)
	{
		GetExitCodeThread(tSurfMapInit->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tSurfMapInit->join();
			delete tSurfMapInit;
			tSurfMapInit = nullptr;
			tIsEndSurfMapInit = true;
		}
	}
}

void TianLi_ThreadMatch::thread_SurfMapInit(Mat& tar)
{
	surfMap.setMap(tar);
	surfMap.Init();
}

void TianLi_ThreadMatch::CheckThread_SurfMapMatch()
{
	DWORD exitCode;
	if (tSurfMapMatch != nullptr)
	{
		GetExitCodeThread(tSurfMapMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tSurfMapMatch->join();
			delete tSurfMapMatch;
			tSurfMapMatch = nullptr;
			tIsEndSurfMapMatch = true;
		}
	}
}

void TianLi_ThreadMatch::thread_SurfMapMatch(Mat& Obj)
{
	//surfMap.setMinMap(Obj);
	if (isExistObjMinMap)
	{
		//surfMap.setMinMap(objMinMap);
		surfMap.setMinMap(Obj);
		surfMap.SURFMatch();
	}
}

void TianLi_ThreadMatch::CheckThread_TemplatePaimonMatch()
{
	DWORD exitCode;
	if (tTemplatePaimonMatch != nullptr)
	{
		GetExitCodeThread(tTemplatePaimonMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplatePaimonMatch->join();
			delete tTemplatePaimonMatch;
			tTemplatePaimonMatch = nullptr;
			tIsEndTemplatePaimonMatch = true;
		}
	}
}

void TianLi_ThreadMatch::thread_TemplatePaimonMatch(Mat& Template, Mat& Obj)
{
	if (isExistObjPaimon)
	{
		tempPaimon.setPaimonTemplate(Template);
		tempPaimon.setPaimonMat(Obj);
		tempPaimon.TemplatePaimon();
	}
}

void TianLi_ThreadMatch::CheckThread_OrbAvatarInit()
{
	DWORD exitCode;
	if (tOrbAvatarInit != nullptr)
	{
		GetExitCodeThread(tOrbAvatarInit->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tOrbAvatarInit->join();
			delete tOrbAvatarInit;
			tOrbAvatarInit = nullptr;
			tIsEndOrbAvatarInit = true;
		}
	}
}

void TianLi_ThreadMatch::thread_OrbAvatarInit(Mat& tar)
{
	//if (tar.empty());
	assert(!tar.empty());
	orbAvatar.setAvatarTemplate(templateAvatar);
	orbAvatar.Init();
}

void TianLi_ThreadMatch::CheckThread_OrbAvatarMatch()
{
	DWORD exitCode;
	if (tOrbAvatarMatch != nullptr)
	{
		GetExitCodeThread(tOrbAvatarMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tOrbAvatarMatch->join();
			delete tOrbAvatarMatch;
			tOrbAvatarMatch = nullptr;
			tIsEndOrbAvatarMatch = true;
		}
	}
}

void TianLi_ThreadMatch::thread_OrbAvatarMatch(Mat& Obj)
{
	if (isExistObjMinMap)
	{
		orbAvatar.setAvatarMat(Obj);
		orbAvatar.ORBMatch();
	}
}

void TianLi_ThreadMatch::CheckThread_TemplateUIDInit()
{
	DWORD exitCode;
	if (tTemplateUIDInit != nullptr)
	{
		GetExitCodeThread(tTemplateUIDInit->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplateUIDInit->join();
			delete tTemplateUIDInit;
			tTemplateUIDInit = nullptr;
			tIsEndTemplateUIDInit = true;
		}
	}
}

void TianLi_ThreadMatch::thread_TemplateUIDInit(Mat* tar)
{
	tempUID.setUIDTemplate(tar);
	tempUID.Init();
}

void TianLi_ThreadMatch::CheckThread_TemplateUIDMatch()
{
	DWORD exitCode;
	if (tTemplateUIDMatch != nullptr)
	{
		GetExitCodeThread(tTemplateUIDMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplateUIDMatch->join();
			delete tTemplateUIDMatch;
			tTemplateUIDMatch = nullptr;
			tIsEndTemplateUIDMatch = true;
		}
	}
}

void TianLi_ThreadMatch::thread_TemplateUIDMatch(Mat& Obj)
{
	if (isExistObjUID)
	{
		//surfMap.setMinMap(objMinMap);
		tempUID.setUIDMat(Obj);
		tempUID.TemplateUID();
	}
}

void TianLi_ThreadMatch::CheckThread_TemplateStarInit()
{
	DWORD exitCode;
	if (tTemplateStarInit != nullptr)
	{
		GetExitCodeThread(tTemplateStarInit->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplateStarInit->join();
			delete tTemplateStarInit;
			tTemplateStarInit = nullptr;
			tIsEndTemplateStarInit = true;
		}
	}
}

void TianLi_ThreadMatch::thread_TemplateStarInit(Mat& tar)
{
	tempStar.setStarTemplate(tar);
	tempStar.Init();
}

void TianLi_ThreadMatch::CheckThread_TemplateStarMatch()
{
	DWORD exitCode;
	if (tTemplateStarMatch != nullptr)
	{
		GetExitCodeThread(tTemplateStarMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplateStarMatch->join();
			delete tTemplateStarMatch;
			tTemplateStarMatch = nullptr;
			tIsEndTemplateStarMatch = true;
		}
	}
}

void TianLi_ThreadMatch::thread_TemplateStarMatch(Mat& Obj)
{
	if (isExistObjMinMap)
	{
		tempStar.setStarMat(Obj);
		tempStar.TemplateStar();
	}
}

void TianLi_ThreadMatch::GetMatchResults()
{
	if (tIsEndSurfMapMatch)
	{
		if (surfMap.getLocalPos().x > 0 && surfMap.getLocalPos().y > 0)
		{
			pos = surfMap.getLocalPos();
		}
	}
	if (tIsEndSurfMapMatch)
	{
		isContinuity = surfMap.getIsContinuity();
	}
	if (tIsEndTemplatePaimonMatch)
	{
		isPaimonVisial = tempPaimon.getPaimonVisible();
	}
	if (tIsEndOrbAvatarMatch)
	{
		rotationAngle = orbAvatar.getRotationAngle();
	}
	if (tIsEndTemplateUIDMatch)
	{
		uid = tempUID.getUID();
	}
	if (tIsEndTemplateStarMatch)
	{
		isStarVisible = tempStar.getStar();
		//if (isStarVisible)
		//{
		starPos = tempStar.getStarPos();
		//}
	}
}



void ATM_TM_Continuity::setState(bool state)
{
	continuity[0] = continuity[1];
	continuity[1] = continuity[2];
	continuity[2] = state;
}

bool ATM_TM_Continuity::getIsContinuity()
{
	return continuity[0] && continuity[1] && continuity[2];
}

bool TianLi_GiState::isRun()
{
	isRunning = IsWindow(giHandle);
	if (giHandle == NULL)
	{
		isRunning = true;
	}
	return isRunning;
}

void TianLi_GiState::getHandle()
{
	if (giName != "原神")
	{
		getRect();
		return;
	}
	giHandle = FindWindowA(giWndClass.c_str(), giName.c_str());
	if (giHandle == NULL)
	{
		LPCWSTR giWindowName = { L"原神" };
		if (giHandle == NULL)
		{
			giWindowName = L"Genshin Impact";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：原神 */
		}
		if (giHandle == NULL)
		{
			giWindowName = L"\u539F\u795E";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：原神 */
		}
		if (giHandle == NULL)
		{
			giWindowName = L"\uC6D0\uC2E0";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：?? */
		}
	}
#ifdef _DEBUG
	//cout << "giHandle: " << giHandle << endl;
#endif
	getRect();
}

void TianLi_GiState::getRect()
{
	if (giHandle != NULL)
	{
		GetWindowRect(giHandle, &giRect);
		GetClientRect(giHandle, &giClientRect);
		int x_offset = GetSystemMetrics(SM_CXDLGFRAME);
		int y_offset = GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION);

		/*SetWindowPos(
			hwnd,
			0,
			margins_xy.cxLeftWidth + (isNoBorder ? 0 : x_offset),
			margins_xy.cxRightWidth + (isNoBorder ? 0 : y_offset),
			margins_size.cyTopHeight,
			margins_size.cyBottomHeight,
			SWP_NOACTIVATE | SWP_FRAMECHANGED
			);
		*/
		giSize.width = giClientRect.right - giClientRect.left;// giRect.right - giRect.left;//+6
		giSize.height = giClientRect.bottom - giClientRect.top;//giRect.bottom - giRect.top;//+29

		getGiRectMode();
	}
	else
	{
		giRect.left = 100;
		giRect.top = 100;
		giRectMode = RectMode::FW_MINIMIZE;
	}
}

int TianLi_GiState::getGiRectMode()
{

	if (giHandle != NULL)
	{
		const Size size1920x1080 = Size(1920, 1080);

		if (giFrame.cols == 1920 && giFrame.rows == 1080)
		{
			isFullScreen = true;
			giSize = size1920x1080;
			giRectMode = RectMode::FW_1920x1080;
			return giRectMode;
		}
		giRectMode = RectMode::FW_UNDIFINDE;
	}
	else
	{
		giRectMode = RectMode::FW_MINIMIZE;
	}
	return giRectMode;
}

int TianLi_GiState::getGiState()
{
	//true ? getHandle() : giRectMode = RectMode::FW_MINIMIZE;
	//isRun() ? getHandle() : giRectMode = RectMode::FW_MINIMIZE;
	if (isRun())
	{
		getHandle();
	}
	else
	{
		giRectMode = RectMode::FW_MINIMIZE;
	}
	
	return giRectMode;
}
Point TianLi_GiState::getOffset()
{
	cv::Point res;
	switch (giRectMode)
	{
	case FW_1920x1080:
	{
		res.x = 288;
		res.y = 82;
		break;
	}
	default:
	{

		break;

	}
	}
	return res;
}

//全屏截屏
void TianLi_GiState::getAllScreen()
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	if (giHandle == NULL)return;

	//获取目标句柄的窗口大小RECT
	//GetWindowRect(giHandle, &giRect);/* 对原神窗口的操作 */

	//获取目标句柄的DC
	HDC hScreen = CreateDCA("DISPLAY", NULL, NULL, NULL);//GetDC(giHandle);/* 对原神窗口的操作 */
	//为屏幕设备描述表创建兼容的内存设备描述表   
	HDC hCompDC = CreateCompatibleDC(hScreen);

	giRect.left = 0;
	giRect.top = 0;
	giRect.right = GetDeviceCaps(hScreen, HORZRES);
	giRect.bottom = GetDeviceCaps(hScreen, VERTRES);

	//获取目标句柄的宽度和高度
	int	nWidth = giRect.right - giRect.left;
	int	nHeight = giRect.bottom - giRect.top;

	//创建Bitmap对象
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//得到位图

	SelectObject(hCompDC, hBmp); //不写就全黑
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//释放对象
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//类型转换
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

	//mat操作
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

}

void TianLi_GiState::getGiScreen()
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	if (giHandle == NULL)return;

	//获取目标句柄的窗口大小RECT
	GetWindowRect(giHandle, &giRect);/* 对原神窗口的操作 */
	GetClientRect(giHandle, &giClientRect);/* 对原神窗口的操作 */

	//获取目标句柄的DC
	HDC hScreen = GetDC(giHandle);/* 对原神窗口的操作 */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//获取目标句柄的宽度和高度
	int	nWidth = giClientRect.right - giClientRect.left;
	int	nHeight = giClientRect.bottom - giClientRect.top;

	//创建Bitmap对象
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//得到位图

	SelectObject(hCompDC, hBmp); //不写就全黑
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//释放对象
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//类型转换
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

	//mat操作
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

}

void TianLi_GiState::reSizeFrame()
{
	int& x = giFrame.cols, & y = giFrame.rows;
	double f = 1, fx = 1, fy = 1;

	if (static_cast<double>(x) / static_cast<double>(y) == 16.0 / 9.0)
	{
		//正常，不做处理
		if (x != 1920 && y != 1080)
		{
			cv::resize(giFrame, giFrame, cv::Size(1920, 1080));
		}
	}
	else if (static_cast<double>(x) / static_cast<double>(y) > 16.0 / 9.0)
	{

		//高型，以宽为比例

		// x = (y * 16) / 9;
		f = y / 1080.0;
		//将giFrame缩放到1920*1080的比例
		fx = x / f;
		// 将图片缩放
		cv::resize(giFrame, giFrame, cv::Size(static_cast<int>(fx), 1080));

	}
	else if (static_cast<double>(x) / static_cast<double>(y) < 16.0 / 9.0)
	{

		//宽型，以高为比例

		// x = (y * 16) / 9;
		f = x / 1920.0;
		//将giFrame缩放到1920*1080的比例
		fy = y / f;
		// 将图片缩放
		cv::resize(giFrame, giFrame, cv::Size(1920, static_cast<int>(fy)));
	}
}

void TianLi_GiState::getGiScreen2()
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	//cout << "getGiScreen2: " << giHandle << endl;

	if (giHandle == NULL)return;

	//获取目标句柄的窗口大小RECT
	//GetWindowRect(giHandle, &giRect);/* 对原神窗口的操作 */
	GetClientRect(giHandle, &giRect);/* 对原神窗口的操作 */

	//获取目标句柄的DC
	HDC hScreen = GetDC(giHandle);/* 对原神窗口的操作 */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//获取目标句柄的宽度和高度
	int	nWidth = giRect.right - giRect.left;
	int	nHeight = giRect.bottom - giRect.top;

	//创建Bitmap对象
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//得到位图

	SelectObject(hCompDC, hBmp); //不写就全黑
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//释放对象
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//类型转换
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

	//mat操作
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

	if (giFrame.channels() == 3)
	{
		cvtColor(giFrame, giFrame, CV_RGB2RGBA);
	}
}
void TianLi_GiState::getGiFrame()
{
	if (isAutoScreen)
	{
		if (giWndClass == "UnityWndClass")
		{
			getGiScreen();
		}
		else
		{
			getGiScreen2();

		}
		//else
		//{
		//	getScreen(giHandle)
		//}
	}
	else
	{
		getAllScreen();
	}

	reSizeFrame();

	getGiRectMode();
	if (giRectMode > 0)
	{
		getGiFramePaimon();
		getGiFrameMap();
		getGiFrameUID();
	}
#ifdef _DEBUG
	//cout << "Gi Window Rect Mode: "<<giRectMode << endl;
#endif
}

void TianLi_GiState::getGiFramePaimon()
{
	Rect PaimonRect;
	switch (giRectMode)
	{
	case FW_1920x1080:
	{
		PaimonRect = Rect(26, 12, 68, 77);
		resIdPaimon = 0;
		break;
	}
	default:
	{

		PaimonRect = Rect(cvCeil(giSize.width * 0.0135), cvCeil(giSize.width * 0.006075), cvCeil(giSize.width * 0.035), cvCeil(giSize.width * 0.0406));
		resIdPaimon = 0;
		// 适配带鱼屏
		if (giSize.width / giSize.height == 64 / 27)
		{
			PaimonRect = Rect(cvCeil(giSize.width * 0.038), cvCeil(giSize.height * 0.012), cvCeil(giSize.height / 9.0 * 16.0 * 0.035), cvCeil(giSize.height / 9.0 * 16.0 * 0.0406));
		}
		break;
	}
	}
	giFrame(PaimonRect).copyTo(giFramePaimon);

}

void TianLi_GiState::getGiFrameMap()
{
	Rect mapRect;
	switch (giRectMode)
	{
	case FW_1920x1080:
	{
		mapRect = Rect(62, 19, 212, 212);
		break;
	}
	default:
	{
		mapRect = Rect(cvCeil(giSize.width * 0.032), cvCeil(giSize.width * 0.01), cvCeil(giSize.width * 0.11), cvCeil(giSize.width * 0.11));
		if (giSize.width / giSize.height == 64 / 27)
		{
			mapRect = Rect(cvCeil(giSize.width * 0.051), cvCeil(giSize.height / 9.0 * 16.0 * 0.01), cvCeil(giSize.height / 9.0 * 16.0 * 0.11), cvCeil(giSize.height / 9.0 * 16.0 * 0.11));
		}
		break;
	}
	}
	giFrame(mapRect).copyTo(giFrameMap);
}

void TianLi_GiState::getGiFrameUID()
{
	Rect uidRect;
	uidRect = Rect(giFrame.cols - 240, giFrame.rows - 25, 180, 18);
	giFrame(uidRect).copyTo(giFrameUID);
}

void TianLi_GiState::setGiNameClass(LANGID SystemLanguageID)
{
	switch (SystemLanguageID)
	{
	case 0X0804:
	{
		giName = "原神";
		break;
	}
	case 0x0409:
	{
		giName = "Genshin Impact";
		break;
	}
	default:
	{
		giName = "Genshin Impact";//GenshinImpact.
		break;
	}
	}
}

void TianLi_GiState::setGiHandle(HWND GiHandle)
{
	if (GiHandle != NULL)
	{
		char classNameLis[256];
		char nameLis[256];
		//cout << GiHandle << endl;
		giHandle = GiHandle;
		GetClassNameA(GiHandle, classNameLis, 256);
		giWndClass = classNameLis;
		GetWindowTextA(GiHandle, nameLis, 256);
		giName = nameLis;
	}
}
