#pragma once

#include "GenshinImpact_TianLi_Track_Utils.h"

template <typename T_in, typename T_out>
class TrackTask
{
	T_in* in_object;
	T_out* out_object;
	T_out(*func_ptr)(T_in&) = nullptr;

	bool is_exit = false;
	bool is_run = false;
	bool is_finish = false;
	std::condition_variable is_run_cv;
	std::mutex is_run_m ;
	std::mutex m;
	std::thread* track_thread = nullptr;
public:
	TrackTask()
	{
		in_object = new T_in();
		out_object = new T_out();
		track_thread = new std::thread(&TrackTask<T_in,T_out>::run, this);
		
	}
	TrackTask(T_out(*task_func_ptr)(T_in&))
	{
		in_object = new T_in();
		out_object = new T_out();
		func_ptr = task_func_ptr;
		track_thread = new std::thread(&TrackTask<T_in, T_out>::run, this);
	}
	~TrackTask()
	{
		is_exit = true;
		is_run_cv.notify_one();

		if (track_thread != nullptr)
		{
			track_thread->join();
			delete track_thread;
		}
		in_object->~T_in();
		out_object->~T_out();
	}
	bool check_wait()
	{
		return !is_finish;
	}
	void set_input(T_in& in)
	{
		// TODO 非拷贝
		*in_object = in;
	}
	T_out get_output()
	{
		// TODO 非拷贝
		std::lock_guard<std::mutex> lk(m);
		return *out_object;
	}
	void task(T_out(*task_func_ptr)(T_in&))
	{
		func_ptr = task_func_ptr;
		//is_run = true;
	}
	void work()
	{
		is_run_cv.notify_one();
	}
	void run()
	{
		while (!is_exit)
		{
			std::unique_lock<std::mutex> lk_is_run(is_run_m);
			is_run_cv.wait(lk_is_run);
			if (is_exit) break;
			// 业务逻辑

			if (func_ptr != nullptr)
			{
				is_finish = true;
				T_out m_out_object = func_ptr(*in_object);
				// TODO 不确定为拷贝
				// 此处需要是原子操作
				std::lock_guard<std::mutex> lk(m);
				*out_object = m_out_object;

				is_finish = false;
			}
		}
	}
};

struct TrackInputTemplate
{
	cv::Mat gi_map;
	cv::Mat gi_paimon;
	cv::Mat gi_avatar;
	cv::Mat gi_avatar2;
	cv::Mat gi_star;
	std::vector<cv::Mat> gi_uid_number;
	std::vector<cv::Mat> gi_map_flag_icon;
};

class GenshinImpact_TianLi_Track_TaskResult
{
	// 获取句柄
	TrackTask<HWND, GenshinHandle>* task_handle = nullptr;
	// 获取屏幕画面
	TrackTask<GenshinHandle, GenshinScreen>* task_screen = nullptr;
	// 检查派蒙
	TrackTask<GenshinScreen, GenshinPaimon>* task_is_find_paimon = nullptr;
	// 匹配小地图标定
	TrackTask<GenshinScreen, GenshinMinimapCailb>* task_match_minimap_cailb = nullptr;
	// 切割小地图
	TrackTask<GenshinScreen, GenshinMinimap>* task_splite_minimap = nullptr;
	// 从小地图获取角色朝向
	TrackTask<GenshinMinimap, GenshinAvatarDirection>* task_get_avatar_direction = nullptr;
	// 从小地图获取角色位置
	TrackTask<GenshinMinimap, GenshinAvatarPosition>* task_get_avatar_position = nullptr;
	// 从小地图获取视角朝向
	TrackTask<GenshinMinimap, GenshinViewerDirection>* task_get_viewer_direction = nullptr;
	// 从小地图获取地图旋转
	TrackTask<GenshinMinimap, GenshinMinimapDirection>* task_get_minimap_direction = nullptr;
	// 从小地图获取神瞳
	TrackTask<GenshinMinimap, GenshinStars>* task_get_stars = nullptr;
	// 从小地图获取可标记
	TrackTask<GenshinMinimap, GenshinTagflags>* task_get_tagflags = nullptr;
	
