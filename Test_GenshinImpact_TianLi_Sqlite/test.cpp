#include "pch.h"

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")
//TEST(Std_TUPLE, Std)
//{
//	std::vector<std::tuple<int, std::string, std::string>> v1;
//	for (int i = 0; i < 10; i++)
//	{
//		v1.push_back(std::make_tuple(i, std::to_string(i), std::to_string(i)));
//	}
//
//	for(auto &[id,name,icon]:v1)
//	{
//		std::cout << id << " " << name << " " << icon << std::endl;
//	}
//}
#ifdef _DEF_V1
TEST(SqliteRead, Std_V1)
{
	GenshinImpact_TianLi_Sqlite sqlite;
	sqlite.OpenSqlite("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/Data.sqlite");

	system("chcp 65001");

	auto areas = v1::get_area_group_map(&sqlite);

	for (auto& area : areas)
	{
		auto& area_parent = area.first;
		auto& area_childs = area.second;
		std::cout << area_parent.first << " " << area_parent.second << std::endl;
		for (auto& area_child : area_childs)
		{
			auto& id = area_child.first;
			auto& name = area_child.second;
			std::cout << "\t" << id << " " << name << std::endl;
		}
	}

	auto types = v1::get_type_group_map(&sqlite);
	for (auto& type : types)
	{
		auto& type_parent = type.first;
		auto& type_childs = type.second;
		std::cout << type_parent.first.first << " " << type_parent.first.second << " " << type_parent.second << std::endl;
		for (auto& type_child : type_childs)
		{
			auto& id = type_child.first.first;
			auto& name = type_child.first.second;
			auto& type = type_child.second;
			std::cout << "\t" << id << " " << name << " " << type << std::endl;
		}
	}
}
#endif
#ifdef _DEF_V2
TEST(SqliteRead, Std_V2)
{
	GenshinImpact_TianLi_Sqlite sqlite;
	sqlite.OpenSqlite("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/Data.sqlite");

	system("chcp 65001");

	auto areas = v2::get_area_group_map(&sqlite);

	for (auto& area : areas)
	{
		auto& area_parent = area.first;
		auto& area_childs = area.second;
		std::cout << area_parent.name << " " << area_parent.id << std::endl;
		for (auto& area_child : area_childs)
		{
			auto& id = area_child.name;
			auto& name = area_child.id;
			std::cout << "\t" << id << " " << name << std::endl;
		}
	}

	auto types = v2::get_type_group_map(&sqlite);
	for (auto& type : types)
	{
		auto& type_parent = type.first;
		auto& type_childs = type.second;
		std::cout << type_parent.name << " " << type_parent.id << " " << type_parent.icon_tag << std::endl;
		for (auto& type_child : type_childs)
		{
			auto& id = type_child.name;
			auto& name = type_child.id;
			auto& type = type_child.icon_tag;
			std::cout << "\t" << id << " " << name << " " << type << std::endl;
		}
	}
}
#endif

TEST(SqliteRead, Std)
{
	GenshinImpact_TianLi_Sqlite sqlite;
	sqlite.OpenSqlite("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/Data.sqlite");

	system("chcp 65001");

	auto areas = get_area_group_map(&sqlite);

	for (auto& area : areas)
	{
		auto& area_parent = area.first;
		auto& area_childs = area.second;
		std::cout << area_parent.first.second << " " << area_parent.first.first << std::endl;
		for (auto& area_child : area_childs)
		{
			auto& id = area_child.first.second;
			auto& name = area_child.first.first;
			std::cout << "\t" << id << " " << name << std::endl;
		}
	}

	auto types = get_type_group_map(&sqlite);
	for (auto& type : types)
	{
		auto& type_parent = type.first;
		auto& type_childs = type.second;
		std::cout << type_parent.first.second << " " << type_parent.first.first << " " << type_parent.second << std::endl;
		for (auto& type_child : type_childs)
		{
			auto& id = type_child.first.second;
			auto& name = type_child.first.first;
			auto& type = type_child.second;
			std::cout << "\t" << id << " " << name << " " << type << std::endl;
		}
	}

	auto items = get_item_group_map(&sqlite);
	for (auto& [area_type_key, item] : items)
	{
		auto& [area, type] = area_type_key;
		int size = item.size();
		auto& area_name = area.first.second;
		auto& area_id = area.first.first;
		auto& type_name = type.first.second;
		auto& type_id = type.first.first;
		
		std::cout << area_name << " " << area_id << " > " << type_name << " " << type_id << " > count: " << size << std::endl;
	}
}

TEST(SqliteRead, SqliteReadCountry)
{
	//"C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/Data.sqlite"
	//"KongYingJiuGuanData.sqlite"
	GenshinImpact_TianLi_Sqlite genshinImpact_TianLi_Sqlite;
	genshinImpact_TianLi_Sqlite.OpenSqlite("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/Data.sqlite");

	system("chcp 65001");

	TextVector VecCountry;
	genshinImpact_TianLi_Sqlite.ReadCountry(VecCountry);
	
	EXPECT_EQ(VecCountry.size, 17);
	for (int it = 0; it < VecCountry.size; it++)
	{
		std::cout << VecCountry[it] << "\n";
		break;
	}
	
	TextVector VecType;
	//genshinImpact_TianLi_Sqlite.ReadType(VecCountry[0], VecType);
	genshinImpact_TianLi_Sqlite.ReadType(VecCountry[0], VecType);
	
	EXPECT_EQ(VecType.size, 10);
	for (int it = 0; it < VecType.size; it++)
	{
		std::cout << VecType[it] << " ";
		break;
	}
	std::cout << VecCountry[0] << "\n";

	
	TextVector VecItem;
	//genshinImpact_TianLi_Sqlite.ReadItem(VecCountry[0], VecType[0], VecItem);
	genshinImpact_TianLi_Sqlite.ReadItem(VecCountry[0], VecType[2], VecItem);
	
	//EXPECT_NE(VecItem.size, 0);

	for (int it = 0; it < VecItem.size; it++)
	{
		std::cout << VecItem[it] << " ";
		break;
	}
	if (VecItem.size == 0)return;
	
	std::cout <<  "\n";

	ItemsVector VecObjects;
	//genshinImpact_TianLi_Sqlite.ReadItem(VecCountry[0], VecType[0], VecItem);
	genshinImpact_TianLi_Sqlite.ReadItems(VecCountry[0], VecType[2], VecItem[1], VecObjects);
	
	std::cout << VecCountry[0] << "\n";
	std::cout << VecType[2] << "\n";
	std::cout << VecItem[1] << "\n";
	
	EXPECT_EQ(VecObjects.size, 66);

	for (int it = 0; it < VecObjects.size; it++)
	{
		std::cout << VecObjects[it].name <<" "<< VecObjects[it].msg << "\n";
		break;
	}
	

	
}