#include "pch.h"
#include "SqliteImpl.h"

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

