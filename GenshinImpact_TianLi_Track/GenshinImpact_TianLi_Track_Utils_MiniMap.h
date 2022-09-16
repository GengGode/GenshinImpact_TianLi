#pragma once
#include "GenshinImpact_TianLi_Track_Utils_Struct.h"

#include <opencv2/xfeatures2d.hpp>

inline void check_paimon(const GenshinScreen& genshin_screen, GenshinPaimon& out_genshin_paimon)
{
	static std::vector<cv::Mat> split_paimon_template;
	static cv::Mat paimon_template_handle_mode;
	static bool is_first = true;
	if (is_first)
	{
		cv::Mat paimon;
		cv::resize(GenshinImpact_TianLi_Resource::GetInstance()->GIPAIMON, paimon, cv::Size(68, 77));
		cv::split(paimon, split_paimon_template);
		cv::resize(split_paimon_template[3], paimon_template_handle_mode, cv::Size(), 1.0 / 1.20, 1.0 / 1.20);
		is_first = false;
	}
	auto& giPaimonRef = genshin_screen.img_paimon_maybe;
	auto& rect_origin = genshin_screen.config.rect_paimon_maybe;

	if (giPaimonRef.empty() || paimon_template_handle_mode.empty()) return;
	if (giPaimonRef.cols < split_paimon_template[3].cols || giPaimonRef.rows < split_paimon_template[3].rows) return;

	std::vector<cv::Mat>  split_paimon;
	cv::split(giPaimonRef, split_paimon);

	cv::Mat template_result;
	// TODO HOTCODE
	cv::matchTemplate(split_paimon[3], split_paimon_template[3], template_result, cv::TM_CCOEFF_NORMED);

	double paimon_match_minVal, paimon_match_maxVal;
	cv::Point paimon_match_minLoc, paimon_match_maxLoc;
	cv::minMaxLoc(template_result, &paimon_match_minVal, &paimon_match_maxVal, &paimon_match_minLoc, &paimon_match_maxLoc);

	if (paimon_match_maxVal < out_genshin_paimon.config.check_match_paimon_params || paimon_match_maxVal == 1)
	{
		if (paimon_match_maxVal > 0.5)
		{
			cv::Mat template_handle_mode_result;
			// TODO HOTCODE
			cv::matchTemplate(split_paimon[3], paimon_template_handle_mode, template_handle_mode_result, cv::TM_CCOEFF_NORMED);

			double paimon_match_handle_mode_minVal, paimon_match_handle_mode_maxVal;
			cv::Point paimon_match_handle_mode_minLoc, paimon_match_handle_mode_maxLoc;
			cv::minMaxLoc(template_handle_mode_result, &paimon_match_handle_mode_minVal, &paimon_match_handle_mode_maxVal, &paimon_match_handle_mode_minLoc, &paimon_match_handle_mode_maxLoc);
			if (paimon_match_handle_mode_maxVal > out_genshin_paimon.config.check_match_paimon_params)
			{
				out_genshin_paimon.is_handle_mode = true;
				out_genshin_paimon.is_visial = true;
				out_genshin_paimon.rect_paimon = cv::Rect(rect_origin.tl() + paimon_match_handle_mode_maxLoc, paimon_template_handle_mode.size());
			}
		}
		else
		{
			out_genshin_paimon.is_visial = false;
		}
	}
	else
	{
		out_genshin_paimon.is_handle_mode = false;
		out_genshin_paimon.is_visial = true;
		out_genshin_paimon.rect_paimon = cv::Rect(rect_origin.tl() + paimon_match_maxLoc, split_paimon_template[3].size());
	}
}

