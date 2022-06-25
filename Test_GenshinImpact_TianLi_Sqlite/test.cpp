#include "pch.h"

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

TEST(SqliteRead, SqliteReadCountry)
{
	TextVector textVector;
	GenshinImpact_TianLi_Sqlite genshinImpact_TianLi_Sqlite;
	genshinImpact_TianLi_Sqlite.OpenSqlite("KongYingJiuGuanData.sqlite");
	genshinImpact_TianLi_Sqlite.ReadCountry(textVector);
	genshinImpact_TianLi_Sqlite.CloseSqlite();
	EXPECT_EQ(textVector.size, 11);
}