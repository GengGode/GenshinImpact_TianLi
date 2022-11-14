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
