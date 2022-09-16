#pragma once
#ifndef _LIB
#ifdef GENSHINIMPACTTIANLITRACK_EXPORTS
#define GENSHINIMPACTTIANLITRACK_API __declspec(dllexport)
#else
//#define GENSHINIMPACTTIANLITRACK_API __declspec(dllimport)
#define GENSHINIMPACTTIANLITRACK_API /* No thing */
#endif

#define APICALL __stdcall
#ifndef DLLAPI
#define DLLAPI GENSHINIMPACTTIANLITRACK_API
#endif // DLLAPI

#else
#ifndef DLLAPI
#define DLLAPI
#endif // DLLAPI
#endif


enum GiMapType
{
	提瓦特,
	渊下宫,
	地下层岩,
};
class TrackManage;

namespace TianLi::Track
{
	struct TrackResult;
	struct TrackConfig;
}

class DLLAPI GenshinImpact_TianLi_Track
{
	TrackManage* track_manage = nullptr;
public:
	GenshinImpact_TianLi_Track();
	~GenshinImpact_TianLi_Track();
public:
	void StartServer();
	void StopServer();
	TianLi::Track::TrackResult& GetResult2();
	void SetConfig(TianLi::Track::TrackConfig &config);
};

