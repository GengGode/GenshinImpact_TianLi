// GenshinImpact_TianLi_Resource.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"

#include "GenshinImpact_TianLi_Resource.h"
#include <vector>
#include <opencv2/opencv.hpp>

namespace TianLi
{
	const char* Temp_Sqlite_FileName = "Temp~";
	
	HBITMAP LoadPNG_GIMAP()
	{
		HMODULE hModu = NULL;
		IWICStream* pIWICStream = NULL;
		IWICBitmapDecoder* pIDecoder = NULL;
		IWICBitmapFrameDecode* pIDecoderFrame = NULL;
		IWICImagingFactory* m_pIWICFactory = NULL;
		IWICBitmapSource* bitmap_source = NULL;
		HRSRC imageResHandle = NULL;
		HGLOBAL imageResDataHandle = NULL;
		void* pImageFile = NULL;
		DWORD imageFileSize = 0;
		
		HBITMAP hGIMAP;
		
		hModu = GetModuleHandle(0);

		CoInitializeEx(NULL, COINIT_MULTITHREADED);

		CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_pIWICFactory)
		);

		imageResHandle = FindResource(hModu, MAKEINTRESOURCE(IDB_PNG_GIMAP), L"PNG");
		imageResDataHandle = LoadResource(hModu, imageResHandle);
		pImageFile = LockResource(imageResDataHandle);
		imageFileSize = SizeofResource(hModu, imageResHandle);
		m_pIWICFactory->CreateStream(&pIWICStream);

		pIWICStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize);
		m_pIWICFactory->CreateDecoderFromStream(
			pIWICStream,                   // The stream to use to create the decoder
			NULL,                          // Do not prefer a particular vendor
			WICDecodeMetadataCacheOnLoad,  // Cache metadata when needed
			&pIDecoder);                   // Pointer to the decoder
		pIDecoder->GetFrame(0, &pIDecoderFrame);

		bitmap_source = pIDecoderFrame;

		UINT width = 0, height = 0, depht = 4;
		bitmap_source->GetSize(&width, &height);

		{
			std::vector<BYTE> buffer(width * height * depht);
			bitmap_source->CopyPixels(NULL, width * depht, static_cast<UINT>(buffer.size()), buffer.data());

			hGIMAP = CreateBitmap(width, height, 1, 8 * depht, buffer.data());
		}
		return hGIMAP;
	}

	// 加载 Bitmap 图片 ID_GIPAIMON
	HBITMAP LoadBitmap_GIPAIMON()
	{
		HMODULE H_Module = GetModuleHandleW(NULL);
		HBITMAP H_GIPAIMON = LoadBitmap(H_Module, MAKEINTRESOURCE(IDB_BITMAP_GIPAIMON));
		return H_GIPAIMON;
	}
	SqliteDbMem LoadSqlite_KYJGDB()
	{
		HMODULE H_Module = GetModuleHandleW(NULL);
		HRSRC H_SQLITE = FindResource(H_Module, MAKEINTRESOURCE(IDR_SQLITE_KYJG), L"SQLITE");
		SqliteDbMem Sqlite_KYJGDB = { nullptr,0 };
#ifdef DEBUG
		int e = GetLastError();
		if (e != 0)
		{
			throw(e);
		}
#endif // DEBUG
		if (H_SQLITE == NULL)
		{
#ifdef DEBUG
			e = GetLastError();
			if (e != 0)
			{
				throw(e);
			}
#endif // DEBUG
		}
		else
		{
			HGLOBAL H_SqliteDB_Handle = LoadResource(H_Module, H_SQLITE);
			// 判断 H_SqliteDB_Handle 是否为空
			if (H_SqliteDB_Handle == NULL)
			{
#ifdef DEBUG
				e = GetLastError();
				if (e != 0)
				{
					throw(e);
				}
#endif // DEBUG
			}
			else
			{
				LPVOID H_SqliteDB_Ptr = LockResource(H_SqliteDB_Handle);
				// 判断 H_SqliteDB_Ptr 是否为空
				if (H_SqliteDB_Ptr == NULL)
				{
#ifdef DEBUG
					e = GetLastError();
					if (e != 0)
					{
						throw(e);
					}
#endif // DEBUG
				}
				else
				{
					DWORD SqliteDB_Size = SizeofResource(H_Module, H_SQLITE);
					// 判断 SqliteDB_Size 是否为空
					if (SqliteDB_Size == NULL)
					{
#ifdef DEBUG
						e = GetLastError();
						if (e != 0)
						{
							throw(e);
						}
#endif // DEBUG
					}
					else
					{
						Sqlite_KYJGDB.ptr = static_cast<unsigned char*>(H_SqliteDB_Ptr);
						Sqlite_KYJGDB.size = SqliteDB_Size;
					}
				}
			}
		}
		return Sqlite_KYJGDB;
	}
	XmlDbMem LoadXml_GIMAP_COMPUTE()
	{
		HMODULE H_Module = GetModuleHandleW(NULL);
		HRSRC H_XML = FindResource(H_Module, MAKEINTRESOURCE(IDR_XML_GIMAP_COMPUTE), L"XML");
		XmlDbMem Xml_GIMAP_COMPUTE = { nullptr,0 };
#ifdef DEBUG
		int e = GetLastError();
		if (e != 0)
		{
			throw(e);
		}
#endif // DEBUG
		if (H_XML == NULL)
		{
#ifdef DEBUG
			e = GetLastError();
			if (e != 0)
			{
				throw(e);
			}
#endif // DEBUG
		}
		else
		{
			HGLOBAL H_XmlDB_Handle = LoadResource(H_Module, H_XML);
			// 判断 H_SqliteDB_Handle 是否为空
			if (H_XmlDB_Handle == NULL)
			{
#ifdef DEBUG
				e = GetLastError();
				if (e != 0)
				{
					throw(e);
				}
#endif // DEBUG
			}
			else
			{
				LPVOID H_XmlDB_Ptr = LockResource(H_XmlDB_Handle);
				// 判断 H_SqliteDB_Ptr 是否为空
				if (H_XmlDB_Ptr == NULL)
				{
#ifdef DEBUG
					e = GetLastError();
					if (e != 0)
					{
						throw(e);
					}
#endif // DEBUG
				}
				else
				{
					DWORD XmlDB_Size = SizeofResource(H_Module, H_XML);
					// 判断 SqliteDB_Size 是否为空
					if (XmlDB_Size == NULL)
					{
#ifdef DEBUG
						e = GetLastError();
						if (e != 0)
						{
							throw(e);
						}
#endif // DEBUG
					}
					else
					{
						Xml_GIMAP_COMPUTE.ptr = static_cast<char*>(H_XmlDB_Ptr);
						Xml_GIMAP_COMPUTE.size = XmlDB_Size;
					}
				}
			}
		}
		return Xml_GIMAP_COMPUTE;
		
		
	}
}

