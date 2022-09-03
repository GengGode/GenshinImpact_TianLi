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

#include <thread>
#include <mutex>
#include <condition_variable>

enum GiMapType
{
	提瓦特,
	渊下宫,
	地下层岩,
};

class VectorStar
{
public:
	struct StarPoint
	{
		float x;
		float y;
	};
public:
	int Count();
	void Append(float x, float y);
	StarPoint At(int id);
	
};

struct TrackResult
{
	int uid;
	float x;
	float y;
	float a;
	float a2;
	GiMapType map_type;
	bool is_continuity;
	bool is_paimon_visial;
	bool is_on_city;
	VectorStar vector_star;

	RECT gi_client_rect;
};
class TianLi_ThreadMatch;
class TianLi_GiState;
class DLLAPI GenshinImpact_TianLi_Track
{
	TianLi_ThreadMatch* thread_match;
	TianLi_GiState* gi_state;
	TrackResult track_result;
	
	std::thread* sync_thread=nullptr;
	
	std::thread* screen_thread;
	std::thread* track_thread;
	
	std::condition_variable *is_run_cv;
	std::mutex *is_run_m;
	bool is_run = false;
	bool is_exit = false;
	// 第一次运行时函数指针
	void(GenshinImpact_TianLi_Track::*run_func)(void) = &GenshinImpact_TianLi_Track::Init;
	
public:
	GenshinImpact_TianLi_Track();
	~GenshinImpact_TianLi_Track();
public:
	void StartServer();
	void StopServer();
	TrackResult& GetResult();
private:
	void Init();
	void Work();

	void SyncLoop();
	void LoopScreen();
	void LoopTrack();
	
};
