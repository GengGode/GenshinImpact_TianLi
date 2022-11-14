#pragma once
#include "GenshinImpact_TianLi_Track_Utils_Struct.h"


#include "..\GenshinImpact_TianLi_Ocr\GenshinImpact_TianLi_Ocr.h"
#pragma comment(lib,"GenshinImpact_TianLi_Ocr.lib")

#include "../GenshinImpact_TianLi_Ocr/3rdparty/libocr/include/libocr.h"
#pragma comment(lib,"../GenshinImpact_TianLi_Ocr/3rdparty/libocr/lib/libocr.lib")

inline std::string ocr_call(cv::Mat& mat)
{
	cv::Mat mat_gray;
	cv::cvtColor(mat, mat_gray, cv::COLOR_BGRA2BGR);
	std::string text;
	{
		const char* t = libocr::ocr_image(mat_gray.cols, mat_gray.rows, (char*)mat_gray.data, mat_gray.channels() * mat_gray.rows * mat_gray.cols);
		text = t;
		libocr::free_char((char*)t);
	}
	//auto text = GenshinImpact_TianLi_Ocr::GetInstance().add_image_tag(mat);
	return text;
}
inline std::vector<std::string> ocr_call(std::vector<cv::Mat>& vec)
{
	std::vector<std::string> str;
	for (int i = 0; i < vec.size(); i++)
	{
		str.push_back(ocr_call(vec[i]));
	}
	return str;
}

inline std::string ocr_task(cv::Mat mat)
{
	return ocr_call(mat);
}

inline void get_pickable_items(const GenshinScreen& genshin_screen, GenshinPickableItems& out_genshin_pickable_items)
{
	auto roi = genshin_screen.img_right_pick_items;
	
	if (roi.empty()) return;
	roi = roi(cv::Rect(64, 0, roi.cols - 64, roi.rows));

	
	cv::imshow("pickable", roi);
	cv::waitKey(1);
	std::vector<cv::Mat> layers;
	split(roi, layers);

	// 横向拼接
	cv::Mat h_all = cv::Mat::zeros(cv::Size(roi.cols * layers.size(), roi.rows), CV_8UC1);
	for (int i = 0; i < layers.size(); i++)
	{
		cv::Mat r = h_all(cv::Rect(roi.cols * i, 0, roi.cols, roi.rows));
		layers[i].copyTo(r);
	}
	// show
	//imshow("h_all", h_all);
	// show alpha layer
	cv::Mat alpha_layer, alpha_layer_max;
	cv::threshold(layers[3], alpha_layer, 245, 255, cv::THRESH_BINARY);

	// show alpha layer
	//imshow("alpha_layer", alpha_layer);
	// 扩散三个像素
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 3));
	dilate(alpha_layer, alpha_layer, kernel);

	// 查找轮廓
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours(alpha_layer, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	// 画出轮廓
	cv::Mat drawing = cv::Mat::zeros(alpha_layer.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(255, 255, 255);
		cv::Rect rect = cv::boundingRect(contours[i]);
		rectangle(layers[3], rect, color, 1);
	}
	imshow("alpha right", layers[3]);

	static std::mutex task_mutex;
	static std::queue<std::function<std::string()>> task_queue;
	// 从原图中取出矩形
	std::vector<cv::Mat> roi_vec;
	for (int i = 0; i < contours.size(); i++)
	{
		cv::Rect rect = cv::boundingRect(contours[i]);
		if (rect.width < 16 || rect.height < 16 ||
			rect.width / rect.height < 0.3)
		{
			continue;
		}
		roi_vec.push_back(roi(rect));
		
		std::lock_guard<std::mutex> lock(task_mutex);
		task_queue.push(std::bind(ocr_task, roi(rect)));
	}
	
	// queue task ocr
	int ocr_task_solt_count = roi_vec.size();
	if (ocr_task_solt_count == 0)
	{
		ocr_task_solt_count = 1;
		if (task_queue.empty())
		{
			ocr_task_solt_count = 0;
		}
	}
	for (int i = 0; i < ocr_task_solt_count; i++)
	{
		auto task = task_queue.front();
		task_queue.pop();
		if (task)
		{
			out_genshin_pickable_items.item_tags.push_back(task());
			Sleep(100);
		}
	}

	// ocr
	//auto res = ocr_call(roi_vec);
	//for (int i = 0; i < res.size(); i++)
	//{
	//	out_genshin_pickable_items.item_tags.push_back(res[i]);
	//}
	// Sleep
	
	//Sleep(3000);
}
