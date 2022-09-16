#pragma once
#include "..\GenshinImpact_TianLi_Resource\GenshinImpact_TianLi_Resource.h"
#pragma comment(lib,"GenshinImpact_TianLi_Resource.lib")

//#define TEST_LOCAL

enum ScreenType
{
	Window,
	Desktop,
	Bitblt,
	DirectX,
};

struct GenshinHandleConfig
{
	bool is_auto_find_genshin = true;
	HWND genshin_handle = nullptr;

};
struct GenshinHandle
{
	bool is_exist = false;
	HWND handle;
	RECT rect;
	RECT rect_client;
	double scale;
	bool is_exist_title_bar = true;
	cv::Size size_frame;
	cv::Rect rect_paimon_maybe;
	cv::Rect rect_paimon;
	cv::Rect rect_minimap_cailb_maybe;
	cv::Rect rect_minimap_cailb;
	cv::Rect rect_minimap_maybe;
	cv::Rect rect_minimap;
	cv::Rect rect_avatar_maybe;
	cv::Rect rect_avatar;
	cv::Rect rect_uid_maybe;
	cv::Rect rect_uid;
	cv::Rect rect_star_maybe;
	cv::Rect rect_star;
	cv::Rect rect_map_flag_maybe;
	cv::Rect rect_map_flag;
	cv::Rect rect_map_flag_icon_maybe;
	cv::Rect rect_map_flag_icon;
	cv::Rect rect_left_give_items_maybe;
	cv::Rect rect_left_give_items;
	cv::Rect rect_right_pick_items_maybe;
	cv::Rect rect_right_pick_items;
	cv::Rect rect_hp_maybe;
	cv::Rect rect_hp;
	GenshinHandleConfig config;
};
struct GenshinScreenConfig
{
	ScreenType screen_type = Bitblt;
	cv::Rect rect_paimon_maybe;
	cv::Rect rect_minimap_cailb_maybe;
	cv::Rect rect_minimap_maybe;

	bool is_handle_mode = false;

	cv::Rect rect_paimon;
	cv::Rect rect_minimap_cailb;
	cv::Rect rect_minimap;
};
struct GenshinScreen
{
	cv::Rect rect_client;

	cv::Mat img_screen;
	cv::Mat img_paimon_maybe;
	cv::Mat img_paimon;
	cv::Mat img_minimap_cailb_maybe;
	cv::Mat img_minimap_cailb;
	cv::Mat img_minimap_maybe;
	cv::Mat img_minimap;
	cv::Mat img_avatar_maybe;
	cv::Mat img_avatar;
	cv::Mat img_uid_maybe;
	cv::Mat img_uid;
	cv::Mat img_star_maybe;
	cv::Mat img_star;
	cv::Mat img_map_flag_maybe;
	cv::Mat img_map_flag;
	cv::Mat img_map_flag_icon_maybe;
	cv::Mat img_map_flag_icon;
	cv::Mat img_left_give_items_maybe;
	cv::Mat img_left_give_items;
	cv::Mat img_right_pick_items_maybe;
	cv::Mat img_right_pick_items;
	cv::Mat img_hp_maybe;
	cv::Mat img_hp;
	GenshinScreenConfig config;
};
struct GenshinPaimonConfig
{
	double check_match_paimon_params = 0.85;
};

struct GenshinPaimon
{
	bool is_visial = false;
	bool is_handle_mode = false;
	cv::Rect rect_paimon;
	GenshinPaimonConfig config;
};
struct GenshinMinimapCailbConfig
{
	bool is_need_find = true;
	double check_match_minimap_cailb_params = 0.85;
};

struct GenshinMinimapCailb
{
	bool is_visial = false;
	cv::Rect rect_minimap_cailb;
	GenshinMinimapCailbConfig config;
};
struct GenshinMinimapConfig
{
	bool is_need_cailb = true;
	bool is_find_paimon = false;
	
};

struct GenshinMinimap
{
	bool is_cailb = false;
	cv::Mat img_minimap;
	cv::Rect rect_minimap;
	cv::Point point_minimap_center;
	cv::Rect rect_avatar;
	cv::Mat img_avatar;
	cv::Rect rect_viewer;
	cv::Mat img_viewer;
	cv::Rect rect_stars;
	cv::Mat img_stars;
	
	GenshinMinimapConfig config;
};
struct GenshinAvatarDirectionConfig
{

};
struct GenshinAvatarDirection
{
	double angle = 0;
	GenshinAvatarDirectionConfig config;
};

struct GenshinAvatarPositionConfig
{
	bool is_init_finish = false;
	bool is_lock_minimap_rotation = true;
	double minimap_rotation = 0;
	bool is_con = false;
	bool is_cov = false;
	bool is_on_city = false;
};
struct GenshinAvatarPosition
{
	cv::Point2d position;
	GenshinAvatarPositionConfig config;
};

struct GenshinViewerDirectionConfig
{
	bool is_lock_minimap_rotation = true;
	double minimap_rotation = 0;
};

struct GenshinViewerDirection
{
	double angle = 0;
	GenshinViewerDirectionConfig config;
};

struct GenshinMinimapDirectionConfig
{
	bool  is_defalut = true;
	bool is_skip = true;
};

struct GenshinMinimapDirection
{
	double angle = 0;
	GenshinMinimapDirectionConfig config;
};
struct GenshinStarsConfig
{
	double check_match_star_params = 0.85;
};
struct GenshinStars
{
	bool is_find = false;
	std::vector<cv::Point> points_star;
	GenshinStarsConfig config;
};

struct GenshinTagflagsConfig
{
	double check_match_star_params = 0.85;
};
struct GenshinTagflags
{
	bool is_find = false;
	std::vector<cv::Point> points_star;
	GenshinTagflagsConfig config;
};

struct GenshinUIDConfig
{
	double check_match_uid_params = 0.85;
};
struct GenshinUID
{
	int uid = 0;
	GenshinUIDConfig config;
};

#ifdef _DEBUG
#define log(str) std::cout << __FUNCTION__ <<":"<< __LINE__<<" ||=>> " << str <<'\n';
#else
#define log(str)
#endif