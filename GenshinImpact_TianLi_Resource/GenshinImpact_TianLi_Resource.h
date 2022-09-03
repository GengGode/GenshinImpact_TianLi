#pragma once
#include "resource.h"
#include <wincodec.h>
#include <opencv2/opencv.hpp>

namespace TianLi 
{	
	struct SqliteDbMem {
		unsigned char *ptr;
		__int64 size;
	};

	extern const char* Temp_Sqlite_FileName;

	HBITMAP LoadPNG_GIMAP();
	HBITMAP LoadBitmap_GIPAIMON();

	SqliteDbMem LoadSqlite_KYJGDB();
}
class GenshinImpact_TianLi_Resource
{
	GenshinImpact_TianLi_Resource();
public:
	~GenshinImpact_TianLi_Resource();
	static GenshinImpact_TianLi_Resource* GetInstance();

	HBITMAP LoadPNG_GIMAP() { return TianLi::LoadPNG_GIMAP(); }
	HBITMAP LoadBitmap_GIPAIMON() { return TianLi::LoadBitmap_GIPAIMON(); }
	TianLi::SqliteDbMem LoadSqlite_KYJGDB() { return TianLi::LoadSqlite_KYJGDB(); }

public:
	cv::Mat GIMAP;
	cv::Mat GIAVATAR;
	cv::Mat GISTAR;
	cv::Mat GIPAIMON;
	cv::Mat* GINUMUID;
public:
	const cv::Mat GiMap() { return GIMAP; }
	const cv::Mat GiAvatar() { return GIAVATAR; }
	const cv::Mat GiStar() { return GISTAR; }
	const cv::Mat GiPaimon() { return GIPAIMON; }
	const cv::Mat* GiNumUID() { return GINUMUID; }
	

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