inline void match_minimap_cailb(const GenshinScreen& genshin_screen, GenshinMinimapCailb& out_genshin_minimap_cailb)
{
	static std::vector<cv::Mat> split_minimap_cailb_template;
	static cv::Mat minimap_cailb_template_handle_mode;
	static bool is_first = true;
	if (is_first)
	{
		cv::Mat minimap_cailb;
		cv::resize(GenshinImpact_TianLi_Resource::GetInstance()->GIMINIMAP_CAILB, minimap_cailb, cv::Size(), 0.8, 0.8);
		cv::split(minimap_cailb, split_minimap_cailb_template);
		cv::resize(split_minimap_cailb_template[3], minimap_cailb_template_handle_mode, cv::Size(), 1 / 1.2, 1 / 1.2, cv::INTER_CUBIC);
		is_first = false;
	}

	auto& giMinimapCailbRef = genshin_screen.img_minimap_cailb_maybe;
	auto& rect_origin = genshin_screen.config.rect_minimap_cailb_maybe;
	auto& is_handle_mode = genshin_screen.config.is_handle_mode;

	if (giMinimapCailbRef.empty() || minimap_cailb_template_handle_mode.empty()) return;
	if (giMinimapCailbRef.cols < split_minimap_cailb_template[3].cols || giMinimapCailbRef.rows < split_minimap_cailb_template[3].rows) return;

	std::vector<cv::Mat>  split_minimap_cailb;
	cv::split(giMinimapCailbRef, split_minimap_cailb);

	cv::Mat template_result;
	if (!is_handle_mode)
	{
		// TODO HOTCODE
		cv::matchTemplate(split_minimap_cailb[3], split_minimap_cailb_template[3], template_result, cv::TM_CCOEFF_NORMED);
	}
	else
	{
		// TODO HOTCODE
		cv::matchTemplate(split_minimap_cailb[3], minimap_cailb_template_handle_mode, template_result, cv::TM_CCOEFF_NORMED);
	}


	double minimap_cailb_match_minVal, minimap_cailb_match_maxVal;
	cv::Point minimap_cailb_match_minLoc, minimap_cailb_match_maxLoc;
	cv::minMaxLoc(template_result, &minimap_cailb_match_minVal, &minimap_cailb_match_maxVal, &minimap_cailb_match_minLoc, &minimap_cailb_match_maxLoc);

	if (minimap_cailb_match_maxVal < out_genshin_minimap_cailb.config.check_match_minimap_cailb_params || minimap_cailb_match_maxVal == 1)
	{
		out_genshin_minimap_cailb.is_visial = false;
	}
	else
	{
		out_genshin_minimap_cailb.is_visial = true;
		if (!is_handle_mode)
		{
			out_genshin_minimap_cailb.rect_minimap_cailb = cv::Rect(rect_origin.tl() + minimap_cailb_match_maxLoc, split_minimap_cailb_template[3].size());
		}
		else
		{
			out_genshin_minimap_cailb.rect_minimap_cailb = cv::Rect(rect_origin.tl() + minimap_cailb_match_maxLoc, minimap_cailb_template_handle_mode.size());
		}
	}

}

