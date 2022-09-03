#include "pch.h"
#include "GenshinImpact_TianLi_Core.h"

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

#include "..\GenshinImpact_TianLi_Resource\GenshinImpact_TianLi_Resource.h"
#pragma comment(lib,"GenshinImpact_TianLi_Resource.lib")

#include "..\GenshinImpact_TianLi_Track\GenshinImpact_TianLi_Track.h"
#pragma comment(lib,"GenshinImpact_TianLi_Track.lib")

GenshinImpact_TianLi_Core::GenshinImpact_TianLi_Core()
{
	sqlite = new GenshinImpact_TianLi_Sqlite();
	resource = GenshinImpact_TianLi_Resource::GetInstance();
	track = new GenshinImpact_TianLi_Track();
}

GenshinImpact_TianLi_Core::~GenshinImpact_TianLi_Core()
{
	delete sqlite;
	delete resource;
	delete track;
}

GenshinImpact_TianLi_Core& GenshinImpact_TianLi_Core::GetInstance()
{
	static GenshinImpact_TianLi_Core instance;
	return instance;
}
