// GenshinImpact_TianLi_Sqlite.cpp : 定义 DLL 的导出函数。
//

#include "pch.h"
#include "framework.h"
#include "GenshinImpact_TianLi_Sqlite.h"
#include "SqliteImpl.h"
//#include "sqlite3/sqlite3.h"
#include <string>
#include <corecrt_malloc.h>

__stdcall Item::Item(const char* name, int index, double x, double y, double z, int uuid, const char* msg)

{
	this->name = new char[strlen(name) + 1];
	strcpy_s(this->name, strlen(name) + 1, name);
	this->index = index;
	this->x = x;
	this->y = y;
	this->z = z;
	this->uuid = uuid;
	this->msg = new char[strlen(msg) + 1];
	strcpy_s(this->msg, strlen(msg) + 1, msg);
}

__stdcall Item::~Item()
{
	if (name)
	{
		delete[] name;
		name = NULL;
	}
	if (msg)
	{
		delete[] msg;
		msg = NULL;
	}
}

__stdcall ItemsVector::ItemsVector()
{
	names = NULL;
	indexs = NULL;
	xs = NULL;
	ys = NULL;
	zs = NULL;
	uuids = NULL;
	msgs = NULL;
	size = 0;
}

__stdcall ItemsVector::~ItemsVector()
{
	if (names)
	{
		for (int i = 0; i < size; i++)
		{
			if (names[i])
			{
				delete[] names[i];
				names[i] = NULL;
			}
		}
		delete[] names;
		names = NULL;
	}
	if (indexs)
	{
		delete[] indexs;
		indexs = NULL;
	}
	if (xs)
	{
		delete[] xs;
		xs = NULL;
	}
	if (ys)
	{
		delete[] ys;
		ys = NULL;
	}
	if (zs)
	{
		delete[] zs;
		zs = NULL;
	}
	if (uuids)
	{
		delete[] uuids;
		uuids = NULL;
	}
	if (msgs)
	{
		for (int i = 0; i < size; i++)
		{
			if (msgs[i])
			{
				delete[] msgs[i];
				msgs[i] = NULL;
			}
		}
		delete[] msgs;
		msgs = NULL;
	}
}

Item __stdcall ItemsVector::operator[](int i)
{
	return Item(names[i], indexs[i], xs[i], ys[i], zs[i], uuids[i], msgs[i]);
}

void __stdcall ItemsVector::Append(const char* name, int index, double x, double y, double z, int uuid, const char* msg)
{
	if (name == NULL || msg == NULL)
	{
		return;
	}
	if (size == 0)
	{
		names = new char* [1];
		indexs = new int[1];
		xs = new double[1];
		ys = new double[1];
		zs = new double[1];
		uuids = new int[1];
		msgs = new char* [1];

		names[0] = new char[strlen(name) + 1];
		strcpy_s(names[0], strlen(name) + 1, name);
		indexs[0] = index;
		xs[0] = x;
		ys[0] = y;
		zs[0] = z;
		uuids[0] = uuid;
		msgs[0] = new char[strlen(msg) + 1];
		strcpy_s(msgs[0], strlen(msg) + 1, msg);

		size = 1;
	}
	else
	{
		char** names_new = new char* [size + 1];
		int* indexs_new = new int[size + 1];
		double* xs_new = new double[size + 1];
		double* ys_new = new double[size + 1];
		double* zs_new = new double[size + 1];
		int* uuids_new = new int[size + 1];
		char** msgs_new = new char* [size + 1];

		for (int i = 0; i < size; i++)
		{
			names_new[i] = names[i];
			indexs_new[i] = indexs[i];
			xs_new[i] = xs[i];
			ys_new[i] = ys[i];
			zs_new[i] = zs[i];
			uuids_new[i] = uuids[i];
			msgs_new[i] = msgs[i];
		}

		names_new[size] = new char[strlen(name) + 1];
		strcpy_s(names_new[size], strlen(name) + 1, name);
		indexs_new[size] = index;
		xs_new[size] = x;
		ys_new[size] = y;
		zs_new[size] = z;
		uuids_new[size] = uuid;
		msgs_new[size] = new char[strlen(msg) + 1];
		strcpy_s(msgs_new[size], strlen(msg) + 1, msg);

		if (names)
		{
			delete[] names;
			names = NULL;
		}
		if (indexs)
		{
			delete[] indexs;
			indexs = NULL;
		}
		if (xs)
		{
			delete[] xs;
			xs = NULL;
		}
		if (ys)
		{
			delete[] ys;
			ys = NULL;
		}
		if (zs)
		{
			delete[] zs;
			zs = NULL;
		}
		if (uuids)
		{
			delete[] uuids;
			uuids = NULL;
		}
		if (msgs)
		{
			delete[] msgs;
			msgs = NULL;
		}

		names = names_new;
		indexs = indexs_new;
		xs = xs_new;
		ys = ys_new;
		zs = zs_new;
		uuids = uuids_new;
		msgs = msgs_new;
		size++;
	}
}

__stdcall TextVector::TextVector()
{
	text = NULL;
	size = 0;
}

__stdcall TextVector::~TextVector()
{
	if (text)
	{
		for (int i = 0; i < size; i++)
		{
			if (text[i])
			{
				delete text[i];
			}
		}
		delete text;
	}
}

const char* __stdcall TextVector::operator[](int i)
{
	if (i > size)
	{
		return NULL;
	}
	return text[i];
}

