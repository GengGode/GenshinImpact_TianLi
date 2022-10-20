#include "pch.h"
#include "GenshinImpact_TianLi_Map.h"


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

void GenshinImpact_TianLi_Map::render_legend(cv::Mat& map)
{
	// ��ͼ���Ļ��ƻ��ι��
	cv::circle(map, cv::Point(map.cols / 2, map.rows / 2), 5, cv::Scalar(0, 128, 255, 128), 1, cv::LINE_AA);
	
	if (map_info.viewer_rect.area() == 0)
	{
		return;
	}
	
	for (auto& info : badge_info.badge_block_list)
	{
		cv::Mat img = info.image;
		for (auto& legend : info.badge_list)
		{
			if (legend & map_info.viewer_rect)
			{
				// ������map��Rect
				cv::Rect r_img = cv::Rect(cv::Point(std::round(legend.x- img.cols/2.0), std::round(legend.y- img.rows/2.0)), img.size());
				
				cv::Rect r = r_img & map_info.viewer_rect;
				
				// ��ȡ����ڵ�ͼͼƬ�ķ�Χ
				cv::Rect r1 = r - map_info.viewer_rect.tl();
				// ��ȡ���������ͼƬ�ķ�Χ
				cv::Rect r2 = r - r_img.tl();
				// ȡ����
				
				img(r2).copyTo(map(r1));
				
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
	Core.GetSqlite().ReadItems(country, type, item, itemsItemsVector);
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
		
		
		if (badge & map_info.viewer_rect)
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
