#include "pch.h"
#include "GenshinImpact_TianLi_Track_Utils_MiniMap.h"

SurfMatch::SurfMatch()
{
	hisP[0] = cv::Point();
	hisP[1] = cv::Point();
	hisP[2] = cv::Point();
}

SurfMatch::~SurfMatch()
{
}

void SurfMatch::setMap(cv::Mat gi_map)
{
	_mapMat = gi_map;
}

void SurfMatch::setMinMap(cv::Mat minMapMat)
{
	_minMapMat = minMapMat;
}

void SurfMatch::Init()
{
	if (isInit)return;
	detector = cv::xfeatures2d::SURF::create(minHessian);
	detector->detectAndCompute(_mapMat, cv::noArray(), Kp_Map, Dp_Map);
	isInit = true;
}

void SurfMatch::Init(std::vector<cv::KeyPoint>& gi_map_keypoints, cv::Mat& gi_map_descriptors)
{
	if (isInit)return;
	Kp_Map = std::move(gi_map_keypoints);
	Dp_Map = std::move(gi_map_descriptors);
	isInit = true;
}

void SurfMatch::match()
{
	cv::Point2d dp1 = hisP[1] - hisP[0];
	cv::Point2d dp2 = hisP[2] - hisP[1];

	bool calc_is_faile = false;

	isContinuity = false;

	int someSizeR = 200;// (img_object.cols + img_object.rows) / 4;

	//角色移动连续性判断
	if (((dis(dp1) + dis(dp2)) < 2000) && (hisP[2].x > someSizeR && hisP[2].x < _mapMat.cols - someSizeR && hisP[2].y>someSizeR && hisP[2].y < _mapMat.rows - someSizeR))
	{
		isContinuity = true;
	}

	if (isContinuity)
	{
		bool calc_continuity_is_faile = false;

		pos = match_continuity(calc_continuity_is_faile);

		if (calc_continuity_is_faile)
		{
			isContinuity = false;
		}
	}

	if (!isContinuity)
	{
		pos = match_no_continuity(calc_is_faile);
	}

	if (calc_is_faile)
	{
		return;
	}

	hisP[0] = hisP[1];
	hisP[1] = hisP[2];
	hisP[2] = pos;

}

cv::Point2d  SurfMatch::match_continuity(bool& calc_continuity_is_faile)
{
	cv::Point2d pos_continuity;

	if (isOnCity == false)
	{
		pos_continuity = match_continuity_not_on_city(calc_continuity_is_faile);
	}
	else
	{
		pos_continuity = match_continuity_on_city(calc_continuity_is_faile);
	}

	return pos_continuity;
}

