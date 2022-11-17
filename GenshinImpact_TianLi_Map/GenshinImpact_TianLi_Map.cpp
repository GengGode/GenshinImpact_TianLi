#include "pch.h"
#include "GenshinImpact_TianLi_Map.h"

#include "GenshinImpact_TianLi_Map_Utils.h"

#include "..\GenshinImpact_TianLi_Core\GenshinImpact_TianLi_Core.h"
#pragma comment(lib,"GenshinImpact_TianLi_Core.lib")
#include "..\GenshinImpact_TianLi_Data\GenshinImpact_TianLi_Data.h"
#pragma comment(lib,"GenshinImpact_TianLi_Data.lib")


GenshinImpact_TianLi_Map::GenshinImpact_TianLi_Map()
{
	core = new GenshinImpact_TianLi_Core();
	data = new GenshinImpact_TianLi_Data();
	//================== init =========================
	data->init(&core->GetSqlite());
}

GenshinImpact_TianLi_Map::~GenshinImpact_TianLi_Map()
{
	delete core;
	delete data;
}

GenshinImpact_TianLi_Map& GenshinImpact_TianLi_Map::GetInstance()
{
	static GenshinImpact_TianLi_Map instance;
	return instance;
}

/// <summary>
/// 渲染覆盖透明层
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
	
	TianLi::Map::Utils::add_rgba_image(map, map_rect_overlay, map,1.0);
}

void GenshinImpact_TianLi_Map::render_legend(cv::Mat& map)
{
	// 地图中心绘制环形光标
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

	map_show_objects.types.clear();

	for (auto& [key,info] : badge_info.badge_block_list)
	{
		cv::Mat img = info.image;
		cv::resize(img, img, cv::Size(), 1.0/map_info.scale_form_gimap, 1.0/map_info.scale_form_gimap);

		BadgeInfo::BadgeBlock show_infos;

		for (auto& legend : info.badge_list)
		{
			// 取交集
			if (legend & map_info.map_rect)
			{
				// 绘制在map中Rect
				// TODO:
				cv::Rect r_img = cv::Rect(cv::Point(std::round((legend.x- img.cols/2.0) / map_info.scale_form_gimap), std::round((legend.y- img.rows/2.0)) / map_info.scale_form_gimap), img.size());
				
				// 取交集
				cv::Rect r = (r_img & map_info.map_rect);
				
				// 获取相对于地图图片的范围
				cv::Rect r1 = r - map_info.map_rect.tl();
				// 获取相对于区块图片的范围
				cv::Rect r2 = r - r_img.tl();
				//img(r2).copyTo(map(r1));
				try {

					auto map_roi = map(r1);
					auto img_roi = img(r2);

					TianLi::Map::Utils::add_rgba_image(map_roi, img_roi, map_roi);

					// 将显示在地图中的点位添加到show_object
					show_infos.badge_list.push_back(legend);

				}
				catch (...)
				{
					//std::cout << "error" << std::endl;
				}
			}
		}

		if (show_infos.badge_list.size() != 0)
		{
			show_infos.image = info.image;
			show_infos.name = info.name;
			map_show_objects.types.push_back(show_infos);
		}
	}
	// 地图中心绘制角色箭头
	// 1. 旋转角色箭头图片
	auto roation_angle = avatar_info.a;
	auto avatar = TianLi::Map::Utils::rotate_avatar(Core.GetResource().GIAVATAR, roation_angle, 1.0 / 1.3);//大地图与小地图之比
	auto avatar_rect = cv::Rect((avatar_info.x - avatar.cols / 2) * map_info.scale_form_gimap, avatar_info.y - avatar.rows / 2, avatar.cols, avatar.rows);
	auto avatar_rect_roi = avatar_rect & map_info.map_rect;
	if (avatar_rect_roi.area() > 0)
	{
		auto map_avatar_roi = map(avatar_rect_roi - map_info.map_rect.tl());
		auto avatar_roi = avatar(avatar_rect_roi - avatar_rect.tl());
		TianLi::Map::Utils::add_rgba_image(map_avatar_roi, avatar_roi, map_avatar_roi);
	}
	else
	{
		//// TODO:
		//// 获取点位相对朝向，计算其于边框的交点
		//// 1. 计算avatar_pos和map_pos的差值
		//auto diff_x = map_info.center_x - avatar_info.x;
		//auto diff_y = map_info.center_y - avatar_info.y;
		//
		//// 1.1 计算diff的角度，[0-90]，以及象限
		//double diff_abs_angle = std::atan2(diff_y, diff_x);
		//double diff_abs_k = static_cast<double>(diff_y/ diff_x);
		//// 
		//double rect_x = map.cols * (0.5 + 0.5 * std::sin(diff_abs_angle));
		//double rect_y = map.rows * (0.5 + 0.5 * std::cos(diff_abs_angle));
		////auto mat = map.clone();
		////cv::putText(mat, std::to_string(diff_abs_k), cv::Point(200, 200), 1, 2, cv::Scalar(128, 128, 128,128), -1);
		////LogInfo(std::to_string(diff_abs_k).c_str());
		////cv::imshow("map", map);
		////cv::waitKey(1);
		//cv::Rect vec_roi;


	}

}

BadgeInfo GenshinImpact_TianLi_Map::search(const char* country, const char* type, const char* item)
{
	BadgeInfo result_badge_info;
	// 从sql中查询目标产物，并从中根据map_info的viewer范围进行挑拣
	ItemsVector itemsItemsVector;
	// 加载该种类下的物品
	core->GetSqlite().ReadItems(country, type, item, itemsItemsVector);
	// 如果读取到的数据是空的
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
	std::tuple<std::string, std::string, std::string> key = { country, type, item };

	result_badge_info.badge_block_list.insert({ key,badge_block });

	return result_badge_info;
}

TianLi::objects& GenshinImpact_TianLi_Map::search(const char* name, double x, double y, double r)
{
	// TODO: 在此处插入 return 语句
	static TianLi::objects objs;
	return objs;
}
