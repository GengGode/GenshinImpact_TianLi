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
	������,
	Ԩ�¹�,
	���²���,
};
class TrackManage;

namespace TianLi::Track
{
	enum ScreenType
	{
		Window,
		Desktop,
		Bitblt,
		DirectX,
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
	};
	struct TrackConfig
	{

		bool is_auto_find_genshin = true;
		HWND genshin_handle = nullptr;
		ScreenType screen_type = Bitblt;
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
	TianLi::Track::TrackResult& GetResult();
	void SetConfig(TianLi::Track::TrackConfig &config);
};
