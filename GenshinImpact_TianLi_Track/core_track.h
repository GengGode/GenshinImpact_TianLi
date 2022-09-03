#pragma once
//#include "pch.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
class core_track_continuity
{
	bool continuity[3] = { false };
public:
	core_track_continuity() {}
	
	void setState(bool state);
	bool getIsContinuity();
};

class core_track
{
	Mat mapGray;
	Mat objMinMap;

	Mat templateAvatar;
	Mat objAvatar;

	Mat templatePaimon;
	Mat objPaimon;

	Mat templateStar;
	Mat objStar;

	Mat* templateUID;
	Mat objUID;

	//ATM_TM_SurfMap surfMap;
	//ATM_TM_TemplatePaimon tempPaimon;
	//ATM_TM_ORBAvatar orbAvatar;
	//ATM_TM_TemplateUID tempUID;
	//ATM_TM_TemplateStar tempStar;

public:
	core_track();
	~core_track();
	core_track_continuity continuity_state;

	Point2d pos = Point2d(5352, 9432);
	bool isPaimonVisial = false;
	bool isContinuity = false;
	double rotationAngle = 0;
	int uid = 0;
	bool isStarExist = false;
	bool isStarVisible = false;
	vector<Point2d> starPos;


	bool isExistObjMinMap = false;
	bool isExistObjPaimon = false;
	bool isExistObjUID = false;

	bool tIsEndSurfMapInit = false;
	bool tIsEndSurfMapMatch = false;
	bool tIsEndTemplatePaimonMatch = false;
	bool tIsEndOrbAvatarInit = false;
	bool tIsEndOrbAvatarMatch = false;
	bool tIsEndTemplateUIDInit = false;
	bool tIsEndTemplateUIDMatch = false;
	bool tIsEndTemplateStarInit = false;
	bool tIsEndTemplateStarMatch = false;

	thread* tSurfMapInit = nullptr;
	thread* tSurfMapMatch = nullptr;
	thread* tTemplatePaimonMatch = nullptr;
	thread* tOrbAvatarInit = nullptr;
	thread* tOrbAvatarMatch = nullptr;
	thread* tTemplateUIDInit = nullptr;
	thread* tTemplateUIDMatch = nullptr;
	thread* tTemplateStarInit = nullptr;
	thread* tTemplateStarMatch = nullptr;

	//Init
	void cThreadInit();

	//SURF Match Map
	void cThreadSurfMapInit(Mat& Map);
	void cThreadSurfMapMatch();
	void setSurfMap(Mat mapMat);

	//Template Match Paimon
	void cThreadTemplatePaimonMatch(Mat& Template);
	void setTemplatePaimon(Mat TemplatePaimonMat);
	void setPaimon(Mat PaimonMat);

	//ORB Match Avatar
	void cThreadOrbAvatarInit(Mat& TemplatAvatar);
	void cThreadOrbAvatarMatch();
	void setAvatat(Mat AvatarMat);

	//Template Match UID
	void cThreadTemplateUIDInit(Mat* TemplateUID);
	void cThreadTemplateUIDMatch();
	void setUID(Mat UIDMat);

	//Template Match Star
	void cThreadTemplateStarInit(Mat& TemplateStar);
	void cThreadTemplateStarMatch();
	void setTemplateStar(Mat TemplateStarMat);
	void setStar(Mat StarMat);

	//Set Some Mat
	void getObjMinMap(Mat& obj);
	void getObjPaimon(Mat& obj);
	void getObjUID(Mat& obj);

	//Check Thread End
	void CheckThread();

	//SURF Init
	void CheckThread_SurfMapInit();
	void thread_SurfMapInit(Mat& tar);

	//SURF Match
	void CheckThread_SurfMapMatch();
	void thread_SurfMapMatch(Mat& Obj);

	//Template Match
	void CheckThread_TemplatePaimonMatch();
	void thread_TemplatePaimonMatch(Mat& Template, Mat& Obj);

	//ORB Init
	void CheckThread_OrbAvatarInit();
	void thread_OrbAvatarInit(Mat& tar);

	//ORB Match
	void CheckThread_OrbAvatarMatch();
	void thread_OrbAvatarMatch(Mat& Obj);

	//Template UID Init
	void CheckThread_TemplateUIDInit();
	void thread_TemplateUIDInit(Mat* tar);

	//Template UID Match
	void CheckThread_TemplateUIDMatch();
	void thread_TemplateUIDMatch(Mat& Obj);

	//Template Star Init
	void CheckThread_TemplateStarInit();
	void thread_TemplateStarInit(Mat& tar);

	//Template Star Match
	void CheckThread_TemplateStarMatch();
	void thread_TemplateStarMatch(Mat& Obj);

	void GetMatchResults();
};
*/

#include "core_match_thread.h"

#include <condition_variable>
#include <mutex>
#include <thread>

class ATM_TM_Continuity
{
	bool continuity[3] = { false };
public:
	void setState(bool state);
	bool getIsContinuity();
};

class TianLi_ThreadMatch
{
	Mat mapGray;
	Mat objMinMap;

	Mat templateAvatar;
	Mat objAvatar;

	Mat templatePaimon;
	Mat objPaimon;

	Mat templateStar;
	Mat objStar;

	Mat* templateUID;
	Mat objUID;

	ATM_TM_SurfMap surfMap;
	ATM_TM_TemplatePaimon tempPaimon;
	ATM_TM_ORBAvatar orbAvatar;
	ATM_TM_TemplateUID tempUID;
	ATM_TM_TemplateStar tempStar;
	//ATM_TM_Thread tempUID;
public:
	~TianLi_ThreadMatch();
	ATM_TM_Continuity continuityState;
	//Point pos = Point(1466, 3272);
	Point2d pos = Point2d(5352, 9432);
	bool isPaimonVisial = false;
	bool isContinuity = false;
	double rotationAngle = 0;
	int uid = 0;
	bool isStarExist = false;
	bool isStarVisible = false;
	vector<Point2d> starPos;


