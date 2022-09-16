#include "pch.h"
#include "GenshinImpact_TianLi_Track_Utils.h"
#include "GenshinImpact_TianLi_Track_Utils_Handle.h"
#include "GenshinImpact_TianLi_Track_Utils_Screen.h"
#include "GenshinImpact_TianLi_Track_Utils_MiniMap.h"
#include "GenshinImpact_TianLi_Track_Utils_UID.h"

GenshinHandle func_get_handle(HWND& in)
{
	static GenshinHandle out;
	if (in == 0)
	{
		get_genshin_handle(out);
	}
	else
	{
		update_genshin_handle(in, out);
	}
	return out;
};

GenshinScreen func_get_screen(GenshinHandle& in)
{
	static GenshinScreen out;
	get_genshin_screen(in, out);
	return out;
};

GenshinPaimon func_is_find_paimon(GenshinScreen& in)
{
	static GenshinPaimon out;
	check_paimon(in, out);
	return out;
};

GenshinMinimapCailb func_match_minimap_cailb(GenshinScreen& in)
{
	static GenshinMinimapCailb out;
	match_minimap_cailb(in, out);
	return out;
};

GenshinMinimap func_splite_minimap(GenshinScreen& in)
{
	static GenshinMinimap out;
	splite_minimap(in, out);
	return out;
};

GenshinAvatarDirection func_get_avatar_direction(GenshinMinimap& in)
{
	static GenshinAvatarDirection out;
	get_avatar_direction(in, out);
	return out;
};

GenshinAvatarPosition func_get_avatar_position(GenshinMinimap& in)
{
	static GenshinAvatarPosition out;
	get_avatar_position(in, out);
	return out;
};

GenshinViewerDirection func_get_viewer_direction(GenshinMinimap& in)
{
	static GenshinViewerDirection out;
	get_viewer_direction(in, out);
	return out;
}
GenshinMinimapDirection func_get_minimap_direction(GenshinMinimap& in)
{
	static GenshinMinimapDirection out;
	get_minimap_direction(in, out);
	return out;
}

GenshinStars func_get_stars(GenshinMinimap& in)
{
	static GenshinStars out;
	get_stars(in, out);
	return out;
}

GenshinTagflags func_get_tagflags(GenshinMinimap& in)
{
	static GenshinTagflags out;
	get_tagflags(in, out);
	return out;
}

GenshinUID func_get_uid(GenshinScreen& in)
{
	static GenshinUID out;
	get_uid(in, out);
	return out;
}
