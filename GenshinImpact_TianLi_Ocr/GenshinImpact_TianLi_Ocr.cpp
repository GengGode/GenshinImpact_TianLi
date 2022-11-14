#include "pch.h"
#include "GenshinImpact_TianLi_Ocr.h"
#include "GenshinImpact_TianLi_Ocr_Task.h"	

GenshinImpact_TianLi_Ocr::GenshinImpact_TianLi_Ocr()
{
	task = new GenshinImpact_TianLi_Ocr_Task();
}

GenshinImpact_TianLi_Ocr::~GenshinImpact_TianLi_Ocr()
{
	delete task;
}

GenshinImpact_TianLi_Ocr& GenshinImpact_TianLi_Ocr::GetInstance()
{
	static GenshinImpact_TianLi_Ocr instance;
	return instance;
}

std::string GenshinImpact_TianLi_Ocr::add_image_tag(cv::Mat image)
{
	std::string tag = task->add_image_tag(image);
	return tag;
}

bool GenshinImpact_TianLi_Ocr::get_image_tag(std::string uuid, std::string& tag)
{
	return task->get_image_tag(uuid, tag);
}

void GenshinImpact_TianLi_Ocr::set_callback_ocr_over(std::function<void(const std::string&,std::string&)> callback)
{
}

std::string GenshinImpact_TianLi_Ocr::ocr_image(cv::Mat image)
{
	std::string result;
	char* result_buff = new char[1024];
	int res = libocr::ocr_image_data(image.cols, image.rows, (const char*)image.data, image.channels() * image.rows * image.cols, result_buff, 1024);
	if (res == 0) {
		result = std::string(result_buff);
	}
	delete[] result_buff;
	return result;
}