inline void splite_minimap(const GenshinScreen& genshin_screen, GenshinMinimap& out_genshin_minimap)
{
	auto& paimon_rect = genshin_screen.config.rect_paimon;
	auto& minimap_cailb_rect = genshin_screen.config.rect_minimap_cailb;

	if (paimon_rect.empty() || minimap_cailb_rect.empty()) return;

	// 1. 以paimon为基准，计算出minimap的左边界
	auto minimap_left = paimon_rect.x + paimon_rect.width / 2;
	// 2. 以minimap_cailb为基准，计算出minimap的右边界
	auto minimap_right = minimap_cailb_rect.x + minimap_cailb_rect.width / 2;
	// 3. 以paimon和minimap_cailb为基准，计算出minimap的上边界
	auto minimap_top = (paimon_rect.y + minimap_cailb_rect.y) / 2;
	// 4. 计算出minimap的宽度
	auto minimap_width = minimap_right - minimap_left;
	// 5. 计算出minimap的高度
	auto minimap_height = minimap_width;
	// 6. 计算出minimap的下边界	
	auto minimap_bottom = minimap_top + minimap_height;
	// 7. 计算出minimap的左上角坐标
	auto minimap_left_top = cv::Point(minimap_left, minimap_top);
	// 8. 计算出minimap的右下角坐标
	auto minimap_right_bottom = cv::Point(minimap_right, minimap_bottom);
	// 9. 计算出minimap的矩形区域
	auto minimap_rect = cv::Rect(minimap_left_top, minimap_right_bottom);
	// 10. 以minimap_rect为基准，计算出minimap的中心点
	auto minimap_center = cv::Point(minimap_rect.x + minimap_rect.width / 2, minimap_rect.y + minimap_rect.height / 2);

	out_genshin_minimap.img_minimap = genshin_screen.img_screen(minimap_rect);
	out_genshin_minimap.rect_minimap = minimap_rect;
	out_genshin_minimap.point_minimap_center = minimap_center;

	int Avatar_Rect_x = cvRound(minimap_width * 0.4);
	int Avatar_Rect_y = cvRound(minimap_height * 0.4);
	int Avatar_Rect_w = cvRound(minimap_width * 0.2);
	int Avatar_Rect_h = cvRound(minimap_height * 0.2);

	out_genshin_minimap.rect_avatar = cv::Rect(Avatar_Rect_x, Avatar_Rect_y, Avatar_Rect_w, Avatar_Rect_h);
	out_genshin_minimap.img_avatar = out_genshin_minimap.img_minimap(out_genshin_minimap.rect_avatar);
	
	int Viewer_Rect_x = cvRound(minimap_width * 0.2);
	int Viewer_Rect_y = cvRound(minimap_height * 0.2);
	int Viewer_Rect_w = cvRound(minimap_width * 0.6);
	int Viewer_Rect_h = cvRound(minimap_height * 0.6);

	out_genshin_minimap.rect_viewer = cv::Rect(Viewer_Rect_x, Viewer_Rect_y, Viewer_Rect_w, Viewer_Rect_h);
	out_genshin_minimap.img_viewer = out_genshin_minimap.img_minimap(out_genshin_minimap.rect_viewer);

	int Stars_Rect_x = cvRound(minimap_width * 0.165);
	int Stars_Rect_y = cvRound(minimap_height * 0.165);
	int Stars_Rect_w = cvRound(minimap_width * 0.67);
	int Stars_Rect_h = cvRound(minimap_height * 0.67);

	out_genshin_minimap.rect_stars = cv::Rect(Stars_Rect_x, Stars_Rect_y, Stars_Rect_w, Stars_Rect_h);
	out_genshin_minimap.img_stars = out_genshin_minimap.img_minimap(out_genshin_minimap.rect_stars);
}

inline double dis(cv::Point p)
{
	return sqrt(p.x * p.x + p.y * p.y);
}
inline std::vector<cv::Point2f> Vector2UnitVector(std::vector<cv::Point2f> pLis)
{
	double length = 1;
	std::vector<cv::Point2f> res;
	for (int i = 0; i < pLis.size(); i++)
	{
		length = sqrt(pLis[i].x * pLis[i].x + pLis[i].y * pLis[i].y);
		res.push_back(cv::Point2f((float)(pLis[i].x / length), (float)(pLis[i].y / length)));
	}
	return res;
}
inline double Line2Angle(cv::Point2f p)
{
	const double rad2degScale = 180 / CV_PI;
	double res = atan2(-p.y, p.x) * rad2degScale;
	res = res - 90; //从屏幕空间左侧水平线为0度转到竖直向上为0度
	return res;
}