void __stdcall TextVector::Append(const char* str)
{
	if (str == NULL)
	{
		return;
	}
	if (text == NULL)
	{
		text = new char* [1];
		text[0] = new char[strlen(str) + 1];
		strcpy_s(text[0], strlen(str) + 1,str);
		size = 1;
	}
	else
	{
		char** tmp = new char* [size + 1];
		for (int i = 0; i < size; i++)
		{
			tmp[i] = text[i];
		}
		tmp[size] = new char[strlen(str) + 1];
		strcpy_s(tmp[size], strlen(str) + 1, str);
		
		if (text)
		{
			delete[] text;
		}
		text = tmp;
		size++;
	}
}

GenshinImpact_TianLi_Sqlite::GenshinImpact_TianLi_Sqlite()
{
	impl = new SqliteImpl();
}

GenshinImpact_TianLi_Sqlite::~GenshinImpact_TianLi_Sqlite()
{
	impl->close();
	delete impl;
}

int __stdcall GenshinImpact_TianLi_Sqlite::OpenSqlite(const char* dbPath)
{
	impl->open(dbPath);
	return 0;
}

int GenshinImpact_TianLi_Sqlite::OpenSqlite(unsigned char* dbData, int size)
{
	impl->open(dbData,size);
	return 0;
}

int __stdcall GenshinImpact_TianLi_Sqlite::CloseSqlite()
{
	impl->close();
	return 0;
}
static int callback(void* data, int argc, char** argv, char** azColName);
int __stdcall GenshinImpact_TianLi_Sqlite::ReadCountry(TextVector &text)
{
	char* errmsg = NULL;
	
	char sql[256]="SELECT distinct layer_addr FROM monoLayer";
	
	impl->exec(sql, nullptr, NULL, &errmsg);
	int rc = impl->exec(sql, callback, (void*)&text, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}

int __stdcall GenshinImpact_TianLi_Sqlite::ReadType(const char* country, TextVector& text)
{
	if (country == NULL)
	{
		return -1;
	}
	char* errmsg = NULL;
	
	// SELECT distinct layer_type FROM monoLayer WHERE layer_addr= '%s'
	std::string sql= "SELECT distinct layer_type FROM monoLayer WHERE layer_addr= '";
	sql += country;
	sql += "'";
	
	int rc = impl->exec(sql.c_str(), callback, (void*)&text, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}
int __stdcall GenshinImpact_TianLi_Sqlite::ReadItem(const char* country, const char* type, TextVector& text)
{
	if (country == NULL || type == NULL)
	{
		return -1;
	}
	char* errmsg = NULL;

	std::string sql = "SELECT distinct layer_name FROM monoLayer WHERE layer_addr='";
	sql += country;
	sql += "' AND layer_type = '";
	sql += type;
	sql += "'";

	int rc = impl->exec(sql.c_str(), callback, (void*)&text, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}

int __stdcall GenshinImpact_TianLi_Sqlite::ReadItems(const char* country, const char* type, const char* item, TextVector& text)
{
	if (country == NULL || type == NULL || item == NULL)
	{
		return -1;
	}

	char* errmsg = NULL;
	// SELECT * FROM items WHERE itemid in (SELECT itemid FROM itemsIndex WHERE kyjgid in (SELECT layer_id FROM monoLayer WHERE layer_addr='$0' AND layer_type = '$1' AND layer_name = '$2'))
	std::string sql = " SELECT poptitle FROM items WHERE itemid in (SELECT itemid FROM itemsIndex WHERE kyjgid in (SELECT layer_id FROM monoLayer WHERE layer_addr='";

	sql += country;
	sql += "' AND layer_type = '";
	sql += type;
	sql += "' AND layer_name = '";
	sql += item;
	sql += "'))";

	int rc = impl->exec(sql.c_str(), callback, (void*)&text, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}
static int callbackItems(void* data, int argc, char** argv, char** azColName);
int __stdcall GenshinImpact_TianLi_Sqlite::ReadItems(const char* country, const char* type, const char* item, ItemsVector& items)
{
	if (country == NULL || type == NULL || item == NULL)
	{
		return -1;
	}

	char* errmsg = NULL;
	// SELECT * FROM items WHERE itemid in (SELECT itemid FROM itemsIndex WHERE kyjgid in (SELECT layer_id FROM monoLayer WHERE layer_addr='$0' AND layer_type = '$1' AND layer_name = '$2'))
	std::string sql = " SELECT * FROM items WHERE itemid in (SELECT itemid FROM itemsIndex WHERE kyjgid in (SELECT layer_id FROM monoLayer WHERE layer_addr='";

	sql += country;
	sql += "' AND layer_type = '";
	sql += type;
	sql += "' AND layer_name = '";
	sql += item;
	sql += "'))";

	int rc = impl->exec(sql.c_str(), callbackItems, (void*)&items, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}
static int callback(void* data, int argc, char** argv, char** azColName) 
{
	TextVector& text = *(TextVector*)data;
	if (argc == 1)
	{
		text.Append(argv[0]);
	}
	return 0;
}

static int callbackItems(void* data, int argc, char** argv, char** azColName) 
{
	ItemsVector& items = *(ItemsVector*)data;
	
	//  24164, 624, 7, 潜身敌营无人知, "完成后可获得杯",-1691.0000305176,283.49993896484

	//  uuid int					-> uuid  0 
	//  itemid int					-> z     1
	//  id int						-> index 2
	//  poptitle varchar(255)		-> name  3 
	//  popupcontent varchar(255)	-> msg   4
	//  x double					-> x     5
	//  y double					-> y     6
	if (argc >= 7)
	{
		// name
		// index
		// x
		// y
		// z
		// uuid
		// msg
		items.Append(
			argv[3],//name
			atoi(argv[2]),//index
			atof(argv[5]),//x
			atof(argv[6]),//y
			atoi(argv[1]),//z
			atoi(argv[0]),//uuid
			argv[4]//msg
		);
	}
	return 0;
}

