#include "pch.h"
#include "GenshinImpact_TianLi_Track_TaskResult.h"

#include "GenshinImpact_TianLi_Track_Utils_Handle.h"

GenshinImpact_TianLi_Track_TaskResult::GenshinImpact_TianLi_Track_TaskResult()
{
	//=========================================================================
	task_handle                 = new TrackTask<HWND, GenshinHandle>                             (func_get_handle            );
	task_screen                 = new TrackTask<GenshinHandle, GenshinScreen>                    (func_get_screen            );
	task_is_find_paimon         = new TrackTask<GenshinScreen, GenshinPaimon>                    (func_is_find_paimon        );
	task_match_minimap_cailb    = new TrackTask<GenshinScreen, GenshinMinimapCailb>              (func_match_minimap_cailb   );
	task_splite_minimap         = new TrackTask<GenshinScreen, GenshinMinimap>                   (func_splite_minimap        );
	task_get_avatar_direction   = new TrackTask<GenshinMinimap, GenshinAvatarDirection>          (func_get_avatar_direction  );
	task_get_avatar_position    = new TrackTask<GenshinMinimap, GenshinAvatarPosition>           (func_get_avatar_position   );
	task_get_viewer_direction   = new TrackTask<GenshinMinimap, GenshinViewerDirection>          (func_get_viewer_direction  );
	task_get_minimap_direction  = new TrackTask<GenshinMinimap, GenshinMinimapDirection>         (func_get_minimap_direction );
	task_get_stars              = new TrackTask<GenshinMinimap, GenshinStars>                    (func_get_stars             );
	task_get_tagflags           = new TrackTask<GenshinMinimap, GenshinTagflags>                 (func_get_tagflags          );
	task_get_uid                = new TrackTask<GenshinScreen, GenshinUID>                       (func_get_uid               );
	task_get_pickable_items     = new TrackTask<GenshinScreen, GenshinPickableItems>             (func_get_pickable_items    );
	
	//=========================================================================
}

GenshinImpact_TianLi_Track_TaskResult::~GenshinImpact_TianLi_Track_TaskResult()
{
	//=========================================================================
	delete task_handle;
	delete task_screen;
	delete task_is_find_paimon;
	delete task_match_minimap_cailb;
	delete task_splite_minimap;
	delete task_get_avatar_direction;
	delete task_get_avatar_position;
	delete task_get_viewer_direction;
	delete task_get_minimap_direction;
	delete task_get_stars;
	delete task_get_tagflags;
	delete task_get_uid;
	delete task_get_pickable_items;
	//=========================================================================
}

