#include "pch.h"
#include "GenshinImpact_TianLi_Map.h"

#include "GenshinImpact_TianLi_Map_Utils.h"

#include "..\GenshinImpact_TianLi_Core\GenshinImpact_TianLi_Core.h"
#pragma comment(lib,"GenshinImpact_TianLi_Core.lib")

GenshinImpact_TianLi_Map::GenshinImpact_TianLi_Map()
{
}

GenshinImpact_TianLi_Map::~GenshinImpact_TianLi_Map()
{
}

//GenshinImpact_TianLi_Map* GenshinImpact_TianLi_Map::GetInstance()
//{
//	static GenshinImpact_TianLi_Map* instance = new GenshinImpact_TianLi_Map();
//	return instance;
//}

/// <summary>
/// äÖÈ¾¸²¸ÇÍ¸Ã÷²ã
/// </summary>
/// <param name="map"></param>
void GenshinImpact_TianLi_Map::render_overlay(cv::Mat& map)
{
	if (map_info.map_rect.area() == 0)
	{
		return;
	}
	auto map_rect_overlay = TianLi::Map::Utils::get_view_map_overlay(Core.GetResource().GiMap_Overlay(), map_info.map_rect);
	cv::resize(map_rect_overlay, map_rect_overlay,map.size());
	
	TianLi::Map::Utils::add_rgba_image(map, map_rect_overlay, map,0.5);
}

void GenshinImpact_TianLi_Map::render_legend(cv::Mat& map)
{
	// µØÍ¼ÖÐÐÄ»æÖÆ»·ÐÎ¹â±ê
	cv::circle(map, cv::Point(map.cols / 2, map.rows / 2), 5, cv::Scalar(0, 128, 255, 128), 1, cv::LINE_AA);

	cv::Rect viewer_rect = cv::Rect(cv::Point(map_info.center_x, map_info.center_y),cv::Size(map_info.viewer_width, map_info.viewer_height));
	if (map_info.is_show_map)
	{
		map = TianLi::Map::Utils::get_view_map(Core.GetResource().GiMap(), cv::Size(map_info.viewer_width, map_info.viewer_height), cv::Point(map_info.center_x, map_info.center_y), map_info.scale_form_gimap, map_info.map_rect);
	}
	else
	{
		// map = cv::Mat(map_info.viewer_width, map_info.viewer_height, CV_8UC4, cv::Scalar(255,255,255, 0));
		map = TianLi::Map::Utils::get_view_map(Core.GetResource().GiMap(), cv::Size(map_info.viewer_width, map_info.viewer_height), cv::Point(map_info.center_x, map_info.center_y), map_info.scale_form_gimap, map_info.map_rect);
		map = TianLi::Map::Utils::get_view_map_overlay(Core.GetResource().GiMap_Overlay(), map_info.map_rect).clone();
	}

	if (map_info.map_rect.area() == 0)
	{
		return;
	}
	
	if (map_info.is_overlay&& map_info.is_show_map)
	{
		render_overlay(map);
	}

	for (auto& info : badge_info.badge_block_list)
	{
		cv::Mat img = info.image;
		cv::resize(img, img, cv::Size(), 1.0/map_info.scale_form_gimap, 1.0/map_info.scale_form_gimap);
		for (auto& legend : info.badge_list)
		{
			// È¡½»¼¯
			if (legend & map_info.map_rect)
			{
				// »æÖÆÔÚmapÖÐRect
				cv::Rect r_img = cv::Rect(cv::Point(std::round(legend.x- img.cols/2.0), std::round(legend.y- img.rows/2.0)), img.size());
				
				// È¡½»¼¯
				cv::Rect r = (r_img & map_info.map_rect);
				
				// »ñÈ¡Ïà¶ÔÓÚµØÍ¼Í¼Æ¬µÄ·¶Î§
				cv::Rect r1 = r - map_info.map_rect.tl();
				// »ñÈ¡Ïà¶ÔÓÚÇø¿éÍ¼Æ¬µÄ·¶Î§
				cv::Rect r2 = r - r_img.tl();
				//img(r2).copyTo(map(r1));
				try {

					auto map_roi = map(r1);
					auto img_roi = img(r2);

					TianLi::Map::Utils::add_rgba_image(map_roi, img_roi, map_roi);
				}
				catch (...)
				{
					//std::cout << "error" << std::endl;
				}
			}
		}
	}
}

BadgeInfo GenshinImpact_TianLi_Map::search(const char* country, const char* type, const char* item)
{
	BadgeInfo result_badge_info;
	// ´ÓsqlÖÐ²éÑ¯Ä¿±ê²úÎï£¬²¢´ÓÖÐ¸ù¾Ýmap_infoµÄviewer·¶Î§½øÐÐÌô¼ð
	ItemsVector itemsItemsVector;
	// ¼ÓÔØ¸ÃÖÖÀàÏÂµÄÎïÆ·
	Core.GetSqlite().ReadItems(country, type, item, itemsItemsVector);
	// Èç¹û¶ÁÈ¡µ½µÄÊý¾ÝÊÇ¿ÕµÄ
	if (itemsItemsVector.size == 0)
	{
		return result_badge_info;
	}
	BadgeInfo::BadgeBlock badge_block;
	for (int i = 0; i < itemsItemsVector.size; i++)
	{
		auto item = itemsItemsVector[i];
		
		BadgeInfo::BadgeBlock::Badge badge;
		badge.x = item.x;
		badge.y = item.y;
		badge.z = item.z;
		
		
		if (badge & map_info.map_rect)
		{
			badge_block.badge_list.push_back(badge);
		}
	}
	result_badge_info.badge_block_list.push_back(badge_block);

	return result_badge_info;
}

TianLi::objects& GenshinImpact_TianLi_Map::search(const char* name, double x, double y, double r)
{
	// TODO: ÔÚ´Ë´¦²åÈë return Óï¾ä
	static TianLi::objects objs;
	return objs;
}
cv::Mat GenshinImpact_TianLi_Map::get_image_tag(const std::string& area, const std::string& type, const std::string& item, const std::string& object)
{
	static cv::Mat empty_object_image;//itemsItemsVector[0].image;
	static bool is_frist = true;
	if (is_frist)
	{
		empty_object_image = cv::Mat(cv::Size(32, 32), CV_8UC4, cv::Scalar(200, 200, 200, 0));
		// ä»¥42ä¸ºç›´å¾„ç»˜åˆ¶å®½åº¦ä¸º3çš„ç™½è‰²åœ†çŽ¯
		cv::circle(empty_object_image, cv::Point(16, 16), 12, cv::Scalar(255, 255, 255, 128), 3, cv::LINE_AA);

		is_frist = false;
	}
	
	cv::Mat image_mat;
	unsigned char* image_buffer = nullptr;
	int size = 0;
	if (!type.empty() && object.empty())
	{
		Core.GetSqlite().GetTypeImage(type.c_str(), image_buffer, size);
	}
	if (type.empty() && !object.empty())
	{
		Core.GetSqlite().GetItemImage(object.c_str(), image_buffer, size);
	}
	if (image_buffer != nullptr)
	{
		image_mat = cv::imdecode(cv::Mat(1, size, CV_8UC1, image_buffer), cv::IMREAD_UNCHANGED);
		if (image_mat.empty())
		{
			image_mat = empty_object_image;
		}
		//if (image_mat.rows >= 32 || image_mat.cols >= 32)
		//{
		//	// image_mat = empty_object_image;
		//	cv::resize(image_mat, image_mat, cv::Size(32, 32));
		//}
	}
	else
	{
		image_mat = empty_object_image;
	}
	return image_mat;
}
