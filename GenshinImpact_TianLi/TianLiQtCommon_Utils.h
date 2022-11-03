#pragma once
/*
#include <QObject>

class TianLiQtCommon_Utils : public QObject
{
	Q_OBJECT
private:
	TianLiQtCommon_Utils(QObject* parent = Q_NULLPTR);

public:
	static TianLiQtCommon_Utils& GetInstance();

	TianLiQtCommon_Utils(const TianLiQtCommon_Utils&) = delete;
	TianLiQtCommon_Utils(TianLiQtCommon_Utils&&) = delete;
	~TianLiQtCommon_Utils();
public:
	
};

#define TianLi_Utils TianLiQtCommon_Utils::GetInstance() 
*/
#include <QObject>
#include <opencv2/opencv.hpp>
#include <Windows.h>

namespace TianLi::Utils 
{
	
	void set_window_blur_bebind(HWND handle);
	
	// 九宫格扩展图片算法
	 QImage border_image(QImage image, int w, int h, int clip_top, int clip_right, int clip_bottom, int clip_left);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="GIMAP">原地图</param>
	/// <param name="view_size">输出图片大小</param>
	/// <param name="view__center">输入视口中心坐标</param>
	/// <param name="view__scale">输入地图缩放</param>
	/// <returns></returns>
	cv::Mat get_view_map(const cv::Mat& GIMAP, cv::Size view_size, cv::Point2d view_center, double view_map_scale, cv::Rect& map_rect);
	
	cv::Mat create_square_mask(int mask_width, int mask_height, double gradient_width);
	
	cv::Mat create_circular_mask(int mask_width, int mask_height, double gradient_width);

	QImage mat_2_qimage(cv::Mat const& src);
	
	cv::Mat qimage_2_mat(QImage const& src);
}

