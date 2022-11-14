#pragma once
#ifndef _LIB
#ifdef GENSHINIMPACTTIANLIDATA_EXPORTS
#define GENSHINIMPACTTIANLIDATA_API __declspec(dllexport)
#else
#define GENSHINIMPACTTIANLIDATA_API __declspec(dllimport)
#endif // GENSHINIMPACTTIANLIDATA_EXPORTS

#define APICALL __stdcall
#ifndef DLLAPI
#define DLLAPI GENSHINIMPACTTIANLIDATA_API
#endif // DLLAPI

#else
#ifndef DLLAPI
#define DLLAPI
#endif // DLLAPI
#endif
#include <map>
#include <vector>
#include <string>
class GenshinImpact_TianLi_Sqlite;
class DLLAPI GenshinImpact_TianLi_Data
{
public:
	GenshinImpact_TianLi_Data();
	~GenshinImpact_TianLi_Data();

	//std::map<int, void*> area_data;
	//std::map<int, void*> type_data;
	//std::map<int, void*> item_data;

	std::vector<std::pair<std::string, std::vector<std::string>>> area_group;
	std::vector<std::pair<std::string, std::vector<std::string>>> type_group;
	std::map<std::pair<std::string, std::string>, std::vector<std::string>> item_group;

	void init(GenshinImpact_TianLi_Sqlite* sqlite);
	void init_area(GenshinImpact_TianLi_Sqlite* sqlite);
	void init_type(GenshinImpact_TianLi_Sqlite* sqlite);
	void init_item(GenshinImpact_TianLi_Sqlite* sqlite);

};