GenshinImpact_TianLi_Resource::GenshinImpact_TianLi_Resource()
{
	/**************************************************/
	GINUMUID          = new cv::Mat[11];//NUM
	/**************************************************/
	LoadPng();
}
GenshinImpact_TianLi_Resource::~GenshinImpact_TianLi_Resource()
{
	delete[] GINUMUID;
}
GenshinImpact_TianLi_Resource* GenshinImpact_TianLi_Resource::GetInstance()
{
	// 多线程单例
	static GenshinImpact_TianLi_Resource* pInstance = new GenshinImpact_TianLi_Resource();
	return pInstance;
}

void GenshinImpact_TianLi_Resource::LoadPng_ID2Mat(int IDB, cv::Mat& mat)
{
	IWICStream* pIWICStream = NULL;
	IWICBitmapDecoder* pIDecoder = NULL;
	IWICBitmapFrameDecode* pIDecoderFrame = NULL;
	IWICBitmapSource* bitmap_source = NULL;
	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;
	
	imageResHandle = FindResource(hModu, MAKEINTRESOURCE(IDB), L"PNG");
	imageResDataHandle = LoadResource(hModu, imageResHandle);
	pImageFile = LockResource(imageResDataHandle);
	imageFileSize = SizeofResource(hModu, imageResHandle);
	m_pIWICFactory->CreateStream(&pIWICStream);

	pIWICStream->InitializeFromMemory(
		reinterpret_cast<BYTE*>(pImageFile),
		imageFileSize);
	m_pIWICFactory->CreateDecoderFromStream(
		pIWICStream,                   // The stream to use to create the decoder
		NULL,                          // Do not prefer a particular vendor
		WICDecodeMetadataCacheOnLoad,  // Cache metadata when needed
		&pIDecoder);                   // Pointer to the decoder
	pIDecoder->GetFrame(0, &pIDecoderFrame);

	bitmap_source = pIDecoderFrame;

	UINT width = 0, height = 0, depht = 4;
	bitmap_source->GetSize(&width, &height);

	{
		std::vector<BYTE> buffer(width * height * depht);
		bitmap_source->CopyPixels(NULL, width * depht, static_cast<UINT>(buffer.size()), buffer.data());
		HBITMAP hPngMat = CreateBitmap(width, height, 1, 8 * depht, buffer.data());

		HBitmap2MatAlpha(hPngMat, mat);

		DeleteObject(hPngMat);
	}

	DeleteObject(bitmap_source);
}