cv::Point2d SurfMatch::match_continuity_on_city(bool& calc_continuity_is_faile)
{
	cv::Point2d pos_on_city;

	cv::Mat img_scene(_mapMat);
	cv::Mat img_object(_minMapMat(cv::Rect(30, 30, _minMapMat.cols - 60, _minMapMat.rows - 60)));
	
	int someSizeR = (img_object.cols + img_object.rows) / 4;

	//在城镇中
		/***********************/
		//重新从完整中地图取出角色周围部分地图
	cv::Mat someMap(img_scene(cv::Rect(static_cast<int>(hisP[2].x - someSizeR), static_cast<int>(hisP[2].y - someSizeR), someSizeR * 2, someSizeR * 2)));
	cv::Mat minMap(img_object);

	resize(someMap, someMap, cv::Size(someSizeR * 4, someSizeR * 4));

	detectorSomeMap = cv::xfeatures2d::SURF::create(minHessian);
	detectorSomeMap->detectAndCompute(someMap, cv::noArray(), Kp_SomeMap, Dp_SomeMap);
	detectorSomeMap->detectAndCompute(minMap, cv::noArray(), Kp_MinMap, Dp_MinMap);

	if (Kp_SomeMap.size() <= 2 || Kp_MinMap.size() <= 2)
	{
		calc_continuity_is_faile = true;
		return pos_on_city;
	}

	cv::Ptr<cv::DescriptorMatcher> matcherTmp = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
	std::vector< std::vector<cv::DMatch> > KNN_mTmp;

	matcherTmp->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mTmp, 2);
	std::vector<double> lisx;
	std::vector<double> lisy;
	double sumx = 0;
	double sumy = 0;

	calc_good_matches(someMap, Kp_SomeMap, img_object, Kp_MinMap, KNN_mTmp, ratio_thresh, 0.8667, lisx, lisy, sumx, sumy);

	if (std::max(lisx.size(), lisy.size()) <= 4)
	{
		calc_continuity_is_faile = true;
		return pos_on_city;
	}

	if (std::min(lisx.size(), lisy.size()) >= 10)
	{
		isOnCity = true;
	}
	else
	{
		isOnCity = false;
	}

	cv::Point2d pos_continuity_on_city = SPC(lisx, sumx, lisy, sumy);

	pos_continuity_on_city.x = (pos_continuity_on_city.x - someMap.cols / 2.0) / 2.0;
	pos_continuity_on_city.y = (pos_continuity_on_city.y - someMap.rows / 2.0) / 2.0;

	pos_on_city = cv::Point2d(pos_continuity_on_city.x + hisP[2].x, pos_continuity_on_city.y + hisP[2].y);

	return pos_on_city;
}

cv::Point2d SurfMatch::match_continuity_not_on_city(bool& calc_continuity_is_faile)
{
	cv::Point2d pos_not_on_city;

	cv::Mat img_scene(_mapMat);
	cv::Mat img_object(_minMapMat(cv::Rect(30, 30, _minMapMat.cols - 60, _minMapMat.rows - 60)));
	
	int someSizeR = (img_object.cols + img_object.rows) / 4;

	//不在城镇中时
	cv::Mat someMap(img_scene(cv::Rect(static_cast<int>(hisP[2].x - someSizeR), static_cast<int>(hisP[2].y - someSizeR), someSizeR * 2, someSizeR * 2)));
	cv::Mat minMap(img_object);

	detectorSomeMap = cv::xfeatures2d::SURF::create(minHessian);
	detectorSomeMap->detectAndCompute(someMap, cv::noArray(), Kp_SomeMap, Dp_SomeMap);
	detectorSomeMap->detectAndCompute(minMap, cv::noArray(), Kp_MinMap, Dp_MinMap);

	// 如果搜索范围内可识别特征点数量为0，则认为计算失败
	if (Kp_SomeMap.size() == 0 || Kp_MinMap.size() <= 2)
	{
		calc_continuity_is_faile = true;
		return pos_not_on_city;
	}
	cv::Ptr<cv::DescriptorMatcher> matcher_not_on_city = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
	std::vector< std::vector<cv::DMatch> > KNN_not_no_city;

	matcher_not_on_city->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_not_no_city, 2);
	std::vector<double> lisx;
	std::vector<double> lisy;
	double sumx = 0;
	double sumy = 0;

	calc_good_matches(someMap, Kp_SomeMap, img_object, Kp_MinMap, KNN_not_no_city, ratio_thresh, render_map_scale, lisx, lisy, sumx, sumy);

	// 如果范围内最佳匹配特征点对数量大于4，则认为不可能处于城镇之中，位于城镇之外
	if (std::min(lisx.size(), lisy.size()) > 4)
	{
		isOnCity = false;

		cv::Point2d p = SPC(lisx, sumx, lisy, sumy);
		pos_not_on_city = cv::Point2d(p.x + hisP[2].x - someSizeR, p.y + hisP[2].y - someSizeR);
	}
	else
	{

		//有可能处于城镇中

		/***********************/
		//重新从完整中地图取出角色周围部分地图
		img_scene(cv::Rect(static_cast<int>(hisP[2].x - someSizeR), static_cast<int>(hisP[2].y - someSizeR), someSizeR * 2, someSizeR * 2)).copyTo(someMap);
		//Mat minMap(img_object);

		resize(someMap, someMap, cv::Size(someSizeR * 4, someSizeR * 4));
		//resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);

		detectorSomeMap = cv::xfeatures2d::SURF::create(minHessian);
		detectorSomeMap->detectAndCompute(someMap, cv::noArray(), Kp_SomeMap, Dp_SomeMap);
		//detectorSomeMap->detectAndCompute(minMap, noArray(), Kp_MinMap, Dp_MinMap);
		if (Kp_SomeMap.size() == 0 || Kp_MinMap.size() == 0)
		{
			calc_continuity_is_faile = true;
			return pos_not_on_city;
		}

		cv::Ptr<cv::DescriptorMatcher> matcher_mabye_on_city = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
		std::vector< std::vector<cv::DMatch> > KNN_mabye_on_city;

		matcher_mabye_on_city->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mabye_on_city, 2);

		std::vector<double> list_x_on_city;
		std::vector<double> list_y_on_city;
		double sum_x_on_city = 0;
		double sum_y_on_city = 0;

		calc_good_matches(someMap, Kp_SomeMap, img_object, Kp_MinMap, KNN_mabye_on_city, ratio_thresh, 0.8667, list_x_on_city, list_y_on_city, sum_x_on_city, sum_y_on_city);

		if (std::min(list_x_on_city.size(), list_y_on_city.size()) <= 4)
		{
			calc_continuity_is_faile = true;
			return pos_not_on_city;
		}

		if (std::min(list_x_on_city.size(), list_y_on_city.size()) >= 10)
		{
			isOnCity = true;
		}
		else
		{
			isOnCity = false;
		}

		cv::Point2d p = SPC(list_x_on_city, sum_x_on_city, list_y_on_city, sum_y_on_city);

		double x = (p.x - someMap.cols / 2.0) / 2.0;
		double y = (p.y - someMap.rows / 2.0) / 2.0;

		pos_not_on_city = cv::Point2d(x + hisP[2].x, y + hisP[2].y);

	}

	return pos_not_on_city;
}

