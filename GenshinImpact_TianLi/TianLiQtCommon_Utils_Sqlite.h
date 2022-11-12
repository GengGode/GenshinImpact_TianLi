#pragma once
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <tuple>

class GenshinImpact_TianLi_Sqlite;
namespace TianLi::Utils::Sqlite
{
	std::map<std::string, int> get_area_map(GenshinImpact_TianLi_Sqlite* sqlite);	
}