inline void get_avatar_direction(const GenshinMinimap& genshin_minimap, GenshinAvatarDirection& out_genshin_direction)
{
	static cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
	static cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));

	if (genshin_minimap.img_avatar.empty()) return;

	cv::Mat giAvatarRef;

	cv::resize(genshin_minimap.img_avatar, giAvatarRef, cv::Size(), 2, 2);

	std::vector<cv::Mat> lis;
	cv::split(giAvatarRef, lis);

	cv::Mat gray_0;
	cv::Mat gray_1;
	cv::Mat gray_2;

	threshold(lis[0], gray_0, 240, 255, cv::THRESH_BINARY);
	threshold(lis[1], gray_1, 212, 255, cv::THRESH_BINARY);
	threshold(lis[2], gray_2, 25, 255, cv::THRESH_BINARY_INV);

	cv::Mat and_1_2;

	cv::bitwise_and(gray_1, gray_2, and_1_2, gray_0);
	cv::resize(and_1_2, and_1_2, cv::Size(), 2, 2, 3);
	cv::Canny(and_1_2, and_1_2, 20, 3 * 20, 3);
	cv::circle(and_1_2, cv::Point(cvCeil(and_1_2.cols / 2), cvCeil(and_1_2.rows / 2)), cvCeil(and_1_2.cols / 4.5), cv::Scalar(0, 0, 0), -1);

	dilate(and_1_2, and_1_2, dilate_element);
	erode(and_1_2, and_1_2, erode_element);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarcy;

	findContours(and_1_2, contours, hierarcy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	std::vector<cv::Rect> boundRect(contours.size());  //定义外接矩形集合

	cv::Point2f rect[4];

	std::vector<cv::Point2d> AvatarKeyPoint;
	double AvatarKeyPointLine[3] = { 0 };
	std::vector<cv::Point2f> AvatarKeyLine;
	cv::Point2f KeyLine;

	if (contours.size() != 3)
	{
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		//box[i] = minAreaRect(Mat(contours[i]));  //计算每个轮廓最小外接矩形
		boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
		AvatarKeyPoint.push_back(cv::Point(cvRound(boundRect[i].x + boundRect[i].width / 2), cvRound(boundRect[i].y + boundRect[i].height / 2)));
	}

	AvatarKeyPointLine[0] = dis(AvatarKeyPoint[2] - AvatarKeyPoint[1]);
	AvatarKeyPointLine[1] = dis(AvatarKeyPoint[2] - AvatarKeyPoint[0]);
	AvatarKeyPointLine[2] = dis(AvatarKeyPoint[1] - AvatarKeyPoint[0]);



	if (AvatarKeyPointLine[0] >= AvatarKeyPointLine[2] && AvatarKeyPointLine[1] >= AvatarKeyPointLine[2])
	{
		AvatarKeyLine.push_back(AvatarKeyPoint[2] - AvatarKeyPoint[1]);
		AvatarKeyLine.push_back(AvatarKeyPoint[2] - AvatarKeyPoint[0]);
	}
	if (AvatarKeyPointLine[0] >= AvatarKeyPointLine[1] && AvatarKeyPointLine[2] >= AvatarKeyPointLine[1])
	{
		AvatarKeyLine.push_back(AvatarKeyPoint[1] - AvatarKeyPoint[0]);
		AvatarKeyLine.push_back(AvatarKeyPoint[1] - AvatarKeyPoint[2]);
	}
	if (AvatarKeyPointLine[1] >= AvatarKeyPointLine[0] && AvatarKeyPointLine[2] >= AvatarKeyPointLine[0])
	{
		AvatarKeyLine.push_back(AvatarKeyPoint[0] - AvatarKeyPoint[1]);
		AvatarKeyLine.push_back(AvatarKeyPoint[0] - AvatarKeyPoint[2]);
	}

	AvatarKeyLine = Vector2UnitVector(AvatarKeyLine);
	KeyLine = AvatarKeyLine[0] + AvatarKeyLine[1];
	out_genshin_direction.angle = Line2Angle(KeyLine);

}

void get_avatar_position(const GenshinMinimap& genshin_minimap, GenshinAvatarPosition& out_genshin_position);


inline void get_viewer_direction(const GenshinMinimap& genshin_minimap, GenshinViewerDirection& out_genshin_direction)
{
	if (genshin_minimap.img_viewer.empty()) return;

	std::vector<cv::Mat>scr_channels;

	auto& giViewerRef = genshin_minimap.img_viewer;
	
	split(giViewerRef, scr_channels);

	cv::Mat Alpha = scr_channels[3];

	Alpha = 255.0 - Alpha;

	Alpha = Alpha * 2;

	cv::threshold(Alpha, Alpha, 150, 0, cv::THRESH_TOZERO_INV);
	cv::threshold(Alpha, Alpha, 50, 0, cv::THRESH_TOZERO);
	cv::threshold(Alpha, Alpha, 50, 255, cv::THRESH_BINARY);

	cv::circle(Alpha, cv::Point(Alpha.cols / 2, Alpha.rows / 2), static_cast<int>(std::min(Alpha.cols / 2, Alpha.rows / 2) * 1.21), cv::Scalar(0, 0, 0), static_cast<int>(std::min(Alpha.cols / 2, Alpha.rows / 2) * 0.42));
	cv::circle(Alpha, cv::Point(Alpha.cols / 2, Alpha.rows / 2), static_cast<int>(std::min(Alpha.cols / 2, Alpha.rows / 2) * 0.3), cv::Scalar(0, 0, 0), -1);


	cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
	cv::dilate(Alpha, Alpha, dilate_element);
	cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
	cv::erode(Alpha, Alpha, erode_element);

	erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
	cv::erode(Alpha, Alpha, erode_element);
	dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
	cv::dilate(Alpha, Alpha, dilate_element);


	//传入黑白图
	//根据白块部分计算视角中心坐标
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarcy;

	cv::findContours(Alpha, contours, hierarcy, 0, 1);

	std::vector<cv::Rect> boundRect(contours.size());  //定义外接矩形集合

	if (contours.size() == 0)
	{
		return;
	}
	
	int maxBlack = 0;
	int maxId = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > maxBlack)
		{
			maxBlack = static_cast<int>(contours[i].size());
			maxId = i;
		}
		boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
	}

	cv::Point pos_viewer_center = cv::Point(boundRect[maxId].x + boundRect[maxId].width / 2, boundRect[maxId].y + boundRect[maxId].height / 2);

	pos_viewer_center = pos_viewer_center - cv::Point(giViewerRef.cols / 2, giViewerRef.rows / 2);

	out_genshin_direction.angle = Line2Angle(pos_viewer_center);
}


