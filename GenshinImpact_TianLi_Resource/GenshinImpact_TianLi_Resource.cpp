// GenshinImpact_TianLi_Resource.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"

#include "GenshinImpact_TianLi_Resource.h"

// 加载 Bitmap 图片 ID_GIMAP
HBITMAP LoadBitmap_GIMAP()
{
	HBITMAP H_GIMAP;
	HMODULE H_Module;
	H_Module = GetModuleHandleW(NULL);
	int e = GetLastError();
	H_GIMAP = LoadBitmap(H_Module, MAKEINTRESOURCE(IDB_BITMAP_GIPAIMON));
	 e = GetLastError();
	
	
	HRSRC H_SQLITE= FindResource(H_Module,MAKEINTRESOURCE(IDR_SQLITE_KYJG), L"SQLITE");
	e = GetLastError();
	if (H_SQLITE == NULL)
	{
		e = GetLastError();
	}
	else
	{
		HGLOBAL H_SqliteDB_Handle= LoadResource(H_Module, H_SQLITE);
		// 判断 H_SqliteDB_Handle 是否为空
		if (H_SqliteDB_Handle == NULL)
		{
			e = GetLastError();
		}
		else
		{
			LPVOID H_SqliteDB_Ptr = LockResource(H_SqliteDB_Handle);
			// 判断 H_SqliteDB_Ptr 是否为空
			if (H_SqliteDB_Ptr == NULL)
			{
				e = GetLastError();
			}
			else
			{
				DWORD SqliteDB_Size = SizeofResource(H_Module, H_SQLITE);
				// 判断 SqliteDB_Size 是否为空
				if (SqliteDB_Size == NULL)
				{
					e = GetLastError();
				}
				else
				{
					// 加载 SQLITE 数据库
					//sqlite3_open_v2(H_SqliteDB_Ptr, &H_SqliteDB, SQLITE_OPEN_READONLY, NULL);
				}
			}
		}
	}


	return H_GIMAP;
}