#include "pch.h"

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

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
	//for (int it = 0; it < VecCountry.size; it++)
	//{
	//	std::cout << VecCountry[it] << "\n";
	//}
	
	TextVector VecType;
	//genshinImpact_TianLi_Sqlite.ReadType(VecCountry[0], VecType);
	genshinImpact_TianLi_Sqlite.ReadType(NULL, VecType);
	
	EXPECT_EQ(VecType.size, 9);
	//for (int it = 0; it < VecType.size; it++)
	//{
	//	std::cout << VecType[it] << "\n";
	//}

	
	TextVector VecItem;
	//genshinImpact_TianLi_Sqlite.ReadItem(VecCountry[0], VecType[0], VecItem);
	genshinImpact_TianLi_Sqlite.ReadItem(NULL,NULL, VecItem);
	
	EXPECT_EQ(VecItem.size, 432);

	for (int it = 0; it < VecItem.size; it++)
	{
		std::cout << VecItem[it] << "\n";
	}

	ItemsVector VecObjects;
	//genshinImpact_TianLi_Sqlite.ReadItem(VecCountry[0], VecType[0], VecItem);
	genshinImpact_TianLi_Sqlite.ReadItems(NULL, NULL, VecItem[2], VecObjects);
	
	std::cout << VecItem[1] << "\n";
	
	EXPECT_EQ(VecObjects.size, 432);

	for (int it = 0; it < VecObjects.size; it++)
	{
		std::cout << VecObjects[it].name <<" "<< VecObjects[it].msg << "\n";
	}
	

	
}