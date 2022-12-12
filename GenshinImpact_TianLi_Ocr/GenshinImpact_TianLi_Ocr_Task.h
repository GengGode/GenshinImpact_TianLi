#pragma once
#include <functional>
#include <thread>	
#include <vector>	
#include <map>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include "3rdparty/libocr/include/libocr.h"
#pragma comment(lib,"3rdparty/libocr/lib/libocr.lib")
class GenshinImpact_TianLi_Ocr_Task
{
	std::string(*func_ptr)(cv::Mat&) = [](cv::Mat& image)-> auto {
		std::string result;
		char* result_buff = new char[1024];
		int res = libocr::ocr_image_data(image.cols,image.rows,(const char*)image.data, image.channels() * image.rows * image.cols, result_buff, 1024);
		if (res == 0) {
			result = std::string(result_buff);
		}
		delete[] result_buff;
		return result;
	};
	std::function<void(const std::string&, std::string&)> result_callback;
	bool is_exit = false;
	bool is_run = false;
	bool is_finish = false;
	std::condition_variable is_run_cv;
	std::mutex is_run_m;
	std::mutex is_used_m;
	std::chrono::milliseconds call_cycle_wait_time = std::chrono::milliseconds(50);
private:
	std::thread* thread_task = nullptr;
public:
	GenshinImpact_TianLi_Ocr_Task()
	{
		thread_task = new std::thread(&GenshinImpact_TianLi_Ocr_Task::run, this);
	}
	~GenshinImpact_TianLi_Ocr_Task()
	{
		{
			std::unique_lock<std::mutex> lk_is_used(is_used_m);
			result_map.clear();
			task_map.clear();
			work_map.clear();
		}
		
		is_exit = true;
		is_run_cv.notify_all();
		thread_task->join();
		delete thread_task;
		
	}
public:
	std::map<std::string, cv::Mat> task_map;
	std::map<std::string, std::string> result_map;
	std::map<std::string, bool> work_map;
	
public:
	std::string add_image_tag(cv::Mat& image)
	{
		// 生成唯一标识 随机
		std::string tag = std::to_string(rand());
		std::lock_guard<std::mutex> lock(is_used_m);
		task_map[tag] = image;
		work_map[tag] = false;
		is_run_cv.notify_one();
		return tag;
	}
	bool get_image_tag(std::string id, std::string& tag)
	{
		std::lock_guard<std::mutex> lock(is_used_m);
		if (result_map.find(id) != result_map.end())
		{
			tag = result_map[id];
			// 清理，加锁
			result_map.erase(id);
			task_map.erase(id);
			work_map.erase(id);
			return true;
		}
		return false;
	}

private:
	void run()
	{
		while (!is_exit)
		{
			std::unique_lock<std::mutex> lk_is_run(is_run_m);
			is_run_cv.wait(lk_is_run);
			if (is_exit) break;
			// 业务逻辑
			
			is_finish = true;

			// 1. get task input image
			// 2. call func_ptr
			// 3. set task output tag
			
			std::unique_lock<std::mutex> lk_is_used(is_used_m);
			// 循环开始时，由于要从map中获取数据，所以需要加锁
			for (auto& [uuid, image]: task_map)
			{
				// 取完数据后，之后需要执行的是耗时操作，所以需要解锁
				lk_is_used.unlock();
			
				auto tag = func_ptr(image);
				result_map.insert(std::make_pair(uuid, tag));
				work_map.insert(std::make_pair(uuid, true));
				if (result_callback)
				{
					result_callback(uuid,tag);
				}
				// 为防止连续调用导致高cpu占用
				std::this_thread::sleep_for(call_cycle_wait_time);
				
				// delete task
				//task_list.erase(uuid);
				
				// 由于要进入到下一次循环，取map中的数据，所以需要加锁
				lk_is_used.lock();
			}
			// 循环结束时，对map的操作已经完成，所以需要解锁
			lk_is_used.unlock();

			for (auto& [uuid, is_finish] : work_map)
			{
				if (is_finish)
				{
					std::unique_lock<std::mutex> lk_is_used(is_used_m);
					task_map.erase(uuid);
				}
			}
			is_finish = false;
		}
	}
};