void GenshinImpact_TianLi_Resource::LoadPng()
{
	hModu = GetModuleHandle(0);

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pIWICFactory)
	);
	
	LoadPng_ID2Mat(IDB_PNG_GIMAP, GIMAP);
	LoadPng_ID2Mat(IDB_PNG_GIPAIMON, GIPAIMON);
	LoadPng_ID2Mat(IDB_PNG_GIMINIMAP_CAILB, GIMINIMAP_CAILB);
	LoadPng_ID2Mat(IDB_PNG_GIMINIMAP_N, GIMINIMAP_N);
	LoadPng_ID2Mat(IDB_PNG_GIAVATAR, GIAVATAR);
	LoadPng_ID2Mat(IDB_PNG_GISTAR, GISTAR);
	
	LoadPng_ID2Mat(IDB_PNG_GIUID_0, GINUMUID[0]);
	LoadPng_ID2Mat(IDB_PNG_GIUID_1, GINUMUID[1]);
	LoadPng_ID2Mat(IDB_PNG_GIUID_2, GINUMUID[2]);
	LoadPng_ID2Mat(IDB_PNG_GIUID_3, GINUMUID[3]);
	LoadPng_ID2Mat(IDB_PNG_GIUID_4, GINUMUID[4]);
	LoadPng_ID2Mat(IDB_PNG_GIUID_5, GINUMUID[5]);
	LoadPng_ID2Mat(IDB_PNG_GIUID_6, GINUMUID[6]);
	LoadPng_ID2Mat(IDB_PNG_GIUID_7, GINUMUID[7]);
	LoadPng_ID2Mat(IDB_PNG_GIUID_8, GINUMUID[8]);
	LoadPng_ID2Mat(IDB_PNG_GIUID_9, GINUMUID[9]);
	LoadPng_ID2Mat(IDB_PNG_GIUID__, GINUMUID[10]);

	CoUninitialize();
}

bool GenshinImpact_TianLi_Resource::HBitmap2Mat(HBITMAP& _hBmp, cv::Mat& _mat)
{
	//BITMAP操作
	BITMAP bmp;
	GetObject(_hBmp, sizeof(BITMAP), &bmp);
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	//int depth = bmp.bmBitsPixel == 1 ? 1 : 8;
	//mat操作
	cv::Mat v_mat;
	v_mat.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8UC3, nChannels));
	GetBitmapBits(_hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, v_mat.data);
	_mat = v_mat;
	if (nChannels == 4)
	{
		cv::cvtColor(v_mat, _mat, cv::COLOR_RGBA2RGB);
		return true;
	}
	return false;
}

//带Alpha通道的32位Bmp图片
bool GenshinImpact_TianLi_Resource::HBitmap2MatAlpha(HBITMAP& _hBmp, cv::Mat& _mat)
{
	//BITMAP操作
	BITMAP bmp;
	GetObject(_hBmp, sizeof(BITMAP), &bmp);
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	//int depth = bmp.bmBitsPixel == 1 ? 1 : 8;
	//mat操作
	cv::Mat v_mat;
	v_mat.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8UC3, nChannels));
	GetBitmapBits(_hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, v_mat.data);
	_mat = v_mat;
	return true;
}

bool GenshinImpact_TianLi_Resource::Mat2MaskMat(cv::Mat& in, cv::Mat& out)
{
	std::vector<cv::Mat> mv0;
	//通道分离
	split(in, mv0);
	out = mv0[0];

	return false;
}

