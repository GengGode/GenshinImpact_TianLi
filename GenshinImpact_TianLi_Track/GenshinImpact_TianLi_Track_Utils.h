#pragma once
#include "GenshinImpact_TianLi_Track_Utils_Struct.h"

GenshinHandle func_get_handle(HWND& in);

GenshinScreen func_get_screen(GenshinHandle& in);

GenshinPaimon func_is_find_paimon(GenshinScreen& in);

GenshinMinimapCailb func_match_minimap_cailb(GenshinScreen& in);

GenshinMinimap func_splite_minimap(GenshinScreen& in);

GenshinAvatarDirection func_get_avatar_direction(GenshinMinimap& in);

GenshinAvatarPosition func_get_avatar_position(GenshinMinimap& in);

GenshinViewerDirection func_get_viewer_direction(GenshinMinimap& in);

GenshinMinimapDirection func_get_minimap_direction(GenshinMinimap& in);
GenshinStars func_get_stars(GenshinMinimap& in);

GenshinTagflags func_get_tagflags(GenshinMinimap& in);

GenshinUID func_get_uid(GenshinScreen& in);
