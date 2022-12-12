#pragma once

#include "GenshinImpact_TianLi_Track_Utils_Struct.h"
#include "Capture/Window/BitbltCapture.h"
#include "Capture/Window/DirectXCapture.h"

Capture* create_capture(TianLi::Track::CaptureType capture_type)
{
	switch (capture_type)
	{
	case TianLi::Track::CaptureType::Unkown:
	case TianLi::Track::CaptureType::Bitblt:
	{
		return new BitbltCapture();
	}
	case TianLi::Track::CaptureType::DirectX:
	{
		return new DirectXCapture();
	}
	case TianLi::Track::CaptureType::Video:
	{
		return new BitbltCapture();
	}
	}
}

inline void get_genshin_screen(const GenshinHandle& genshin_handle, GenshinScreen& out_genshin_screen)
{
	auto& giHandle = genshin_handle.handle;
	auto& giRect = genshin_handle.rect;
	auto& giRectClient = genshin_handle.rect_client;
	//auto& giScale = genshin_handle.scale;
	auto& giFrame = out_genshin_screen.img_screen;

	if (out_genshin_screen.config.capture == nullptr)
	{
		out_genshin_screen.config.capture = create_capture(genshin_handle.config.capture_type);
		out_genshin_screen.config.capture->setHandle(giHandle);
		if (out_genshin_screen.config.capture->type == TianLi::Track::CaptureType::DirectX || genshin_handle.config.is_force_used_no_alpha)
		{
			out_genshin_screen.config.is_used_alpha = false;
		}
		else
		{
			out_genshin_screen.config.is_used_alpha = true;
		}
	}
	else
	{
		if (genshin_handle.config.capture_type != out_genshin_screen.config.capture->type)
		{
			delete out_genshin_screen.config.capture;
			out_genshin_screen.config.capture = create_capture(genshin_handle.config.capture_type);
			out_genshin_screen.config.capture->setHandle(giHandle);

			if (out_genshin_screen.config.capture->type == TianLi::Track::CaptureType::DirectX || genshin_handle.config.is_force_used_no_alpha)
			{
				out_genshin_screen.config.is_used_alpha = false;
			}
			else
			{
				out_genshin_screen.config.is_used_alpha = true;
			}
		}
	}

	if (out_genshin_screen.config.capture != nullptr)
	{
		if (out_genshin_screen.config.capture->genshin_handle!= giHandle)
			out_genshin_screen.config.capture->setHandle(giHandle);

		out_genshin_screen.config.capture->capture(giFrame);
	}
	else
	{
		return;
	}

	{
		if (giFrame.empty())return;

		cv::resize(giFrame, giFrame, genshin_handle.size_frame);

		out_genshin_screen.rect_client = cv::Rect(giRect.left, giRect.top, giRectClient.right - giRectClient.left, giRectClient.bottom - giRectClient.top);
		
		// ªÒ»°maybe«¯”Ú
		out_genshin_screen.img_paimon_maybe = giFrame(genshin_handle.rect_paimon_maybe);
		out_genshin_screen.img_minimap_cailb_maybe = giFrame(genshin_handle.rect_minimap_cailb_maybe);
		out_genshin_screen.img_minimap_maybe = giFrame(genshin_handle.rect_minimap_maybe);
		out_genshin_screen.img_uid_maybe = giFrame(genshin_handle.rect_uid_maybe);
		out_genshin_screen.img_left_give_items_maybe = giFrame(genshin_handle.rect_left_give_items_maybe);
		out_genshin_screen.img_right_pick_items_maybe = giFrame(genshin_handle.rect_right_pick_items_maybe);
		out_genshin_screen.img_left_give_items = giFrame(genshin_handle.rect_left_give_items);
		out_genshin_screen.img_right_pick_items = giFrame(genshin_handle.rect_right_pick_items);
		

		out_genshin_screen.config.rect_paimon_maybe = genshin_handle.rect_paimon_maybe;
		out_genshin_screen.config.rect_minimap_cailb_maybe = genshin_handle.rect_minimap_cailb_maybe;
		out_genshin_screen.config.rect_minimap_maybe = genshin_handle.rect_minimap_maybe;


		out_genshin_screen.img_uid = giFrame(genshin_handle.rect_uid);
	}
}