	// 从屏幕画面获取UID
	TrackTask<GenshinScreen, GenshinUID>* task_get_uid = nullptr;
	
	// 从屏幕画面获取右侧可捡取物品列表
	TrackTask<GenshinScreen, GenshinPickableItems>* task_get_pickable_items = nullptr;

	
	TianLi::Track::TrackResult result;
	TianLi::Track::TrackConfig config;
public:
	GenshinImpact_TianLi_Track_TaskResult();
	~GenshinImpact_TianLi_Track_TaskResult();
public:
	std::map<std::string, bool> check_wait();
	TianLi::Track::TrackResult get_result();
	void set_config(TianLi::Track::TrackConfig& tasks_config);
};

class TrackManage
{
	bool is_run = false;
	bool is_exit = false;

	std::chrono::milliseconds track_tick_time_ms = std::chrono::milliseconds(42);
	std::chrono::milliseconds last_runtime_ms = std::chrono::milliseconds(0);

	std::thread* track_thread = nullptr;
	std::condition_variable* is_run_cv = nullptr;
	std::mutex* is_run_m = nullptr;

	GenshinImpact_TianLi_Track_TaskResult* track_tasks;

	TianLi::Track::TrackResult tasks_result;
	TianLi::Track::TrackConfig tasks_config;
	std::mutex* result_m = nullptr;
	std::mutex* config_m = nullptr;

public:
	TrackManage()
	{
		is_run_cv = new std::condition_variable();
		is_run_m = new std::mutex();
		result_m = new std::mutex();
		config_m = new std::mutex();


		track_tasks = new GenshinImpact_TianLi_Track_TaskResult();
	}
	~TrackManage()
	{
		is_exit = true;

		is_run_cv->notify_all();
		if (track_thread != nullptr)
		{
			track_thread->join();
			delete track_thread;
		}

		track_tasks->~GenshinImpact_TianLi_Track_TaskResult();

		delete is_run_cv;
		delete is_run_m;
		delete result_m;
		delete config_m;
	}
	void Start()
	{
		if (track_thread == nullptr)
		{
			track_thread = new std::thread(&TrackManage::server, this);
		}
		is_run_cv->notify_all();
		is_run = true;
	}
	void Stop()
	{
		is_run = false;
	}
	bool GetIsRun()
	{
		return is_run;
	}
	TianLi::Track::TrackResult& GetResult()
	{
		std::lock_guard<std::mutex> lk(*result_m);
		tasks_result.last_runtime_ms =static_cast<int>( last_runtime_ms.count());
		return tasks_result;
	}
	void SetConfig(TianLi::Track::TrackConfig& config)
	{
		std::lock_guard<std::mutex> lk(*result_m);
		tasks_config = config;
	}
private:
	void server()
	{
		while (!is_exit)
		{
			if (!is_run)
			{
				// is_run 为 false 时，阻塞cv等待1s
				std::unique_lock<std::mutex> lk(*is_run_m);
				is_run_cv->wait_for(lk, std::chrono::seconds(1), [this] { return is_run; });
				continue;
			}
			auto start = std::chrono::system_clock::now();

			// 业务逻辑
			if (check_wait_any())
			{
				sync_result();
				sync_config();
			}
			
			auto end = std::chrono::system_clock::now();
			last_runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			if (last_runtime_ms < track_tick_time_ms)
			{
				
				std::unique_lock<std::mutex> lk(*is_run_m);
				is_run_cv->wait_for(lk, track_tick_time_ms - last_runtime_ms, [this] { return false; });
			}
		}
	}
	bool check_wait_any()
	{
		// 保存task的状态
		std::map<std::string, bool> wait_state_list;

		wait_state_list = track_tasks->check_wait();

		// 存在任意一个task完成
		if (!wait_state_list.empty())
		{
			return true;
		}
		return false;
	}
	void sync_result()
	{
		std::lock_guard<std::mutex> lk(*result_m);
		tasks_result = track_tasks->get_result();
	}
	void sync_config()
	{
		std::lock_guard<std::mutex> lk(*result_m);
		track_tasks->set_config(tasks_config);
	}
};