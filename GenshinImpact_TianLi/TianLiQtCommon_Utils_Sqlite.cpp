#include "TianLiQtCommon_Utils_Sqlite.h"

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

namespace TianLi::Utils::Sqlite
{
	std::map<std::string, int> TianLi::Utils::Sqlite::get_area_map(GenshinImpact_TianLi_Sqlite* sqlite)
	{
		std::map<std::string, int> area_map;
		//std::vector<std::tuple<std::string, int>> area_list = sqlite->GetAreaList();
		auto map = get_area_group_map(sqlite);
		for (auto& [tag, tag_child] : map)
		{
			// v2 area_map[key.name] = key.id;
			if (tag_child.size() == 0)
			{
				area_map[tag.first.second] = tag.first.first;
			}
			else
			{
				for (auto& tag : tag_child)
				{
					area_map[tag.first.second] = tag.first.first;
				}
			}
		}
		return area_map;
	}
}
