#include "TianLiQtCommon_Utils_Sqlite.h"

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

namespace TianLi::Utils::Sqlite
{
	std::map<std::string, int> TianLi::Utils::Sqlite::get_area_map(GenshinImpact_TianLi_Sqlite* sqlite)
	{
		std::map<std::string, int> area_map;
		std::vector<std::tuple<std::string, int>> area_list = sqlite->GetAreaList();
		return std::map<std::string, int>();
	}
}