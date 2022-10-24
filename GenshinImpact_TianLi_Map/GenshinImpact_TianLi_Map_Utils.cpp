#include "pch.h"
#include "GenshinImpact_TianLi_Map_Utils.h"

cv::Mat TianLi::Map::Utils::get_view_map_overlay(const cv::Mat& GIMAP_OVERLAY, cv::Rect& viewer_rect)
{
	cv::Mat map_rect_overlay = GIMAP_OVERLAY(viewer_rect);
	return map_rect_overlay;
}


cv::Mat TianLi::Map::Utils::get_view_map(const cv::Mat& GIMAP, cv::Size view_size, cv::Point2d view_center, double view_map_scale, cv::Rect& map_rect)
{
	static cv::Mat viewMap;
	static cv::Rect viewMapRect;
	//static cv::Point2d viewMapCenter;

	static int mapSizeWidth = GIMAP.size().width;
	static int mapSizeHeight = GIMAP.size().height;
	const static cv::Point2d originGIMAP(1428, 2937);
	//需要能越过边界，否则大范围显示时无法保证角色箭头处于正确位置

	cv::Point minMapPoint = cv::Point(0, 0);

	cv::Size reMapSize = view_size;
	cv::Point2d reMapCenter = cv::Point(view_size.width / 2, view_size.height / 2) * view_map_scale;
	//cv::Point2d reAutoMapCenter = view_center;
	reMapSize.width = (reMapSize.width * view_map_scale);
	reMapSize.height = (reMapSize.height * view_map_scale);
	if (reMapSize.width > mapSizeWidth)
	{
		reMapSize.width = mapSizeWidth;
	}
	if (reMapSize.height > mapSizeHeight)
	{
		reMapSize.height = mapSizeHeight;
	}


	cv::Point2d LT = view_center - reMapCenter;
	cv::Point2d RB = view_center + cv::Point2d(reMapSize) - reMapCenter;

	minMapPoint = LT;

	if (LT.x < 0)
	{
		minMapPoint.x = 0;
	}
	if (LT.y < 0)
	{
		minMapPoint.y = 0;
	}
	if (RB.x > mapSizeWidth)
	{
		minMapPoint.x = mapSizeWidth - reMapSize.width;
	}
	if (RB.y > mapSizeHeight)
	{
		minMapPoint.y = mapSizeHeight - reMapSize.height;
	}
	viewMapRect = cv::Rect(minMapPoint, reMapSize);
	map_rect = viewMapRect;
	resize(GIMAP(viewMapRect), viewMap, view_size);
	return viewMap;
}
/// <summary>
/// 叠加 RGBA 图像，以 正片叠底 的方式
/// </summary>
/// <param name="src1">底图 RGBA</param>
/// <param name="src2">要叠加图像 RGBA</param>
/// <param name="dst">输出图像 RGBA</param>
/// <param name="alpha">要叠加图像的透明度 0-1</param>
void TianLi::Map::Utils::add_rgba_image(cv::Mat& src1, cv::Mat& src2, cv::Mat& dst, double alpha)
{
	//assert(src1.size() != src2.size());
	assert(src1.channels() == 4 && src2.channels() == 4);
	
	std::vector<cv::Mat> src1_split;
	std::vector<cv::Mat> src2_split;
	std::vector<cv::Mat> dst_merge;

	cv::split(src1, src1_split);
	cv::split(src2, src2_split);

	for (int i = 0; i < 3; i++)
	{
		auto dst_src1 = src1_split[i].mul(~src2_split[3], 1.0 / 255.0);
		auto dst_src2 = src2_split[i].mul(src2_split[3], alpha / 255.0);
		auto dst_channel = dst_src1 + dst_src2;
		dst_merge.push_back(dst_channel);
	}
	cv::Mat alpha_dst = src1_split[3] + src2_split[3] * alpha;
	dst_merge.push_back(alpha_dst);
	cv::merge(dst_merge, dst);
}
