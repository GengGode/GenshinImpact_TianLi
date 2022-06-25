#include "pch.h"

#include "..\GenshinImpact_TianLi_Resource\GenshinImpact_TianLi_Resource.h"
#pragma comment(lib,"GenshinImpact_TianLi_Resource.lib")

TEST(TestCaseName, TestName) {
	HBITMAP map = TianLi::LoadBitmap_GIMAP();
	EXPECT_NE(map, nullptr);
}