inline void get_minimap_direction(const GenshinMinimap& genshin_minimap, GenshinMinimapDirection& out_genshin_direction)
{
	static cv::Mat N_template_mask;
	static cv::Point2d N_template_center;
	static cv::Mat mask_circle;
	static bool is_first = true;
	if (is_first)
	{
		std::vector<cv::Mat> N_template_channels;
		N_template_mask = GenshinImpact_TianLi_Resource::GetInstance()->GIMINIMAP_N;
		split(N_template_mask, N_template_channels);
		N_template_mask = N_template_channels[3];
		N_template_center = cv::Point2d(N_template_mask.cols / 2.0, N_template_mask.rows / 2.0);
		is_first = false;
	}

	
	auto& giMinimapRef = genshin_minimap.img_minimap;

	if (giMinimapRef.empty()) return;

	std::vector<cv::Mat> split_gi_frame;
	split(giMinimapRef, split_gi_frame);
	
	cv::Mat giMinimapRef_A = split_gi_frame[3];

	cv::Point2d minimap_center = cv::Point(giMinimapRef.cols / 2, giMinimapRef.rows / 2);
	int circle_mask_r0 = static_cast<int>(std::min(giMinimapRef.cols, giMinimapRef.rows) * 0.367);
	int circle_mask_r1 = static_cast<int>(std::min(giMinimapRef.cols, giMinimapRef.rows) * 0.442);
	int circle_mask_r2 = static_cast<int>(std::min(giMinimapRef.cols, giMinimapRef.rows) * 0.432);
	int circle_mask_d0 = static_cast<int>(std::min(giMinimapRef.cols, giMinimapRef.rows) * 0.035);

	// 边缘提取
	cv::Mat canny;
	cv::Canny(giMinimapRef_A, canny, 80, 200);

	// 在边缘图中心绘制半径为80的黑色遮罩
	cv::circle(canny, minimap_center, circle_mask_r0, cv::Scalar(0), -1);
	
	if (mask_circle.size() != giMinimapRef.size())
	{
		mask_circle = cv::Mat::zeros(giMinimapRef.size(), CV_8UC1);
		cv::circle(mask_circle, minimap_center, circle_mask_r1, cv::Scalar(255), -1);
		cv::circle(mask_circle, minimap_center, static_cast<int>(circle_mask_r0), cv::Scalar(0), -1);
	}

	cv::Mat mask_canny;
	canny.copyTo(mask_canny, mask_circle);
	// 进行一次膨胀操作
	cv::dilate(mask_canny, mask_canny, cv::Mat(), cv::Point(-1, -1), 1);

	// 83-194
	// 阈值分割提取出【83，194】的色块
	cv::Mat test_roi_gray;
	cv::Mat test_roi_binary;
	cv::Mat test_roi_binary_inv;
	cv::Mat test_roi_binary_final;
	cv::Mat mask_test_roi_binary_final;
	cv::Mat canny_mask_binary_final;
	cv::Mat sum_mask;

	giMinimapRef_A.copyTo(test_roi_gray);
	
	cv::threshold(test_roi_gray, test_roi_binary, 83, 255, cv::THRESH_BINARY);

	cv::circle(test_roi_binary, minimap_center, 86, cv::Scalar(255), 5);

	cv::threshold(test_roi_gray, test_roi_binary_inv, 194, 255, cv::THRESH_BINARY_INV);
	
	cv::circle(test_roi_binary_inv, minimap_center, circle_mask_r0, cv::Scalar(0), -1);

	cv::bitwise_and(test_roi_binary, test_roi_binary_inv, test_roi_binary_final);

	test_roi_binary_final.copyTo(mask_test_roi_binary_final, mask_circle);

	cv::bitwise_and(mask_canny, mask_test_roi_binary_final, canny_mask_binary_final);

	sum_mask = mask_test_roi_binary_final + mask_canny;
	// 改为与运算
	cv::bitwise_and(mask_test_roi_binary_final, mask_canny, sum_mask);

	cv::circle(sum_mask, minimap_center, circle_mask_r2, cv::Scalar(0), circle_mask_d0);

	// 做一次膨胀操作 半径15
	cv::Mat sum_mask_dilate;
	cv::dilate(sum_mask, sum_mask_dilate, cv::Mat(), cv::Point(-1, -1), 15);
	// 做一次腐蚀操作 半径10
	cv::Mat sum_mask_erode;
	cv::erode(sum_mask_dilate, sum_mask_erode, cv::Mat(), cv::Point(-1, -1), 10);

	// test_roi
			// 搜索区域为test_roi中以sum_mask_erode为遮罩的部分
	cv::Mat test_roi_mask;
	giMinimapRef.copyTo(test_roi_mask, sum_mask_erode);
	

	// 拆分被匹配图片，只要它的透明通道 test_roi_mask
	std::vector<cv::Mat> test_roi_mask_channels;
	cv::split(test_roi_mask, test_roi_mask_channels);
	cv::Mat test_roi_mask_mask = test_roi_mask_channels[3];

	// 查找色块轮廓
	std::vector<std::vector<cv::Point>> N_contours;
	cv::findContours(sum_mask, N_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	
	if (N_contours.size() == 0)
	{
		return;
	}

	std::vector<std::pair<double, double>> angle_maxVal_list;
	// 遍历每一个轮廓，根据连线的角度旋转模板，进行匹配
	for (int i = 0; i < N_contours.size(); i++)
	{
		cv::Moments mu = cv::moments(N_contours[i]);
		cv::Point2d mc = cv::Point2d(mu.m10 / mu.m00, mu.m01 / mu.m00);
		double angle = atan2(mc.y - minimap_center.y, mc.x - minimap_center.x) * 180 / CV_PI + 90;
		// 根据角度旋转模板图片
		cv::Mat N_template_rotate;
		cv::Mat rot = cv::getRotationMatrix2D(N_template_center, -angle, 1.0);
		cv::warpAffine(N_template_mask, N_template_rotate, rot, N_template_mask.size(), 1, 0, cv::Scalar(0));

		// 根据200作为阈值二值化出匹配模板掩膜, 大于200的都变成0
		cv::Mat N_template_rotate_binary;
		cv::threshold(N_template_rotate, N_template_rotate_binary, 1, 255, cv::THRESH_BINARY);
		cv::Mat N_template_rotate_binary2 = N_template_rotate_binary * 255;
		
		// 匹配模板
		cv::Mat result;
		cv::matchTemplate(test_roi_mask_mask, N_template_rotate, result, cv::TM_CCOEFF_NORMED, N_template_rotate_binary);
		// 剔除结果中的inf
		cv::Mat result2;
		result.copyTo(result2);
		for (int i = 0; i < result2.rows; i++)
		{
			for (int j = 0; j < result2.cols; j++)
			{
				if (result2.at<float>(i, j) >1)
				{
					result2.at<float>(i, j) = 0;
				}
			}
		}
		result = result2;
		// 查找最大值和最小值
		double minVal, maxVal;
		cv::Point minLoc, maxLoc;
		cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

		angle_maxVal_list.push_back({ angle,maxVal });
	}
	
	if (angle_maxVal_list.size() > 1)
	{
		// 从匹配结果中找到最大值
		double maxVal = 0;
		double maxVal_angle = 0;
		for (int i = 0; i < angle_maxVal_list.size(); i++)
		{
			if (angle_maxVal_list[i].second > maxVal)
			{
				maxVal = angle_maxVal_list[i].second;
				maxVal_angle = angle_maxVal_list[i].first;
			}
		}
		out_genshin_direction.angle = maxVal_angle;
	}
	else
	{
		out_genshin_direction.angle = angle_maxVal_list[0].first;
	}
	
}

inline void get_stars(const GenshinMinimap& genshin_minimap, GenshinStars& out_genshin_stars)
{
	static cv::Mat star_template;
	static cv::Point2d star_template_center;
	static bool is_first = true;
	if (is_first)
	{
		// 单通道图片
		std::vector<cv::Mat> star_template_channels;
		star_template = GenshinImpact_TianLi_Resource::GetInstance()->GISTAR;
		cv::split(star_template, star_template_channels);
		star_template = star_template_channels[3];
		
		star_template_center = cv::Point2d(star_template.cols / 2, star_template.rows / 2);
		is_first = false;
	}
	
	int MAXLOOP = 0;
	bool isLoopMatch = false;
	
	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	
	std::vector<cv::Point2d> pos;
	
	double scale = 1.3;

	
	auto& giMinimapRef = genshin_minimap.img_stars;
	if (giMinimapRef.empty()) return;
	
	cv::Point2d minimap_center = cv::Point(giMinimapRef.cols / 2, giMinimapRef.rows / 2);
	cv::Mat minimap_bary;
	
	//一个bug 未开游戏而先开应用，开游戏时触发
	cv::cvtColor(giMinimapRef,minimap_bary, cv::COLOR_RGBA2GRAY);

	bool is_star_visible;
	
	cv::Mat result;
	
	matchTemplate(star_template, minimap_bary, result, cv::TM_CCOEFF_NORMED);
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	
	if (maxVal < 0.66)
	{
		is_star_visible = false;
	}
	else
	{
		isLoopMatch = true;
		is_star_visible = true;
		pos.push_back(cv::Point2d(maxLoc) -minimap_center +star_template_center);
	}

	while (isLoopMatch)
	{
		minimap_bary(cv::Rect(maxLoc.x, maxLoc.y, star_template.cols, star_template.rows)) = cv::Scalar(0, 0, 0);
		matchTemplate(star_template, minimap_bary, result, cv::TM_CCOEFF_NORMED);
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
		if (maxVal < 0.66)
		{
			isLoopMatch = false;
		}
		else
		{
			pos.push_back(cv::Point2d(maxLoc) - minimap_center + star_template_center);
		}

		MAXLOOP > 10 ? isLoopMatch = false : MAXLOOP++;
	}

	if (is_star_visible == true)
	{
		return;
	}
}

inline void get_tagflags(const GenshinMinimap& genshin_minimap, GenshinTagflags& out_genshin_tagflags)
{
	
}

class SurfMatch
{
	cv::Mat _mapMat;
	cv::Mat _minMapMat;
	cv::Mat _minMapLastMat;

	double ratio_thresh = 0.66;
	double render_map_scale = 1.3;//1.3;
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

	void setMap(cv::Mat gi_map);
	void setMinMap(cv::Mat minMapMat);

	void Init();
	void Init(std::vector<cv::KeyPoint>& gi_map_keypoints, cv::Mat& gi_map_descriptors);
	void match();

	cv::Point2d match_continuity(bool& calc_continuity_is_faile);
	cv::Point2d match_continuity_on_city(bool& calc_continuity_is_faile);
	cv::Point2d match_continuity_not_on_city(bool& calc_continuity_is_faile);

	cv::Point2d match_no_continuity(bool& calc_is_faile);

	cv::Point2d SURFMatch(cv::Mat minMapMat);
	cv::Point2d getLocalPos();
	bool getIsContinuity();
private:
	double dis(cv::Point2d& p);
	cv::Point2d SPC(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy);
	//Point SPC2(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	double var(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy);
};

void draw_good_matches(cv::Mat& img_scene, std::vector<cv::KeyPoint> keypoint_scene, cv::Mat& img_object, std::vector<cv::KeyPoint> keypoint_object, std::vector<cv::DMatch>& good_matches);

void calc_good_matches(cv::Mat& img_scene, std::vector<cv::KeyPoint> keypoint_scene, cv::Mat& img_object, std::vector<cv::KeyPoint> keypoint_object, std::vector<std::vector<cv::DMatch>>& KNN_m, double ratio_thresh, double render_map_scale, std::vector<double>& lisx, std::vector<double>& lisy, double& sumx, double& sumy);