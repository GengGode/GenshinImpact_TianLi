#pragma once
#ifndef _LIB
#ifdef GENSHINIMPACTTIANLIOCR_EXPORTS
#define GENSHINIMPACTTIANLIOCR_API __declspec(dllexport)
#else
#define GENSHINIMPACTTIANLIOCR_API __declspec(dllimport)
#endif // GENSHINIMPACT_TIANLI_OCR_EXPORTS

#define APICALL __stdcall
#ifndef DLLAPI
#define DLLAPI GENSHINIMPACTTIANLIOCR_API
#endif // DLLAPI

#else
#ifndef DLLAPI
#define DLLAPI
#endif // DLLAPI
#endif

#include <opencv2/opencv.hpp>

class GenshinImpact_TianLi_Ocr_Task;
class DLLAPI GenshinImpact_TianLi_Ocr
{
	GenshinImpact_TianLi_Ocr();
public:
	static GenshinImpact_TianLi_Ocr& GetInstance();
	~GenshinImpact_TianLi_Ocr();

public:
	// 添加待识别图片到队列
	std::string add_image_tag(cv::Mat image);
	// 从队列中获取识别结果	
	bool get_image_tag(std::string uuid, std::string& tag);
	
	void set_callback_ocr_over(std::function<void(const std::string&, std::string&)> callback);
	
	// 识别图片
	std::string ocr_image(cv::Mat image);
	
private:

	// 识别线程
	GenshinImpact_TianLi_Ocr_Task* task;
	// std::thread *thread_ocr_service=nullptr;
};
