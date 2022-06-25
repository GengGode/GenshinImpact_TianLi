#include "pch.h"
#include "CppUnitTest.h"
#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestGenshinImpactTianLiSqlite
{
	TEST_CLASS(UnitTestGenshinImpactTianLiSqlite)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(1, 1);
			TextVector textVector;
			GenshinImpact_TianLi_Sqlite genshinImpact_TianLi_Sqlite;
			genshinImpact_TianLi_Sqlite.OpenSqlite("asd");
			genshinImpact_TianLi_Sqlite.ReadCountry(textVector);
			genshinImpact_TianLi_Sqlite.CloseSqlite();
			Assert::AreEqual(textVector.size, 0);
		}
	};
	void test() 
	{
		
	}
}
