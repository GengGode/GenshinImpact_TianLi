// GenshinImpact_TianLi_Resource.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"

#include "GenshinImpact_TianLi_Resource.h"
#include <vector>

namespace TianLi
{
	const char* Temp_Sqlite_FileName = "Tamp~";
	
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
			bitmap_source->CopyPixels(NULL, width * depht, buffer.size(), buffer.data());

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
}