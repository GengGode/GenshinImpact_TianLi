#include "pch.h"
#include "GenshinImpact_TianLi_Track.h"
#include "core_track.h"
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include "..\GenshinImpact_TianLi_Resource\GenshinImpact_TianLi_Resource.h"
#pragma comment(lib,"GenshinImpact_TianLi_Resource.lib")

GenshinImpact_TianLi_Track::GenshinImpact_TianLi_Track()
{
	is_run_cv = new std::condition_variable;
	is_run_m = new std::mutex;
	thread_match = new TianLi_ThreadMatch();
	gi_state = new TianLi_GiState();
}

GenshinImpact_TianLi_Track::~GenshinImpact_TianLi_Track()
{
	StopServer();
	is_exit = true;
	sync_thread->join();
	
	delete sync_thread;
	delete thread_match;
	delete gi_state;
	
	delete is_run_m;
	delete is_run_cv;
}

void GenshinImpact_TianLi_Track::StartServer()
{
	if (sync_thread == nullptr)
	{
		sync_thread = new std::thread(&GenshinImpact_TianLi_Track::SyncLoop, this);
	}
	is_run = true;
}

void GenshinImpact_TianLi_Track::StopServer()
{
	is_run = false;
	//is_exit = true;
}

TrackResult& GenshinImpact_TianLi_Track::GetResult()
{
	return track_result;
}

void GenshinImpact_TianLi_Track::Init()
{
	run_func = &GenshinImpact_TianLi_Track::Work;

	thread_match->cThreadSurfMapInit(GenshinImpact_TianLi_Resource::GetInstance()->GIMAP);
	cv::Mat paimon;
	cv::resize(GenshinImpact_TianLi_Resource::GetInstance()->GIPAIMON, paimon, cv::Size(68, 77));
	thread_match->cThreadTemplatePaimonMatch(paimon);
	
	thread_match->cThreadOrbAvatarInit(GenshinImpact_TianLi_Resource::GetInstance()->GIAVATAR);
	thread_match->cThreadTemplateUIDInit(GenshinImpact_TianLi_Resource::GetInstance()->GINUMUID);
	//thread_match->cThreadTemplateStarInit(GenshinImpact_TianLi_Resource::GetInstance()->GISTAR);
	

}

void GenshinImpact_TianLi_Track::Work()
{
	//多线程检查输出
	thread_match->CheckThread();
	thread_match->GetMatchResults();
	//多线程初始化
	thread_match->cThreadSurfMapMatch();
	thread_match->cThreadOrbAvatarMatch();
	thread_match->cThreadTemplateUIDMatch();
	thread_match->cThreadTemplateStarMatch();
}

void GenshinImpact_TianLi_Track::SyncLoop()
{
	// LoopScreen
	//screen_thread = new std::thread(&GenshinImpact_TianLi_Track::LoopScreen, this);
	// LoopTrack
	//track_thread = new thread(&GenshinImpact_TianLi_Track::LoopTrack, this);

	while (!is_exit)
	{
		if (is_run)
		{
			//auto f = &GenshinImpact_TianLi_Track::Init;
			LoopScreen();
			LoopTrack();
			(this->*run_func)();
			track_result.uid = thread_match->uid;
			track_result.x = thread_match->pos.x;
			track_result.y = thread_match->pos.y;
			track_result.a = thread_match->rotationAngle;
			track_result.is_continuity = thread_match->isContinuity;
			track_result.is_paimon_visial = thread_match->isPaimonVisial;
			
			track_result.gi_client_rect = gi_state->giRect;
			/*	
			int uid;
			float x;
			float y;
			float a;
			float a2;
			GiMapType map_type;
			bool is_continuity;
			bool is_on_city;
			VectorStar vector_star;
			*/
		}
		else
		{
			std::unique_lock<std::mutex> lock((*is_run_m));
			(* is_run_cv).wait_for(lock, 1s, [] {return false; });
		}
	}
}

/// <summary>
/// 循环截屏，帧率24fps 大约42ms运行一次
/// </summary>
void GenshinImpact_TianLi_Track::LoopScreen()
{
	
	// 记录截屏函数用时
	auto start_time = std::chrono::system_clock::now();

	//=====================================================

	// 调用截屏
	gi_state->getGiState();
	if (gi_state->giRectMode != 0)
	{
		// 此处需要是原子的
		gi_state->getGiFrame();
	}

	//=====================================================
	
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);
	if (elapsed_time.count() < 42)
	{
		std::chrono::milliseconds sleep_time = 42ms - elapsed_time;
		// 休眠至整个函数总耗时为 42ms
		//std::unique_lock<std::mutex> lock(gi_state->con_var_mutex);
		//gi_state->con_var.wait_for(lock, sleep_time, [] {return true; });
	}
}

void GenshinImpact_TianLi_Track::LoopTrack()
{
	// 记录截屏函数用时
	auto start_time = std::chrono::system_clock::now();

	//=====================================================

	
	if (gi_state->isRunning && gi_state->giRectMode > 0)
	{
		Mat matRGB2GRAY;
		cvtColor(gi_state->giFrameMap, matRGB2GRAY, cv::COLOR_RGB2GRAY);
		thread_match->getObjMinMap(gi_state->giFrameMap);
		//TMS.getObjMinMap(matRGB2GRAY);
		cvtColor(gi_state->giFramePaimon, matRGB2GRAY, cv::COLOR_RGB2GRAY);
		thread_match->getObjPaimon(matRGB2GRAY);
		thread_match->getObjUID(gi_state->giFrameUID);
	}
	else
	{
		thread_match->isExistObjMinMap = false;
		thread_match->isExistObjPaimon = false;
		thread_match->isExistObjUID = false;
	}
	
	//=====================================================

	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);
	if (elapsed_time.count() < 42)
	{
		std::chrono::milliseconds sleep_time = 42ms - elapsed_time;
		// 休眠至整个函数总耗时为 42ms
		//std::unique_lock<std::mutex> lock(gi_state->con_var_mutex);
		//gi_state->con_var.wait_for(lock, sleep_time, [] {return true; });
	}
}