	bool isExistObjMinMap = false;
	bool isExistObjPaimon = false;
	bool isExistObjUID = false;

	bool tIsEndSurfMapInit = false;
	bool tIsEndSurfMapMatch = false;
	bool tIsEndTemplatePaimonMatch = false;
	bool tIsEndOrbAvatarInit = false;
	bool tIsEndOrbAvatarMatch = false;
	bool tIsEndTemplateUIDInit = false;
	bool tIsEndTemplateUIDMatch = false;
	bool tIsEndTemplateStarInit = false;
	bool tIsEndTemplateStarMatch = false;

	std::thread* tSurfMapInit = nullptr;
	std::thread* tSurfMapMatch = nullptr;
	std::thread* tTemplatePaimonMatch = nullptr;
	std::thread* tOrbAvatarInit = nullptr;
	std::thread* tOrbAvatarMatch = nullptr;
	std::thread* tTemplateUIDInit = nullptr;
	std::thread* tTemplateUIDMatch = nullptr;
	std::thread* tTemplateStarInit = nullptr;
	std::thread* tTemplateStarMatch = nullptr;

	//Init
	void cThreadInit();

	//SURF Match Map
	void cThreadSurfMapInit(Mat& Map);
	void cThreadSurfMapMatch();
	void setSurfMap(Mat mapMat);

	//Template Match Paimon
	void cThreadTemplatePaimonMatch(Mat& Template);
	void setTemplatePaimon(Mat TemplatePaimonMat);
	void setPaimon(Mat PaimonMat);

	//ORB Match Avatar
	void cThreadOrbAvatarInit(Mat& TemplatAvatar);
	void cThreadOrbAvatarMatch();
	void setAvatat(Mat AvatarMat);

	//Template Match UID
	void cThreadTemplateUIDInit(Mat* TemplateUID);
	void cThreadTemplateUIDMatch();
	void setUID(Mat UIDMat);

	//Template Match Star
	void cThreadTemplateStarInit(Mat& TemplateStar);
	void cThreadTemplateStarMatch();
	void setTemplateStar(Mat TemplateStarMat);
	void setStar(Mat StarMat);

	//Set Some Mat
	void getObjMinMap(Mat& obj);
	void getObjPaimon(Mat& obj);
	void getObjUID(Mat& obj);

	//Check Thread End
	void CheckThread();

	//SURF Init
	void CheckThread_SurfMapInit();
	void thread_SurfMapInit(Mat& tar);

	//SURF Match
	void CheckThread_SurfMapMatch();
	void thread_SurfMapMatch(Mat& Obj);

	//Template Match
	void CheckThread_TemplatePaimonMatch();
	void thread_TemplatePaimonMatch(Mat& Template, Mat& Obj);

	//ORB Init
	void CheckThread_OrbAvatarInit();
	void thread_OrbAvatarInit(Mat& tar);

	//ORB Match
	void CheckThread_OrbAvatarMatch();
	void thread_OrbAvatarMatch(Mat& Obj);

	//Template UID Init
	void CheckThread_TemplateUIDInit();
	void thread_TemplateUIDInit(Mat* tar);

	//Template UID Match
	void CheckThread_TemplateUIDMatch();
	void thread_TemplateUIDMatch(Mat& Obj);

	//Template Star Init
	void CheckThread_TemplateStarInit();
	void thread_TemplateStarInit(Mat& tar);

	//Template Star Match
	void CheckThread_TemplateStarMatch();
	void thread_TemplateStarMatch(Mat& Obj);

	void GetMatchResults();
public:
	std::condition_variable con_var;
	std::mutex lock_mutex;
};

enum RectMode
{
	FW_UNDIFINDE = -1,
	FW_MINIMIZE = 0,
	FW_1920x1080, // 16/9, 			
};


class TianLi_GiState
{
public:
	//原神游戏窗口句柄
	HWND giHandle;
	//原神窗口区域
	RECT giRect;
	//原神窗口区域保存对比用
	RECT giRectTmp;
	RECT giClientRect;

	//原神游戏窗口截图
	Mat giFrame;
	//原神游戏窗口截图有效区域
	//Mat giFrameROI;
	//
	Mat giFrameRect;
	//
	Mat giFramePaimon;
	//
	Mat giFrameMap;

	Mat giFrameUID;

	string giWndClass = "UnityWndClass";
	string giName = "原神";

	//原神是否运行中
	bool isRunning = false;
	//派蒙是否可见即是否处于主画面
	bool isPaimonVisible = false;
	//原神是否全屏
	bool isFullScreen = false;
	//窗口分辨率模式
	int giRectMode = RectMode::FW_MINIMIZE;

	int resIdPaimon = 0;
	//窗口大小
	Size giSize;

	bool isAutoScreen = true;

	bool isRun();
	void getHandle();
	void getRect();
	int getGiRectMode();
	int getGiState();
	Point getOffset();

	void getAllScreen();

	//void getScreen(HWND handle); 
	void reSizeFrame();

	void getGiScreen();
	void getGiScreen2();
	void getGiFrame();
	void getGiFramePaimon();
	void getGiFrameMap();
	void getGiFrameUID();

	void setGiNameClass(LANGID SystemLanguageID);

	void setGiHandle(HWND GiHandle);
public:
	std::condition_variable con_var;
	std::mutex con_var_mutex;
};
