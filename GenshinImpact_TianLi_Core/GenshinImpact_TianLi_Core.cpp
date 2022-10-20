#include "pch.h"
#include "GenshinImpact_TianLi_Core.h"

#include "..\GenshinImpact_TianLi_Resource\GenshinImpact_TianLi_Resource.h"
#pragma comment(lib,"GenshinImpact_TianLi_Resource.lib")

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

#include "..\GenshinImpact_TianLi_Track\GenshinImpact_TianLi_Track.h"
#pragma comment(lib,"GenshinImpact_TianLi_Track.lib")

#include "..\GenshinImpact_TianLi_Map\GenshinImpact_TianLi_Map.h"
#pragma comment(lib,"GenshinImpact_TianLi_Map.lib")

GenshinImpact_TianLi_Core::GenshinImpact_TianLi_Core()
{
	resource = GenshinImpact_TianLi_Resource::GetInstance();
	
	sqlite = new GenshinImpact_TianLi_Sqlite();
	track = new GenshinImpact_TianLi_Track();
	map = new GenshinImpact_TianLi_Map();
}

GenshinImpact_TianLi_Core::~GenshinImpact_TianLi_Core()
{
	delete resource;
	delete sqlite;
	delete track;
	delete map;
}

GenshinImpact_TianLi_Core& GenshinImpact_TianLi_Core::GetInstance()
{
	static GenshinImpact_TianLi_Core instance;
	return instance;
}
