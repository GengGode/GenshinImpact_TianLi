#pragma once
#include <opencv2/opencv.hpp>
namespace TianLi::Map::Utils
{
	// 四通道半透明图片叠加算法
	void add_rgba_image(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst, double alpha = 1.0);
	
}