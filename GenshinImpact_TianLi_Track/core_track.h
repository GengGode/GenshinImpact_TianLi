#pragma once
//#include "pch.h"
#include <opencv2/opencv.hpp>

//using namespace std;
//using namespace cv;

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
	cv::Mat mapGray;
	cv::Mat objMinMap;
	
	cv::Mat templateAvatar;
	cv::Mat objAvatar;
	
	cv::Mat templatePaimon;
	cv::Mat objPaimon;
	
	cv::Mat templateStar;
	cv::Mat objStar;
	
	cv::Mat* templateUID;
	cv::Mat objUID;

	SurfMatch surfMap;
	ATM_TM_TemplatePaimon tempPaimon;
	ATM_TM_ORBAvatar orbAvatar;
	ATM_TM_TemplateUID tempUID;
	ATM_TM_TemplateStar tempStar;
	//ATM_TM_Thread tempUID;
public:
	~TianLi_ThreadMatch();
	ATM_TM_Continuity continuityState;
	//Point pos = Point(1466, 3272);
	cv::Point2d pos = cv::Point2d(5352, 9432);
	bool isPaimonVisial = false;
	bool isContinuity = false;
	double rotationAngle = 0;
	int uid = 0;
	bool isStarExist = false;
	bool isStarVisible = false;
	std::vector<cv::Point2d> starPos;


	bool isExistObjMinMap = false;
	bool isExistObjPaimon = false;
	bool isExistObjUID = false;

	bool tIsEndSurfMapInit = false;
	bool tIsEndSurfMapMatch = false;
	bool tIsEndTemplatePaimonInit = false;
	bool tIsEndTemplatePaimonMatch = false;
	bool tIsEndOrbAvatarInit = false;
	bool tIsEndOrbAvatarMatch = false;
	bool tIsEndTemplateUIDInit = false;
	bool tIsEndTemplateUIDMatch = false;
	bool tIsEndTemplateStarInit = false;
	bool tIsEndTemplateStarMatch = false;

	std::thread* tSurfMapInit = nullptr;
	std::thread* tSurfMapMatch = nullptr;
	std::thread* tTemplatePaimonInit = nullptr;
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
	void cThreadSurfMapInit(cv::Mat& Map);
	void cThreadSurfMapMatch();
	void setSurfMap(cv::Mat mapMat);

	//Template Match Paimon
	void cThreadTemplatePaimonInit(cv::Mat& Template);
	void cThreadTemplatePaimonMatch();
	void setTemplatePaimon(cv::Mat TemplatePaimonMat);
	void setPaimon(cv::Mat PaimonMat);

	//ORB Match Avatar
	void cThreadOrbAvatarInit(cv::Mat& TemplatAvatar);
	void cThreadOrbAvatarMatch();
	void setAvatat(cv::Mat AvatarMat);

	//Template Match UID
	void cThreadTemplateUIDInit(cv::Mat* TemplateUID);
	void cThreadTemplateUIDMatch();
	void setUID(cv::Mat UIDMat);

	//Template Match Star
	void cThreadTemplateStarInit(cv::Mat& TemplateStar);
	void cThreadTemplateStarMatch();
	void setTemplateStar(cv::Mat TemplateStarMat);
	void setStar(cv::Mat StarMat);

	//Set Some Mat
	void getObjMinMap(cv::Mat& obj);
	void getObjPaimon(cv::Mat& obj);
	void getObjUID(cv::Mat& obj);

	//Check Thread End
	void CheckThread();

	//SURF Init
	void CheckThread_SurfMapInit();
	void thread_SurfMapInit(cv::Mat& tar);
	
	//SURF Match
	void CheckThread_SurfMapMatch();
	void thread_SurfMapMatch(cv::Mat& Obj);

	//Template Init
	void CheckThread_TemplatePaimonInit();
	void thread_TemplatePaimonInit(cv::Mat& Template);
	
	//Template Match
	void CheckThread_TemplatePaimonMatch();
	void thread_TemplatePaimonMatch(cv::Mat& Obj);

	//ORB Init
	void CheckThread_OrbAvatarInit();
	void thread_OrbAvatarInit(cv::Mat& tar);

	//ORB Match
	void CheckThread_OrbAvatarMatch();
	void thread_OrbAvatarMatch(cv::Mat& Obj);

	//Template UID Init
	void CheckThread_TemplateUIDInit();
	void thread_TemplateUIDInit(cv::Mat* tar);

	//Template UID Match
	void CheckThread_TemplateUIDMatch();
	void thread_TemplateUIDMatch(cv::Mat& Obj);

	//Template Star Init
	void CheckThread_TemplateStarInit();
	void thread_TemplateStarInit(cv::Mat& tar);

	//Template Star Match
	void CheckThread_TemplateStarMatch();
	void thread_TemplateStarMatch(cv::Mat& Obj);

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
	cv::Mat giFrame;
	//原神游戏窗口截图有效区域
	//Mat giFrameROI;
	//
	cv::Mat giFrameRect;
	//
	cv::Mat giFramePaimon;
	//
	cv::Mat giFrameMap;

	cv::Mat giFrameUID;

	std::string giWndClass = "UnityWndClass";
	std::string giName = "原神";

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
	cv::Size giSize;

	bool isAutoScreen = true;

	bool isRun();
	void getHandle();
	void getRect();
	int getGiRectMode();
	int getGiState();
	cv::Point getOffset();

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