std::map<std::string, bool> GenshinImpact_TianLi_Track_TaskResult::check_wait()
{
	std::map<std::string, bool> wait_map;
	
	//=========================================================================
	static GenshinHandle handle_out;
	static GenshinScreen screen_out;
	static GenshinPaimon paimon_out;
	static GenshinMinimapCailb minimap_cailb_out;
	static GenshinMinimap minimap_out;
	static GenshinAvatarDirection avatar_direction_out;
	static GenshinAvatarPosition avatar_position_out;
	static GenshinViewerDirection viewer_direction_out;
	static GenshinMinimapDirection minimap_direction_out;
	static GenshinStars stars_out;
	static GenshinTagflags tagflags_out;
	static GenshinUID uid_out;
	static GenshinPickableItems pickable_items_out;
	
	if (task_handle->check_wait())
	{
		
		handle_out = task_handle->get_output();
		task_handle->set_input(handle_out.handle);
		task_handle->work();
		wait_map["task_handle"] = true;
	}
	if (task_screen->check_wait())
	{
		screen_out = task_screen->get_output();
		if (/*handle_out.config.capture->mode == Capture::DirectX ||*/ handle_out.config.is_force_used_no_alpha)
		{
			screen_out.config.is_used_alpha = false;
		}
		else
		{
			screen_out.config.is_used_alpha = true;
		}
		task_screen->set_input(handle_out);
		task_screen->work();
		wait_map["task_screen"] = true;
	}
	if (task_is_find_paimon->check_wait())
	{
		paimon_out = task_is_find_paimon->get_output();
		
		screen_out.config.rect_paimon = paimon_out.rect_paimon;
		screen_out.config.is_handle_mode = paimon_out.is_handle_mode;
		
		task_is_find_paimon->set_input(screen_out);
		task_is_find_paimon->work();
		wait_map["task_is_find_paimon"] = true;

	}
	if (task_match_minimap_cailb->check_wait())
	{
		minimap_cailb_out = task_match_minimap_cailb->get_output();
		screen_out.config.rect_minimap_cailb = minimap_cailb_out.rect_minimap_cailb;
		task_match_minimap_cailb->set_input(screen_out);
		task_match_minimap_cailb->work();
		wait_map["task_match_minimap_cailb"] = true;
	}
	if (task_splite_minimap->check_wait())
	{
		minimap_out = task_splite_minimap->get_output();
		task_splite_minimap->set_input(screen_out);
		task_splite_minimap->work();
		wait_map["task_splite_minimap"] = true;
	}
	if (task_get_avatar_direction->check_wait())
	{
		avatar_direction_out = task_get_avatar_direction->get_output();
		task_get_avatar_direction->set_input(minimap_out);
		task_get_avatar_direction->work();
		wait_map["task_get_avatar_direction"] = true;

	}
	if (task_get_avatar_position->check_wait())
	{
		avatar_position_out = task_get_avatar_position->get_output();
		minimap_out.config.is_find_paimon = paimon_out.is_visial;
		task_get_avatar_position->set_input(minimap_out);
		task_get_avatar_position->work();
		wait_map["task_get_avatar_position"] = true;

	}
	if (task_get_viewer_direction->check_wait())
	{
		viewer_direction_out = task_get_viewer_direction->get_output();
		task_get_viewer_direction->set_input(minimap_out);
		task_get_viewer_direction->work();
		wait_map["task_get_view_direction"] = true;

	}
	if (task_get_minimap_direction->check_wait())
	{
		minimap_direction_out = task_get_minimap_direction->get_output();
		task_get_minimap_direction->set_input(minimap_out);
		task_get_minimap_direction->work();
		wait_map["task_get_minimap_direction"] = true;

	}
	if (task_get_stars->check_wait())
	{
		stars_out = task_get_stars->get_output();
		task_get_stars->set_input(minimap_out);
		task_get_stars->work();
		wait_map["task_get_stars"] = true;

	}
	if (task_get_tagflags->check_wait())
	{
		tagflags_out = task_get_tagflags->get_output();
		task_get_tagflags->set_input(minimap_out);
		task_get_tagflags->work();
		wait_map["task_get_tagflags"] = true;

	}
	if (task_get_uid->check_wait())
	{
		uid_out = task_get_uid->get_output();
		task_get_uid->set_input(screen_out);
		task_get_uid->work();
		wait_map["task_get_uid"] = true;

	}
	if (task_get_pickable_items->check_wait())
	{
		pickable_items_out = task_get_pickable_items->get_output();
		task_get_pickable_items->set_input(screen_out);
		task_get_pickable_items->work();
		wait_map["task_get_pickable_items"] = true;

	}

	//=========================================================================

	return wait_map;
}

TianLi::Track::TrackResult GenshinImpact_TianLi_Track_TaskResult::get_result()
{
	//=========================================================================
	result.client_rect.left = task_screen->get_output().rect_client.x;
	result.client_rect.top = task_screen->get_output().rect_client.y;
	result.client_rect.right = task_screen->get_output().rect_client.x + task_screen->get_output().rect_client.width;
	result.client_rect.bottom = task_screen->get_output().rect_client.y + task_screen->get_output().rect_client.height;
	//result.position        = task_get_avatar_direction->get_output();
	result.position_x = task_get_avatar_position->get_output().position.x;
	result.position_y = task_get_avatar_position->get_output().position.y;
	result.is_find_paimon  = task_is_find_paimon->get_output().is_visial;
	result.avatar_angle    = task_get_avatar_direction->get_output().angle;
	result.minimap_rect.left = task_splite_minimap->get_output().rect_minimap.x + task_screen->get_output().rect_client.x;
	result.minimap_rect.top = task_splite_minimap->get_output().rect_minimap.y + task_screen->get_output().rect_client.y;
	result.minimap_rect.right = result.minimap_rect.left + task_splite_minimap->get_output().rect_minimap.width;
	result.minimap_rect.bottom = result.minimap_rect.top + task_splite_minimap->get_output().rect_minimap.height;

	result.viewer_angle = task_get_viewer_direction->get_output().angle;

	result.uid = task_get_uid->get_output().uid;
	result.item_tags = std::vector<std::string>();
	task_get_pickable_items->get_output().item_tags.swap(result.item_tags);
	
	//=========================================================================

	return result;
}

void GenshinImpact_TianLi_Track_TaskResult::set_config(TianLi::Track::TrackConfig& tasks_config)
{
	config = tasks_config;
	//=========================================================================
	
	//=========================================================================
}