cv::Point2d SurfMatch::match_no_continuity(bool& calc_is_faile)
{
	cv::Point2d pos_continuity_no;

	// TODO: 可优化为static
	cv::Mat img_scene(_mapMat);
	cv::Mat img_object(_minMapMat(cv::Rect(30, 30, _minMapMat.cols - 60, _minMapMat.rows - 60)));

	detector->detectAndCompute(img_object, cv::noArray(), Kp_MinMap, Dp_MinMap);

	if (Kp_MinMap.size() == 0)
	{
		calc_is_faile = true;
		return pos_continuity_no;
	}

	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
	std::vector< std::vector<cv::DMatch> > KNN_m;

	matcher->knnMatch(Dp_MinMap, Dp_Map, KNN_m, 2);

	std::vector<double> lisx;
	std::vector<double> lisy;
	double sumx = 0;
	double sumy = 0;

	calc_good_matches(img_scene, Kp_Map, img_object, Kp_MinMap, KNN_m, ratio_thresh, render_map_scale, lisx, lisy, sumx, sumy);

	if (std::min(lisx.size(), lisy.size()) == 0)
	{
		calc_is_faile = true;
		return pos_continuity_no;
	}

	pos_continuity_no = SPC(lisx, sumx, lisy, sumy);

	return pos_continuity_no;
}


