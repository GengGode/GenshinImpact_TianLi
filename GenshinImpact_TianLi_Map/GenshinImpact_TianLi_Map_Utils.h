#pragma once
#include <opencv2/opencv.hpp>
namespace TianLi::Map::Utils
{
	cv::Mat get_view_map_overlay(const cv::Mat& GIMAP_OVERLAY, cv::Rect& viewer_rect);


	cv::Mat get_view_map(const cv::Mat& GIMAP, cv::Size view_size, cv::Point2d view_center, double view_map_scale, cv::Rect& map_rect);
	
	// 四通道半透明图片叠加算法
	void add_rgba_image(cv::Mat& src1, cv::Mat& src2, cv::Mat& dst, double alpha = 1.0);
	
}