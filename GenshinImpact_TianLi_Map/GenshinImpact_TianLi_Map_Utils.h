#pragma once
#include <opencv2/opencv.hpp>
namespace TianLi::Map::Utils
{
	cv::Mat get_view_map_overlay(const cv::Mat& GIMAP_OVERLAY, cv::Rect& viewer_rect);
	
	// ��ͨ����͸��ͼƬ�����㷨
	void add_rgba_image(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst, double alpha = 1.0);
	
}