namespace CalcMatch
{
	namespace Debug
	{
		void calc_good_matches_show(cv::Mat& img_scene, std::vector<cv::KeyPoint> keypoint_scene, cv::Mat& img_object, std::vector<cv::KeyPoint> keypoint_object, std::vector<std::vector<cv::DMatch>>& KNN_m, double ratio_thresh, double mapScale, std::vector<double>& lisx, std::vector<double>& lisy, double& sumx, double& sumy)
		{
			std::vector<cv::DMatch> good_matches;
			for (size_t i = 0; i < KNN_m.size(); i++)
			{
				if (KNN_m[i][0].distance < ratio_thresh * KNN_m[i][1].distance)
				{
					good_matches.push_back(KNN_m[i][0]);
					if (KNN_m[i][0].queryIdx >= keypoint_object.size())
					{
						continue;
					}
					lisx.push_back(((img_object.cols / 2.0 - keypoint_object[KNN_m[i][0].queryIdx].pt.x) * mapScale + keypoint_scene[KNN_m[i][0].trainIdx].pt.x));
					lisy.push_back(((img_object.rows / 2.0 - keypoint_object[KNN_m[i][0].queryIdx].pt.y) * mapScale + keypoint_scene[KNN_m[i][0].trainIdx].pt.y));
					sumx += lisx.back();
					sumy += lisy.back();
				}
			}
			draw_good_matches(img_scene, keypoint_scene, img_object, keypoint_object, good_matches);
				}
		}

	void calc_good_matches_show(cv::Mat&, std::vector<cv::KeyPoint> keypoint_scene, cv::Mat& img_object, std::vector<cv::KeyPoint> keypoint_object, std::vector<std::vector<cv::DMatch>>& KNN_m, double ratio_thresh, double mapScale, std::vector<double>& lisx, std::vector<double>& lisy, double& sumx, double& sumy)
	{
		for (size_t i = 0; i < KNN_m.size(); i++)
		{
			if (KNN_m[i][0].distance < ratio_thresh * KNN_m[i][1].distance)
			{
				if (KNN_m[i][0].queryIdx >= keypoint_object.size())
				{
					continue;
				}
				lisx.push_back(((img_object.cols / 2.0 - keypoint_object[KNN_m[i][0].queryIdx].pt.x) * mapScale + keypoint_scene[KNN_m[i][0].trainIdx].pt.x));
				lisy.push_back(((img_object.rows / 2.0 - keypoint_object[KNN_m[i][0].queryIdx].pt.y) * mapScale + keypoint_scene[KNN_m[i][0].trainIdx].pt.y));
				sumx += lisx.back();
				sumy += lisy.back();
			}
		}
	}

	}

void calc_good_matches(cv::Mat& img_scene, std::vector<cv::KeyPoint> keypoint_scene, cv::Mat& img_object, std::vector<cv::KeyPoint> keypoint_object, std::vector<std::vector<cv::DMatch>>& KNN_m, double ratio_thresh, double mapScale, std::vector<double>& lisx, std::vector<double>& lisy, double& sumx, double& sumy)
{
	CalcMatch::
#ifdef _DEBUG
		Debug::
#endif
		calc_good_matches_show(img_scene, keypoint_scene, img_object, keypoint_object, KNN_m, ratio_thresh, mapScale, lisx, lisy, sumx, sumy);
}


