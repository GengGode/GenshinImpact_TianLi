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
	enum class CaptureType
	{
		Unkown,
		Bitblt,
		DirectX,
		Video
	};
	struct TrackResultStarPosition
	{
		double x;
		double y;
	};
	template<typename T>
	struct Array
	{
		T* data;
		int size;
		
		Array()
		{
			data = nullptr;
			size = 0;
		}
		~Array()
		{
			delete data;
		}
		
	};
	struct TrackResultStar
	{
		bool is_exist;
		int count;
		Array<TrackResultStarPosition> position;
	};
	struct TrackResult
	{
		
		double position_x;
		double position_y;

		bool is_find_paimon = false;
		double avatar_angle = 0.0;
		double viewer_angle = 0.0;
		RECT client_rect;
		RECT minimap_rect;
		int uid = 0;
		int last_runtime_ms;
		
		std::vector<std::string> item_tags;

	};
	struct TrackConfig
	{

		bool is_auto_find_genshin = true;
		HWND genshin_handle = nullptr;
		CaptureType capture_type = CaptureType::Bitblt;
	};
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
	bool ServerState();
	TianLi::Track::TrackResult& GetResult();
	TianLi::Track::TrackConfig& GetConfig();
	void SetConfig(TianLi::Track::TrackConfig &config);
};

