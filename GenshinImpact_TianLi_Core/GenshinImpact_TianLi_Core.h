#pragma once
#ifndef _LIB
#ifdef GENSHINIMPACTTIANLICORE_EXPORTS
#define GENSHINIMPACTTIANLICORE_API __declspec(dllexport)
#else
//#define GENSHINIMPACTTIANLICORE_API __declspec(dllimport)
#define GENSHINIMPACTTIANLICORE_API /* No thing */
#endif

#define APICALL __stdcall
#ifndef DLLAPI
#define DLLAPI GENSHINIMPACTTIANLICORE_API
#endif // DLLAPI

#else
#ifndef DLLAPI
#define DLLAPI
#endif // DLLAPI
#endif

#include "..\GenshinImpact_TianLi_Resource\GenshinImpact_TianLi_Resource.h"
#pragma comment(lib,"GenshinImpact_TianLi_Resource.lib")

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

#include "..\GenshinImpact_TianLi_Track\GenshinImpact_TianLi_Track.h"
#pragma comment(lib,"GenshinImpact_TianLi_Track.lib")

#include "..\GenshinImpact_TianLi_Map\GenshinImpact_TianLi_Map.h"
#pragma comment(lib,"GenshinImpact_TianLi_Map.lib")

class GenshinImpact_TianLi_Sqlite;
class GenshinImpact_TianLi_Resource;
class GenshinImpact_TianLi_Track;
class GenshinImpact_TianLi_Map;

class DLLAPI GenshinImpact_TianLi_Core
{
	GenshinImpact_TianLi_Resource* resource;
	GenshinImpact_TianLi_Sqlite* sqlite;
	GenshinImpact_TianLi_Track* track;
	GenshinImpact_TianLi_Map* map;
	
	GenshinImpact_TianLi_Core();
public:
	~GenshinImpact_TianLi_Core();
	static GenshinImpact_TianLi_Core& GetInstance();
public:
	GenshinImpact_TianLi_Resource& GetResource() { return *resource; }
	GenshinImpact_TianLi_Sqlite& GetSqlite() { return *sqlite; }
	GenshinImpact_TianLi_Track& GetTrack() { return *track; }
	GenshinImpact_TianLi_Map& GetMap() { return *map; }
	
	
};

#define Core GenshinImpact_TianLi_Core::GetInstance()



