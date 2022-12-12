#pragma once
#ifndef _LIB
#ifdef GENSHINIMPACTTIANLIRESOURCE_EXPORTS
#define GENSHINIMPACTTIANLIRESOURCE_API __declspec(dllexport)
#else
//#define GENSHINIMPACTTIANLIRESOURCE_API __declspec(dllimport)
#define GENSHINIMPACTTIANLIRESOURCE_API /* No thing */
#endif

#define APICALL __stdcall
#ifndef DLLAPI
#define DLLAPI GENSHINIMPACTTIANLIRESOURCE_API
#endif // DLLAPI

#else
#ifndef DLLAPI
#define DLLAPI
#endif // DLLAPI
#endif


#include "resource.h"
#include <wincodec.h>
#include <opencv2/opencv.hpp>

namespace TianLi 
{	
	struct DLLAPI SqliteDbMem {
		unsigned char *ptr;
		__int64 size;
	};

	struct DLLAPI XmlDbMem
	{
		char* ptr;
		__int64 size;
	};

	SqliteDbMem DLLAPI LoadSqlite_KYJGDB();
	XmlDbMem DLLAPI LoadXml_GIMAP_COMPUTE();
}
class DLLAPI GenshinImpact_TianLi_Resource
{
	GenshinImpact_TianLi_Resource();
public:
	~GenshinImpact_TianLi_Resource();
	static GenshinImpact_TianLi_Resource* GetInstance();
	TianLi::SqliteDbMem LoadSqlite_KYJGDB() { return TianLi::LoadSqlite_KYJGDB(); }
	TianLi::XmlDbMem LoadXml_GIMAP_COMPUTE() { return TianLi::LoadXml_GIMAP_COMPUTE(); }

public:
	cv::Mat GIMAP;
	cv::Mat GIAVATAR;
	cv::Mat GISTAR;
	cv::Mat GIPAIMON;
	cv::Mat GIMINIMAP_CAILB;
	cv::Mat GIMINIMAP_N;
	cv::Mat* GINUMUID;
	// overlay
	cv::Mat GIMAP_OVERLAY;
	cv::Rect GIMAP_OVERLAY_RECT = cv::Rect(0, 3482, 1844,1622);
public:
	const cv::Mat GiMap() { return GIMAP; }
	const cv::Mat GiAvatar() { return GIAVATAR; }
	const cv::Mat GiStar() { return GISTAR; }
	const cv::Mat GiPaimon() { return GIPAIMON; }
	const cv::Mat GiMiniMap_Cailb() { return GIMINIMAP_CAILB; }
	const cv::Mat* GiNumUID() { return GINUMUID; }
	
	// overlay
	const cv::Mat GiMap_Overlay() { return GIMAP_OVERLAY; }
private:
	std::map<std::tuple<std::string, std::string, std::string, std::string>, cv::Mat> ImageBuffer;
	std::function<cv::Mat(std::string area, std::string type, std::string item, std::string object)> ImageBuffer_Callback;
public:
	cv::Mat GetImageBuffer(std::string area, std::string type, std::string item, std::string object);
	void SetImageBuffer(std::string area, std::string type, std::string item, std::string object, cv::Mat mat);
	void SetGetImageBufferCallback(std::function<cv::Mat(std::string area, std::string type, std::string item, std::string object)> callback);


private:
	IWICImagingFactory* m_pIWICFactory = NULL;
	HMODULE hModu = NULL;
private:
	void LoadPng_ID2Mat(int IDB, cv::Mat& mat);
	void LoadPng();
	
	bool HBitmap2Mat(HBITMAP& _hBmp, cv::Mat& _mat);
	bool HBitmap2MatAlpha(HBITMAP& _hBmp, cv::Mat& _mat);
	bool Mat2MaskMat(cv::Mat& in, cv::Mat& out);
//
//	QPixmap QImage2QPixmap(QImage& in);
//	QImage QPixmap2QImage(QPixmap& in);
//	Mat QImage2Mat(QImage& in);
//	QImage Mat2QImage(Mat& in);
};