#pragma once
#include "resource.h"
#include <wincodec.h>

namespace TianLi 
{	
	struct SqliteDbMem {
		unsigned char *ptr;
		__int64 size;
	};

	extern const char* Temp_Sqlite_FileName;

	HBITMAP LoadBitmap_GIMAP();

	SqliteDbMem LoadSqlite_KYJGDB();
}