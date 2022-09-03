#include "pch.h"

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

TEST(SqliteRead, SqliteReadCountry)
{
	GenshinImpact_TianLi_Sqlite genshinImpact_TianLi_Sqlite;
	genshinImpact_TianLi_Sqlite.OpenSqlite("KongYingJiuGuanData.sqlite");

	TextVector VecCountry;
	genshinImpact_TianLi_Sqlite.ReadCountry(VecCountry);
	
	EXPECT_EQ(VecCountry.size, 11);
	
	TextVector VecType;
	genshinImpact_TianLi_Sqlite.ReadType(VecCountry[0], VecType);
	
	EXPECT_EQ(VecType.size, 7);

	TextVector VecItem;
	genshinImpact_TianLi_Sqlite.ReadItem(VecCountry[0], VecType[0], VecItem);
	
	EXPECT_EQ(VecItem.size, 21);
	
	
}