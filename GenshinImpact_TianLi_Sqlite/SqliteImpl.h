#pragma once

#include "sqlite3/sqlite3.h"

class SqliteImpl
{
	sqlite3 * db=nullptr;
	bool isOpen=false;
public:
	SqliteImpl();
	~SqliteImpl();
	
	int open(const char* dbName);
	int open(unsigned char * pData,int size);
	int close();
	int exec(const char * sql, int (*callback)(void*,int,char**,char**), void * data, char** errmsg);
	sqlite3& instance() { return *db; }
	
};

