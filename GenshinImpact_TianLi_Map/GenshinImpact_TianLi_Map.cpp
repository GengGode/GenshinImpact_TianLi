#include "pch.h"
#include "GenshinImpact_TianLi_Map.h"

#include "GenshinImpact_TianLi_Map_Utils.h"

#include "..\GenshinImpact_TianLi_Core\GenshinImpact_TianLi_Core.h"
#pragma comment(lib,"GenshinImpact_TianLi_Core.lib")

GenshinImpact_TianLi_Map::GenshinImpact_TianLi_Map()
{
	core = new GenshinImpact_TianLi_Core();
}

GenshinImpact_TianLi_Map::~GenshinImpact_TianLi_Map()
{
	delete core;
}

GenshinImpact_TianLi_Map& GenshinImpact_TianLi_Map::GetInstance()
{
	static GenshinImpact_TianLi_Map instance;
	return instance;
}

/// <summary>
/// ��Ⱦ����͸����
/// </summary>
/// <param name="map"></param>
void GenshinImpact_TianLi_Map::render_overlay(cv::Mat& map)
{
	if (map_info.map_rect.area() == 0)
	{
		return;
	}
	auto map_rect_overlay = TianLi::Map::Utils::get_view_map_overlay(core->GetResource().GiMap_Overlay(), map_info.map_rect);
	cv::resize(map_rect_overlay, map_rect_overlay,map.size());
	
	TianLi::Map::Utils::add_rgba_image(map, map_rect_overlay, map,0.5);
}

void GenshinImpact_TianLi_Map::render_legend(cv::Mat& map)
{
	// ��ͼ���Ļ��ƻ��ι��
	cv::circle(map, cv::Point(map.cols / 2, map.rows / 2), 5, cv::Scalar(0, 128, 255, 128), 1, cv::LINE_AA);

	cv::Rect viewer_rect = cv::Rect(cv::Point(map_info.center_x, map_info.center_y),cv::Size(map_info.viewer_width, map_info.viewer_height));
	if (map_info.is_show_map)
	{
		map = TianLi::Map::Utils::get_view_map(core->GetResource().GiMap(), cv::Size(map_info.viewer_width, map_info.viewer_height), cv::Point(map_info.center_x, map_info.center_y), map_info.scale_form_gimap, map_info.map_rect);
	}
	else
	{
		// map = cv::Mat(map_info.viewer_width, map_info.viewer_height, CV_8UC4, cv::Scalar(255,255,255, 0));
		map = TianLi::Map::Utils::get_view_map(core->GetResource().GiMap(), cv::Size(map_info.viewer_width, map_info.viewer_height), cv::Point(map_info.center_x, map_info.center_y), map_info.scale_form_gimap, map_info.map_rect);
		map = TianLi::Map::Utils::get_view_map_overlay(core->GetResource().GiMap_Overlay(), map_info.map_rect).clone();
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
			// ȡ����
			if (legend & map_info.map_rect)
			{
				// ������map��Rect
				// TODO:
				cv::Rect r_img = cv::Rect(cv::Point(std::round((legend.x- img.cols/2.0) / map_info.scale_form_gimap), std::round((legend.y- img.rows/2.0)) / map_info.scale_form_gimap), img.size());
				
				// ȡ����
				cv::Rect r = (r_img & map_info.map_rect);
				
				// ��ȡ����ڵ�ͼͼƬ�ķ�Χ
				cv::Rect r1 = r - map_info.map_rect.tl();
				// ��ȡ���������ͼƬ�ķ�Χ
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
	// ��sql�в�ѯĿ���������и���map_info��viewer��Χ��������
	ItemsVector itemsItemsVector;
	// ���ظ������µ���Ʒ
	core->GetSqlite().ReadItems(country, type, item, itemsItemsVector);
	// �����ȡ���������ǿյ�
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
	// TODO: �ڴ˴����� return ���
	static TianLi::objects objs;
	return objs;
}