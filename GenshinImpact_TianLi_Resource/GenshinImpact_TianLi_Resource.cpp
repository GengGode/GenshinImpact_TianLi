// GenshinImpact_TianLi_Resource.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"

#include "GenshinImpact_TianLi_Resource.h"
#include <cstdio>

namespace TianLi
{
	const char* Temp_Sqlite_FileName = "Tamp~";

	// 加载 Bitmap 图片 ID_GIMAP
	HBITMAP LoadBitmap_GIMAP()
	{
		HMODULE H_Module = GetModuleHandleW(NULL);
		HBITMAP H_GIMAP = LoadBitmap(H_Module, MAKEINTRESOURCE(IDB_BITMAP_GIPAIMON));
		return H_GIMAP;
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