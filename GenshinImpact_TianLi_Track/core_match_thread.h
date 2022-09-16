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


class SurfMatch
{
	cv::Mat _mapMat;
	cv::Mat _minMapMat;
	cv::Mat _minMapLastMat;

	double ratio_thresh = 0.66;
	double mapScale = 1.3;//1.3;
	int someSizeR = 106;
	double MatchMatScale = 2.0;




	cv::Point2d hisP[3];

	cv::Point2d pos;
public:
	SurfMatch();
	~SurfMatch();

public:
	int minHessian = 400;
	
	cv::Ptr<cv::xfeatures2d::SURF> detector, detectorSomeMap;
	std::vector<cv::KeyPoint> Kp_MinMap, Kp_Map, Kp_SomeMap;
	cv::Mat Dp_MinMap, Dp_Map, Dp_SomeMap;
	
	bool isInit = false;
	bool isContinuity = false;
	bool isOnCity = false;
	
	void setMap(cv::Mat mapMat);
	void setMinMap(cv::Mat minMapMat);

	void Init();
	void Init(std::vector<cv::KeyPoint> &gi_map_keypoints, cv::Mat& gi_map_descriptors);
	void match();

	cv::Point2d match_continuity(bool& calc_continuity_is_faile);
	cv::Point2d match_continuity_on_city(bool& calc_continuity_is_faile);
	cv::Point2d match_continuity_not_on_city(bool& calc_continuity_is_faile);
	
	cv::Point2d match_no_continuity( bool& calc_is_faile);
	
	cv::Point2d SURFMatch(cv::Mat minMapMat);
	cv::Point2d getLocalPos();
	bool getIsContinuity();
private:
	double dis(cv::Point2d& p);
	cv::Point2d SPC(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy);
	//Point SPC2(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	double var(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy);
};

class ATM_TM_TemplatePaimon
{
	cv::Mat _paimonTemplate;
	cv::Mat _paimonMat;
	bool isPaimonVisible = false;
public:
	void setPaimonTemplate(cv::Mat paimonTemplateMat);
	void setPaimonMat(cv::Mat paimonMat);
	void match();
	bool getPaimonVisible();

};

class ATM_TM_ORBAvatar
{

	cv::Mat _avatarTemplate;
	cv::Mat _avatarMat;
	double rotationAngle = 0;
	
	cv::Mat gray0;
	cv::Mat gray1;
	cv::Mat gray2;
	cv::Mat and12;
	cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
	cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));

public:
	bool isInit = false;

	void setAvatarTemplate(cv::Mat avatarTemplateMat);
	void setAvatarMat(cv::Mat avatarMat);
	void Init();
	void match();
	double getRotationAngle();
private:
	double dis(cv::Point p);
	std::vector<cv::Point2f> Vector2UnitVector(std::vector<cv::Point2f> pLis);
	double Line2Angle(cv::Point2f p);
};

class ATM_TM_TemplateUID
{
	struct ATM_TM_Template_NumUID
	{
		cv::Mat UID;
		cv::Mat n[10];
		int max = 10;
	}giNumUID;

	cv::Mat _uidMat;

	int _NumBit[9] = { 0 };
	int _uid = 0;

	int getMaxID(double lis[], int len);

public:
	bool isInit = false;

	void Init();
	void setUIDTemplate(cv::Mat* uidTemplateMat);
	void setUIDMat(cv::Mat uidMat);
	void match();
	int getUID();
};

class ATM_TM_TemplateStar
{
	cv::Mat _starTemplate;
	cv::Mat _starMat;
	bool isStarVisible = false;
	std::vector<cv::Point2d> pos;
public:
	bool isInit = false;

	void Init();
	void setStarTemplate(cv::Mat starTemplateMat);
	void setStarMat(cv::Mat starMat);
	void match();
	bool getStar();
	std::vector<cv::Point2d> getStarPos();
};

class ATM_TM_TemplateAvatar
{
	cv::Mat _avatarTemplate;
	cv::Mat _avatarMat;
	bool isAvatarVisible = false;
};

void draw_good_matches(cv::Mat& img_scene, std::vector<cv::KeyPoint> keypoint_scene, cv::Mat& img_object, std::vector<cv::KeyPoint> keypoint_object, std::vector<cv::DMatch>& good_matches);

void calc_good_matches(cv::Mat& img_scene, std::vector<cv::KeyPoint> keypoint_scene, cv::Mat& img_object, std::vector<cv::KeyPoint> keypoint_object, std::vector<std::vector<cv::DMatch>>& KNN_m, double ratio_thresh, double mapScale, std::vector<double>& lisx, std::vector<double>& lisy, double& sumx, double& sumy);