void draw_good_matches(cv::Mat& img_scene, std::vector<cv::KeyPoint> keypoint_scene, cv::Mat& img_object, std::vector<cv::KeyPoint> keypoint_object, std::vector<cv::DMatch>& good_matches)
{
	cv::Mat img_matches, imgmap, imgminmap;
	drawKeypoints(img_scene, keypoint_scene, imgmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(img_object, keypoint_object, imgminmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	drawMatches(img_object, keypoint_object, img_scene, keypoint_scene, good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
}

cv::Point2d SurfMatch::SURFMatch(cv::Mat minMapMat)
{
	return cv::Point2d();
}

cv::Point2d SurfMatch::getLocalPos()
{
	return pos;
}

bool SurfMatch::getIsContinuity()
{
	return isContinuity;
}

double SurfMatch::dis(cv::Point2d& p)
{
	return sqrt(p.x * p.x + p.y * p.y);
}

cv::Point2d SurfMatch::SPC(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy)
{
	//这个剔除异常点算法
	//回头要改
	cv::Point2d mpos;
	double meanx = sumx / lisx.size(); //均值
	double meany = sumy / lisy.size(); //均值
	double x = meanx;
	double y = meany;
	if (std::min(lisx.size(), lisy.size()) > 3)
	{
		double accumx = 0.0;
		double accumy = 0.0;
		for (int i = 0; i < std::min(lisx.size(), lisy.size()); i++)
		{
			accumx += (lisx[i] - meanx) * (lisx[i] - meanx);
			accumy += (lisy[i] - meany) * (lisy[i] - meany);
		}

		double stdevx = sqrt(accumx / (lisx.size() - 1)); //标准差
		double stdevy = sqrt(accumy / (lisy.size() - 1)); //标准差

		sumx = 0;
		sumy = 0;
		double numx = 0;
		double numy = 0;
		for (int i = 0; i < std::min(lisx.size(), lisy.size()); i++)
		{
			if (abs(lisx[i] - meanx) < 1 * stdevx)
			{
				sumx += lisx[i];
				numx++;
			}

			if (abs(lisy[i] - meany) < 1 * stdevy)
			{
				sumy += lisy[i];
				numy++;
			}
		}
		double xx = sumx / numx;
		double yy = sumy / numy;
		mpos = cv::Point2d(xx, yy);
	}
	else
	{
		mpos = cv::Point2d(x, y);
	}
	return mpos;
}

double SurfMatch::var(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy)
{
	double accumx = 0.0;
	double accumy = 0.0;
	for (int i = 0; i < std::min(lisx.size(), lisy.size()); i++)
	{
		accumx = (lisx[i] - sumx) * (lisx[i] - sumx);
		accumy = (lisy[i] - sumy) * (lisy[i] - sumy);
	}
	double stdevx = sqrt(accumx / (lisx.size() - 1));
	double stdevy = sqrt(accumy / (lisy.size() - 1));

	return sqrt(stdevx * stdevx + stdevy * stdevy);
}

bool func_test_diff_match(const cv::Mat minimap, double& dx, double& dy)
{
	static cv::Mat last_minimap_mat;
	static cv::Point2d pos_minimap = { 0,0 };
	if (minimap.empty())
	{
		std::cout << "frame is empty" << std::endl;
		return false;
	}

	int x = static_cast<int>(minimap.cols * 0.1);
	int y = static_cast<int>(minimap.cols * 0.1);
	int w = static_cast<int>(minimap.cols * 0.8);
	int h = static_cast<int>(minimap.cols * 0.8);

	auto now_minimap_mat = minimap(cv::Rect(x, y, w, h));

	if (last_minimap_mat.empty())
	{
		last_minimap_mat = now_minimap_mat.clone();
		std::cout << "last_minimap_mat is empty" << std::endl;
		return false;
	}
	if (last_minimap_mat.size != now_minimap_mat.size)
	{
		last_minimap_mat = now_minimap_mat.clone();
		std::cout << "last_minimap_mat.size != now_minimap_mat.size" << std::endl;
		return false;
	}
	

	cv::Mat  now_minimap_mat_gray;
	cv::Mat last_minimap_mat_gray;
	cv::cvtColor(now_minimap_mat, now_minimap_mat_gray, cv::COLOR_RGBA2GRAY);
	cv::cvtColor(last_minimap_mat, last_minimap_mat_gray, cv::COLOR_RGBA2GRAY);
	cv::Mat now_minimap_mat_f64;
	cv::Mat last_minimap_mat_f64;
	now_minimap_mat_gray.convertTo(now_minimap_mat_f64, CV_64FC1);
	last_minimap_mat_gray.convertTo(last_minimap_mat_f64, CV_64FC1);

	auto diff_pos = cv::phaseCorrelate(now_minimap_mat_f64, last_minimap_mat_f64);
	
	dx = diff_pos.x;
	dy = diff_pos.y;

	//diff_pos = diff_pos - cv::Point2d(0.5, 0.5);

	//auto diff_pos = calc_diff_pos_match_surf(now_minimap_mat_gray, last_minimap_mat_gray);
	//auto diff_pos = calc_diff_pos(now_minimap_mat_f64, last_minimap_mat_f64);

	std::cout << "diff = " << dx << " , " << dy << std::endl;

	last_minimap_mat = now_minimap_mat.clone();
	
	//
	//pos_minimap += diff_pos;
	//std::cout << "pos_minimap = " << pos_minimap.x << " , " << pos_minimap.y << std::endl;c
	//// 小地图叠加绘制展示
	//static cv::Mat minimap_mat(500, 500, CV_8UC4, cv::Scalar(0, 0, 0, 0));
	//static cv::Point center(250, 250);
	//static bool is_frist = true;
	//if (is_frist)
	//{
	//	is_frist = false;
	//	// 拷贝小地图到偏移位置
	//	cv::Mat minimap_mat_roi = minimap_mat(cv::Rect(static_cast<int>(250 - now_minimap_mat.cols / 2), static_cast<int>(250 - now_minimap_mat.rows / 2), now_minimap_mat.cols, now_minimap_mat.rows));
	//	now_minimap_mat.copyTo(minimap_mat_roi);
	//}

	//cv::Point2d pos_minimap_draw = pos_minimap + cv::Point2d(center);
	//static std::vector<cv::Point2d> pos_history_draw;
	//static int count = 0;
	//count++;
	//if (count == 10)
	//{
	//	count = 0;
	//	pos_history_draw.push_back(pos_minimap_draw);
	//	if (pos_history_draw.size() > 100)
	//	{
	//		pos_history_draw.erase(pos_history_draw.begin());
	//	}
	//}

	//try {

	//	// 绘制偏移位置
	//	cv::Mat minimap_mat_roi = minimap_mat(cv::Rect(static_cast<int>(pos_minimap_draw.x - now_minimap_mat.cols / 2), static_cast<int>(pos_minimap_draw.y - now_minimap_mat.rows / 2), now_minimap_mat.cols, now_minimap_mat.rows));
	//	cv::addWeighted(minimap_mat_roi, 0.5, now_minimap_mat, 0.5, 0, minimap_mat_roi);

	//}
	//catch (const std::exception& e)
	//{
	//	std::cout << e.what() << std::endl;
	//}
	////cv::line(minimap_mat, center, pos_minimap_draw, cv::Scalar(0, 0, 255), 2);
	//for (auto pos : pos_history_draw)
	//{
	//	//cv::line(minimap_mat, center, pos, cv::Scalar(0, 0, 255), 2);
	//	cv::circle(minimap_mat, pos, 2, cv::Scalar(0, 255, 255), 2);
	//}
	//cv::imshow("minimap_mat", minimap_mat);
	//cv::waitKey(1);


	return true;
}
struct Pos
{
	cv::Point2d pos_t0;
	cv::Point2d pos_t;
	double t_dx;
	double t_dy;
	void updata(cv::Point2d pos)
	{
		pos_t0 = pos_t;
		pos_t = pos;
		t_dx = pos_t.x - pos_t0.x;
		t_dy = pos_t.y - pos_t0.y;
	}
	void updata(double dx, double dy)
	{
		pos_t0 = pos_t;
		pos_t = cv::Point2d(pos_t0.x + dx, pos_t0.y + dy);
		t_dx = dx;
		t_dy = dy;
	}
};

bool ins_gps_data_fusion(const Pos& ins, const Pos& gps, double& x, double& y)
{
	//x = ins.x + ins.dx - gps.x - gps.dx;
	//y = ins.y + ins.dy - gps.y - gps.dy;

	// 基于kalman filter的ins和gps数据融合
	// 分别使用kalman filter对ins和gps数据进行滤波
	// 将滤波后的ins和gps数据进行融合
	// 从而得到更加准确的位置信息
	// 1. ins数据滤波
	// 2. gps数据滤波
	// 3. ins和gps数据融合
	// 4. 融合后的数据进行滤波
	// 5. 返回融合后的数据

	// ins数据滤波
	static Filter* ins_filter = nullptr;
	static bool ins_kf_init = false;
	if (!ins_kf_init)
	{
		ins_kf_init = true;
		ins_filter = new Kalman();
		auto ins_pos_d = ins_filter->filterting(ins.pos_t0);

	}
	auto ins_pos = ins_filter->filterting(ins.pos_t);

	// gps数据滤波
	static Filter* gps_filter = nullptr;
	static bool gps_kf_init = false;
	if (!gps_kf_init)
	{
		gps_kf_init = true;
		gps_filter = new Kalman();
		auto gps_pos = gps_filter->filterting(gps.pos_t0);
	}
	auto gps_pos = gps_filter->filterting(gps.pos_t);

	// ins和gps数据融合
	auto pos = (gps_pos + ins_pos);

	// 融合后的数据进行滤波
	static Filter* fusion_filter = nullptr;
	static bool fusion_kf_init = false;
	if (!fusion_kf_init)
	{
		fusion_kf_init = true;
		fusion_filter = new Kalman();
		auto fusion_pos = fusion_filter->filterting(pos);

	}
	auto fusion_pos = fusion_filter->filterting(pos);

	x = fusion_pos.x;
	y = fusion_pos.y;
	
	std::cout << "ins = " << ins_pos.x << " , " << ins_pos.y<<" | ";
	std::cout << "gps = " << gps_pos.x << " , " << gps_pos.y<<" | ";
	std::cout << "fus = " << fusion_pos.x << " , " << fusion_pos.y<< "\n";
	
	return true;
}

void get_avatar_position(const GenshinMinimap& genshin_minimap, GenshinAvatarPosition& out_genshin_position)
{
	static SurfMatch surf_match;
	static bool is_init = false;
	if (!is_init)
	{
		TianLi::XmlDbMem xml_db_mem = GenshinImpact_TianLi_Resource::GetInstance()->LoadXml_GIMAP_COMPUTE();
		// 从内存中加载xml文件的string
		std::string xml_str(xml_db_mem.ptr);
		// 将xml文件的string转换为xml文件
		cv::FileStorage fs(xml_str, cv::FileStorage::MEMORY | cv::FileStorage::READ);
		
		std::vector<cv::KeyPoint> gi_map_keypoints;
		cv::Mat gi_map_descriptors;
		// 从fs加载 keypoints 和 descriptor
		fs["keypoints"] >> gi_map_keypoints;
		fs["descriptors"] >> gi_map_descriptors;
		
		surf_match.setMap(GenshinImpact_TianLi_Resource::GetInstance()->GIMAP);

		surf_match.detector = cv::xfeatures2d::SURF::create(surf_match.minHessian);
		surf_match.detectorSomeMap = cv::xfeatures2d::SURF::create(surf_match.minHessian);
		
		surf_match.Init(gi_map_keypoints, gi_map_descriptors);
		
		is_init = true;
	}
	

	if (genshin_minimap.config.is_find_paimon == false)
	{
		return;
	}

	if (genshin_minimap.img_minimap.empty())
	{
		return;
	}

	surf_match.setMinMap(genshin_minimap.img_minimap);
	
	surf_match.match();
	// GPS数据获取
	out_genshin_position.position = surf_match.getLocalPos();

	//static Pos ins_pos;
	//static Pos gps_pos;
	//static Pos fusion_pos;
	//double dx = 0;
	//double dy = 0;
	//// INS数据获取
	//func_test_diff_match(genshin_minimap.img_minimap, dx, dy);
	//// 从GPS获取绝对位置
	//ins_pos.pos_t = ins_pos.pos_t;
	//ins_pos.updata(dx/2.0, dy/2.0);
	//// GPS数据获取
	//gps_pos.updata(out_genshin_position.position);
	//// INS和GPS数据融合
	//double x = 0;
	//double y = 0;
	//ins_gps_data_fusion(ins_pos, gps_pos, x, y);
	//out_genshin_position.position.x = x;
	//out_genshin_position.position.y = y;
}