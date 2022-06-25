#include "pch.h"
#include "SqliteImpl.h"

//typedef struct sqlite3_vfs sqlite3_vfs;
//typedef void (*sqlite3_syscall_ptr)(void);
//struct sqlite3_vfs {
//	int iVersion;            /* Structure version number (currently 3) */
//	int szOsFile;            /* Size of subclassed sqlite3_file */
//	int mxPathname;          /* Maximum file pathname length */
//	sqlite3_vfs* pNext;      /* Next registered VFS */
//	const char* zName;       /* Name of this virtual file system */
//	void* pAppData;          /* Pointer to application-specific data */
//	int (*xOpen)(sqlite3_vfs*, const char* zName, sqlite3_file*,
//		int flags, int* pOutFlags);
//	int (*xDelete)(sqlite3_vfs*, const char* zName, int syncDir);
//	int (*xAccess)(sqlite3_vfs*, const char* zName, int flags, int* pResOut);
//	int (*xFullPathname)(sqlite3_vfs*, const char* zName, int nOut, char* zOut);
//	void* (*xDlOpen)(sqlite3_vfs*, const char* zFilename);
//	void (*xDlError)(sqlite3_vfs*, int nByte, char* zErrMsg);
//	void (*(*xDlSym)(sqlite3_vfs*, void*, const char* zSymbol))(void);
//	void (*xDlClose)(sqlite3_vfs*, void*);
//	int (*xRandomness)(sqlite3_vfs*, int nByte, char* zOut);
//	int (*xSleep)(sqlite3_vfs*, int microseconds);
//	int (*xCurrentTime)(sqlite3_vfs*, double*);
//	int (*xGetLastError)(sqlite3_vfs*, int, char*);
//	/*
//	** The methods above are in version 1 of the sqlite_vfs object
//	** definition.  Those that follow are added in version 2 or later
//	*/
//	int (*xCurrentTimeInt64)(sqlite3_vfs*, sqlite3_int64*);
//	/*
//	** The methods above are in versions 1 and 2 of the sqlite_vfs object.
//	** Those below are for version 3 and greater.
//	*/
//	int (*xSetSystemCall)(sqlite3_vfs*, const char* zName, sqlite3_syscall_ptr);
//	sqlite3_syscall_ptr(*xGetSystemCall)(sqlite3_vfs*, const char* zName);
//	const char* (*xNextSystemCall)(sqlite3_vfs*, const char* zName);
//	/*
//	** The methods above are in versions 1 through 3 of the sqlite_vfs object.
//	** New fields may be appended in future versions.  The iVersion
//	** value will increment whenever this happens.
//	*/
//};

SqliteImpl::SqliteImpl()
{
}

SqliteImpl::~SqliteImpl()
{
	close();
}

int SqliteImpl::open(const char* dbName)
{
	return sqlite3_open(dbName, &db);
}

int SqliteImpl::close()
{
	return sqlite3_close(db);
}

int SqliteImpl::exec(const char* sql, int (*callback)(void*, int, char**, char**), void* data,char** errmsg)
{
	return sqlite3_exec(db,sql,callback,data, errmsg);
}

