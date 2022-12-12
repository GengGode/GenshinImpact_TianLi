#pragma once
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include "..\GenshinImpact_TianLi_Track.h"
class Capture
{
public:
	TianLi::Track::CaptureType type = TianLi::Track::CaptureType::Unkown;
	//ErrorCode& err = ErrorCode::getInstance();
	std::pair<int, std::string> err;

	Capture() {}
	virtual ~Capture() {}

	virtual bool init() = 0;
	virtual bool uninit() = 0;
	virtual bool capture(cv::Mat& frame) = 0;
	virtual bool setHandle(HWND handle = 0) = 0;
};
