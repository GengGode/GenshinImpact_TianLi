#include "pch.h"
#include "GenshinImpact_TianLi_Data.h"

#include "../GenshinImpact_TianLi_Sqlite/GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

GenshinImpact_TianLi_Data::GenshinImpact_TianLi_Data()
{
}

GenshinImpact_TianLi_Data::~GenshinImpact_TianLi_Data()
{
}

void GenshinImpact_TianLi_Data::init(GenshinImpact_TianLi_Sqlite* sqlite)
{
	init_area(sqlite);
	init_type(sqlite);
	init_item(sqlite);
}

void GenshinImpact_TianLi_Data::init_area(GenshinImpact_TianLi_Sqlite* sqlite)
{
	auto area = get_area_group_map(sqlite);
	for (auto& [tag_parent, tag_childs] : area)
	{
		auto parent_id = tag_parent.first.first;
		auto parent_name = tag_parent.first.second;
		std::vector<std::string> childs;
		for (auto& tag_child : tag_childs)
		{
			auto child_id = tag_child.first.first;
			auto child_name = tag_child.first.second;
			childs.push_back(child_name);
		}
		area_group.push_back({ parent_name, childs });
	}
}
void GenshinImpact_TianLi_Data::init_type(GenshinImpact_TianLi_Sqlite* sqlite)
{
	auto type = get_type_group_map(sqlite);
	for (auto& [tag_parent, tag_childs] : type)
	{
		auto parent_id = tag_parent.first.first;
		auto parent_name = tag_parent.first.second;
		std::vector<std::string> childs;
		for (auto& tag_child : tag_childs)
		{
			auto child_id = tag_child.first.first;
			auto child_name = tag_child.first.second;
			childs.push_back(child_name);
		}
		type_group.push_back({ parent_name, childs });
	}
}

void GenshinImpact_TianLi_Data::init_item(GenshinImpact_TianLi_Sqlite* sqlite)
{
	auto item = get_item_group_map(sqlite);
	for (auto& [area_type_key, items] : item)
	{
		auto& [area, type] = area_type_key;
		auto& area_name = area.first.second;
		auto& type_name = type.first.second;
		std::vector<std::string> childs;
		for (auto& item_child : items)
		{
			auto child_name = item_child.first.second;
			childs.push_back(child_name);
		}
		item_group.insert({ {area_name,type_name}, childs });
	}
}
