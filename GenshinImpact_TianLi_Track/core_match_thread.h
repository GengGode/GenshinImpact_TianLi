#pragma once
#include <functional> 
/*
template<typename T_in,typename T_Out>
class core_match_thread
{
public:
	template <typename T_in>
	struct thread_init_input
	{
		T_in in;
	};
	typedef  thread_init_input<T_in> init_input;
	T_in _init_in;
public:
	core_match_thread();
	~core_match_thread();
	
	void set_init_input(thread_init_input<T_in> in);
	void init();
	void init(std::function<int(bool&)> callback_init_finished);
	// 调用回调通知初始化完成
	void notify_init_finished();
	void set_input();
	void join_get();
	
	int work();

};
*/

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
using namespace std;
using namespace cv;

class ATM_TM_SurfMap
{
	Mat _mapMat;
	Mat _minMapMat;
	Mat _minMapLastMat;

	int minHessian = 400;
	double ratio_thresh = 0.66;
	double mapScale = 1.3;//1.3;
	int someSizeR = 106;
	double MatchMatScale = 2.0;

	Ptr<xfeatures2d::SURF> detector, detectorSomeMap;
	std::vector<KeyPoint> Kp_MinMap, Kp_Map, Kp_SomeMap;
	Mat Dp_MinMap, Dp_Map, Dp_SomeMap;

	//static Point hisP[3];
	Point2d hisP[3];

	Point2d pos;
public:
	ATM_TM_SurfMap();
	~ATM_TM_SurfMap();

public:
	bool isInit = false;
	bool isContinuity = false;
	bool isOnCity = false;
	void setMap(Mat mapMat);
	void setMinMap(Mat minMapMat);

	void Init();
	void SURFMatch();
	Point2d SURFMatch(Mat minMapMat);
	Point2d getLocalPos();
	bool getIsContinuity();
private:
	double dis(Point2d& p);
	Point2d SPC(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	//Point SPC2(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	double var(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
};

class ATM_TM_TemplatePaimon
{
	Mat _paimonTemplate;
	Mat _paimonMat;
	bool isPaimonVisible = false;
public:
	void setPaimonTemplate(Mat paimonTemplateMat);
	void setPaimonMat(Mat paimonMat);
	void TemplatePaimon();
	bool getPaimonVisible();

};

class ATM_TM_ORBAvatar
{

	Mat _avatarTemplate;
	Mat _avatarMat;
	double rotationAngle = 0;

	Mat gray0;
	Mat gray1;
	Mat gray2;
	Mat and12;
	Mat dilate_element = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat erode_element = getStructuringElement(MORPH_RECT, Size(2, 2));

public:
	bool isInit = false;

	void setAvatarTemplate(Mat avatarTemplateMat);
	void setAvatarMat(Mat avatarMat);
	void Init();
	void ORBMatch();
	double getRotationAngle();
private:
	double dis(Point p);
	std::vector<Point2f> Vector2UnitVector(std::vector<Point2f> pLis);
	double Line2Angle(Point2f p);
};

class ATM_TM_TemplateUID
{
	struct ATM_TM_Template_NumUID
	{
		Mat UID;
		Mat n[10];
		int max = 10;
	}giNumUID;

	Mat _uidMat;

	int _NumBit[9] = { 0 };
	int _uid = 0;

	int getMaxID(double lis[], int len);

public:
	bool isInit = false;

	void Init();
	void setUIDTemplate(Mat* uidTemplateMat);
	void setUIDMat(Mat uidMat);
	void TemplateUID();
	int getUID();
};

class ATM_TM_TemplateStar
{
	Mat _starTemplate;
	Mat _starMat;
	bool isStarVisible = false;
	vector<Point2d> pos;
public:
	bool isInit = false;

	void Init();
	void setStarTemplate(Mat starTemplateMat);
	void setStarMat(Mat starMat);
	void TemplateStar();
	bool getStar();
	vector<Point2d> getStarPos();
};
