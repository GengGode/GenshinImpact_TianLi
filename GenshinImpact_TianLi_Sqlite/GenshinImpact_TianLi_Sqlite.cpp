// GenshinImpact_TianLi_Sqlite.cpp : 定义 DLL 的导出函数。
//

#include "pch.h"
#include "framework.h"
#include "GenshinImpact_TianLi_Sqlite.h"
#include "SqliteImpl.h"
#include <vector>
#include <map>
#include <string>
#include <format>
#include <algorithm>
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
	if (i >= size)
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

static int callback_country(void* data, int argc, char** argv, char** azColName);
int __stdcall GenshinImpact_TianLi_Sqlite::ReadCountry(TextVector &text)
{
	char* errmsg = NULL;
	//============================================================================
	char sql[256]="SELECT distinct name FROM Area ORDER BY sortIndex DESC;";
	//============================================================================

	impl->exec(sql, nullptr, NULL, &errmsg);
	int rc = impl->exec(sql, callback_country, (void*)&text, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}
//std::vector<std::pair<std::string, int>> GenshinImpact_TianLi_Sqlite::get_area_map()
//{
//	std::map<int, std::string> area_parent;
//	auto sql_get_parent = "SELECT areaId, name FROM Area WHERE parent = -1 AND hiddenFlag = 0;";
//	impl->exec(sql_get_parent, [](void* data, int argc, char** argv, char** azColName) -> int {
//		auto area_parent = (std::map<int, std::string>*)data;
//		int areaId = atoi(argv[0]);
//		std::string name = argv[1];
//		area_parent->insert(std::make_pair(areaId, name));
//		return 0;
//		}, &area_parent, nullptr);
//	
//	std::vector<std::tuple<std::string, int>> area_map;
//	auto sql_get_area = "SELECT areaId, name FROM Area WHERE parent =";
//	for (auto& [areaId, name] : area_parent)
//	{
//		std::string sql = sql_get_area + std::to_string(areaId) + " AND hiddenFlag = 0;";
//		impl->exec(sql.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
//			auto area_map = (std::vector<std::tuple<std::string, int>>*)data;
//			int areaId = atoi(argv[0]);
//			std::string name = argv[1];
//			area_map->push_back(std::make_tuple(name, areaId));
//			return 0;
//			}, &area_map, nullptr);
//	}
//	
//	
//	
//
//
//	std::vector<std::tuple<std::string, int, int, std::string >> ret;
//	{
//		auto sql = "SELECT name, areaId, parentId, iconTag FROM Area ORDER BY sortIndex DESC;";
//		auto callback = [](void* data, int argc, char** argv, char** azColName)->int {
//			auto ret = (std::vector<std::tuple<std::string, int, int, std::string >>*)data;
//			ret->push_back(std::make_tuple(argv[0], atoi(argv[1]), atoi(argv[2]), argv[3]));
//			return 0;
//		};
//		char* errmsg = NULL;
//		impl->exec(sql, callback, (void*)&ret, &errmsg);
//	}
//	
//
//
//
//
//	//// areaId : name, iconTag, parentId
//	//std::map<int, std::tuple<std::string, std::string, int>> area_map;
//	//for (auto& item : ret)
//	//{
//	//	area_map[std::get<1>(item)] = std::make_tuple(std::get<0>(item), std::get<3>(item), std::get<2>(item));
//	//}
//	//// parent area name : name, iconTag, areaId
//	//std::map<std::string, std::vector<std::tuple<std::string, std::string, int>>> ret2;
//	//for (auto& item : area_map)
//	//{
//	//	auto parent_name = std::get<0>(area_map[std::get<2>(item.second)]);
//	//	ret2[parent_name].push_back(std::make_tuple(std::get<0>(item.second), std::get<1>(item.second), item.first));
//	//}
//	
//	return std::vector<std::pair<std::string, int>>();
//}

static int callback_type(void* data, int argc, char** argv, char** azColName);
int __stdcall GenshinImpact_TianLi_Sqlite::ReadType(const char* area, TextVector& text)
{
	if (area == NULL)
	{
		return -1;
	}
	char* errmsg = NULL;
	
	//============================================================================
	// SELECT distinct layer_type FROM monoLayer WHERE layer_addr= '%s'
	std::string sql = "SELECT distinct name, content FROM Type WHERE isFinal = 1 AND hiddenFlag = 0 ORDER BY sortIndex DESC;";
	//============================================================================

	int rc = impl->exec(sql.c_str(), callback_type, (void*)&text, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}
int __stdcall GenshinImpact_TianLi_Sqlite::ReadItem(const char* area, const char* type, TextVector& text)
{
	if (area == NULL || type == NULL)
	{
		return -1;
	}
	char* errmsg = NULL;

	//============================================================================
	//SELECT distinct areaId FROM Area WHERE name = 'area';
	//SELECT distinct typeId FROM Type WHERE name = 'type';
	//SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type');
	//SELECT distinct name FROM Item WHERE areaId in (SELECT distinct areaId FROM Area WHERE name = 'area') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type'));
	std::string sql = "SELECT distinct name FROM Item WHERE areaId in (SELECT distinct areaId FROM Area WHERE name = '";
	sql += area;
	sql += "') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = '";
	sql += type;
	sql += "'));";
	//sql += country;
	//sql += "' AND layer_type = '";
	//sql += type;
	//sql += "'";
	//============================================================================

	int rc = impl->exec(sql.c_str(), callback_country, (void*)&text, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}

int __stdcall GenshinImpact_TianLi_Sqlite::ReadItems(const char* area, const char* type, const char* item, TextVector& text)
{
	//if (country == NULL || type == NULL || item == NULL)
	if (area == NULL || type == NULL || item == NULL)
	{
		return -1;
	}

	char* errmsg = NULL;
	//============================================================================
	//SELECT distinct areaId FROM Area WHERE name = 'area';
	//SELECT distinct typeId FROM Type WHERE name = 'type';
	// SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type');
	//SELECT distinct itemId FROM Item WHERE name = 'item' AND areaId in (SELECT distinct areaId FROM Area WHERE name = 'area') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type'));
	// SELECT distinct objectId FROM O2M_Object_2_Item WHERE itemId in (SELECT distinct itemId FROM Item WHERE name = 'item' AND areaId in (SELECT distinct areaId FROM Area WHERE name = 'area') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type')));
	// SELECT distinct name FROM Object WHERE objectId in (SELECT distinct objectId FROM O2M_Object_2_Item WHERE itemId in (SELECT distinct itemId FROM Item WHERE name = 'item' AND areaId in (SELECT distinct areaId FROM Area WHERE name = 'area') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type'))));
	std::string sql = "SELECT distinct name FROM Object WHERE objectId in (SELECT distinct objectId FROM O2M_Object_2_Item WHERE itemId in (SELECT distinct itemId FROM Item WHERE name = '";
	sql += item;
	sql += "' AND areaId in (SELECT distinct areaId FROM Area WHERE name = '";
	sql += area;
	sql += "') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = '";
	sql += type;
	sql += "'))));";
	//============================================================================

	int rc = impl->exec(sql.c_str(), callback_country, (void*)&text, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}
static int callback_items(void* data, int argc, char** argv, char** azColName);
int __stdcall GenshinImpact_TianLi_Sqlite::ReadItems(const char* area, const char* type, const char* item, ItemsVector& items)
{
	//if (country == NULL || type == NULL || item == NULL)
	if (area == NULL || type == NULL || item == NULL)
	{
		return -1;
	}

	char* errmsg = NULL;
	//============================================================================
	//SELECT distinct areaId FROM Area WHERE name = 'area';
	//SELECT distinct typeId FROM Type WHERE name = 'type';
	// SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type');
	//SELECT distinct itemId FROM Item WHERE name = 'item' AND areaId in (SELECT distinct areaId FROM Area WHERE name = 'area') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type'));
	// SELECT distinct objectId FROM O2M_Object_2_Item WHERE itemId in (SELECT distinct itemId FROM Item WHERE name = 'item' AND areaId in (SELECT distinct areaId FROM Area WHERE name = 'area') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type')));
	// SELECT distinct name FROM Object WHERE objectId in (SELECT distinct objectId FROM O2M_Object_2_Item WHERE itemId in (SELECT distinct itemId FROM Item WHERE name = 'item' AND areaId in (SELECT distinct areaId FROM Area WHERE name = 'area') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = 'type'))));
	std::string sql = "SELECT distinct * FROM Object WHERE objectId in (SELECT distinct objectId FROM O2M_Object_2_Item WHERE itemId in (SELECT distinct itemId FROM Item WHERE name = '";
	sql += item;
	sql += "' AND areaId in (SELECT distinct areaId FROM Area WHERE name = '";
	sql += area;
	sql += "') AND itemId in (SELECT distinct itemId FROM O2M_Item_2_Type WHERE typeId in (SELECT distinct typeId FROM Type WHERE name = '";
	sql += type;
	sql += "'))));";
	//============================================================================

	int rc = impl->exec(sql.c_str(), callback_items, (void*)&items, &errmsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(errmsg);
	}
	return 0;
}
static int callbackTyoeImage(void* data, int argc, char** argv, char** azColName);
int GenshinImpact_TianLi_Sqlite::GetTypeImage(const char* name, unsigned char*& data, int& size)
{
	if (name == NULL)
	{
		return -1;
	}
	char* errmsg = NULL;
	//============================================================================
	// SELECT iconTag FROM Type WHERE name = 'name';
	// SELECT data FROM Icon WHERE tag = 'iconTag' limit 1;
	std::string sql = "SELECT data FROM Icon WHERE tag = (SELECT iconTag FROM Type WHERE name = '";
	sql += name;
	sql += "') limit 1;";
	//============================================================================
	// TODO：此处 sqlite3_stmt 需要 delete
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(&impl->instance(), sql.c_str(), -1, &stmt, NULL);
	int rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		int len = sqlite3_column_bytes(stmt, 0);
		// TODO：此处内存泄漏
		data = (unsigned char*)sqlite3_column_blob(stmt, 0);
		size = len;
	}
	return 0;
}

int GenshinImpact_TianLi_Sqlite::GetItemImage(const char* name, unsigned char*& data, int& size)
{
	if (name == NULL)
	{
		return -1;
	}
	char* errmsg = NULL;
	//============================================================================
	// SELECT iconTag FROM item_images WHERE name = '$0' limit 1
	std::string sql = " SELECT data FROM Icon WHERE tag = '";
	sql += name;
	sql += "';";
	//============================================================================
	// TODO：此处 sqlite3_stmt 需要 delete
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(&impl->instance(), sql.c_str(), -1, &stmt, NULL);
	int rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		int len = sqlite3_column_bytes(stmt, 0);
		// TODO：此处内存泄漏
		data = (unsigned char*)sqlite3_column_blob(stmt, 0);
		size = len;
	}
	return 0;
}

static int callback_country(void* data, int argc, char** argv, char** azColName) 
{
	TextVector& text = *(TextVector*)data;
	if (argc == 1)
	{
		text.Append(argv[0]);
	}
	return 0;
}
static int callback_type(void* data, int argc, char** argv, char** azColName)
{
	TextVector& text = *(TextVector*)data;
	if (argc == 2)
	{
		std::string name = argv[0];
		//const std::string content = argv[1];

		// 将 content 中的 {{item.name}} 替换为 name
		// TODO:

		text.Append(name.c_str());
	}
	return 0;
}
inline void tranfrom_kyjg_2_tl(double& x, double& y)
{
	x = x / 1.7 + 4480;
	y = y / 1.7 + 3015.5;
}
static int callback_items(void* data, int argc, char** argv, char** azColName) 
{
	ItemsVector& items = *(ItemsVector*)data;

	// 0  UUID               -> uuid
	// 1  content			 -> msg
	// 2  hiddenFlag		 -> 
	// 3  id				 -> index
	// 4  itemList			 -> 
	// 5  markerCreatorId	 -> 
	// 6  markerTitle		 -> name
	// 7  picture			 -> 
	// 8  position			 -> 
	// 9  position_x		 -> x
	// 10 position_y		 -> y
	// 11 refreshTime		 -> 
	// 12 version			 -> z
	// 13 videoPath		     -> 
	
	if (argc >= 7)
	{
		// name
		// index
		// x
		// y
		// z
		// uuid
		// msg
		double x = atof(argv[9]);
		double y = atof(argv[10]);
		tranfrom_kyjg_2_tl(x, y);
		items.Append(
			argv[6],//name
			atoi(argv[3]),//index
			x,//x
			y,//y
			atoi(argv[12]),//z
			atoi(argv[0]),//uuid
			argv[1]//msg
		);
	}
	return 0;
}

int callbackTyoeImage(void* data, int argc, char** argv, char** azColName)
{
	return 0;
}
namespace v1 
{
	std::map<std::pair<int, std::string>, std::vector<std::pair<int, std::string>> > get_area_group_map(GenshinImpact_TianLi_Sqlite* sqlite)
	{
		std::map<int, std::string> area_parent;
		auto sql_get_parent = "SELECT areaId, name FROM Area WHERE parentId = -1 AND hiddenFlag = 0;";
		sqlite->impl->exec(sql_get_parent, [](void* data, int argc, char** argv, char** azColName) -> int {
			auto area_parent = (std::map<int, std::string>*)data;
			int areaId = atoi(argv[0]);
			std::string name = argv[1];
			area_parent->insert(std::make_pair(areaId, name));
			return 0;
			}, &area_parent, nullptr);
		std::map<std::pair<int, std::string>, std::vector<std::pair<int, std::string>> > area_map;
		for (auto& area : area_parent)
		{
			std::vector<std::pair<int, std::string>> area_child;
			std::string sql_get_child = "SELECT areaId, name FROM Area WHERE parentId = " + std::to_string(area.first) + " ORDER BY sortIndex DESC;";
			sqlite->impl->exec(sql_get_child.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
				auto area_child = (std::vector<std::pair<int, std::string>>*)data;
				int areaId = atoi(argv[0]);
				std::string name = argv[1];
				area_child->push_back(std::make_pair(areaId, name));
				return 0;
				}, &area_child, nullptr);
			area_map.insert(std::make_pair(area, area_child));
		}

		return area_map;
	}
	
	std::map<std::pair<std::pair<int, std::string>, std::string>, std::vector<std::pair<std::pair<int, std::string>, std::string>> > get_type_group_map(GenshinImpact_TianLi_Sqlite* sqlite)
	{
		std::vector<std::pair<std::pair<int, std::string>, std::string>> type_parent;
		auto sql_get_parent = "SELECT typeId, name, iconTag FROM Type WHERE parentId = -1 AND hiddenFlag = 0;";
		sqlite->impl->exec(sql_get_parent, [](void* data, int argc, char** argv, char** azColName) -> int {
			auto type_parent = (std::vector<std::pair<std::pair<int, std::string>, std::string>>*)data;
			int typeId = atoi(argv[0]);
			std::string name = argv[1];
			std::string icon = argv[2];
			type_parent->push_back(std::make_pair(std::make_pair(typeId, name), icon));
			return 0;
			}, &type_parent, nullptr);
		std::map<std::pair<std::pair<int, std::string>, std::string>, std::vector<std::pair<std::pair<int, std::string>, std::string>>> type_map;
		for (auto& type : type_parent)
		{
			std::vector<std::pair<std::pair<int, std::string>, std::string>> type_child;
			std::string sql_get_child = "SELECT typeId, name, iconTag FROM Type WHERE parentId = " + std::to_string(type.first.first) + " ORDER BY sortIndex DESC;";
			sqlite->impl->exec(sql_get_child.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
				auto type_child = (std::vector<std::pair<std::pair<int, std::string>, std::string>>*)data;
				int typeId = atoi(argv[0]);
				std::string name = argv[1];
				std::string icon = argv[2];
				type_child->push_back(std::make_pair(std::make_pair(typeId, name), icon));
				return 0;
				}, &type_child, nullptr);
			type_map.insert(std::make_pair(type, type_child));
		}


		return type_map;
	}
}

std::map<Tag, std::vector<Tag> > get_area_group_map(GenshinImpact_TianLi_Sqlite* sqlite)
{
	std::map<Tag, std::vector<Tag>> area_map;
	auto sql_get_parent = "SELECT areaId, name, iconTag FROM Area WHERE parentId = -1 AND hiddenFlag = 0;";
	sqlite->impl->exec(sql_get_parent, [](void* data, int argc, char** argv, char** azColName) -> int {
		auto area_map = (std::map<Tag, std::vector<Tag>>*)data;
		int areaId = atoi(argv[0]);
		std::string name = argv[1];
		std::string iconTag = argv[2];
		
		//c= { Tag::TagType::Area , areaId, name, iconTag };
		Tag tag;
		tag.tag_type = Tag::TagType::Area;
		tag.id = areaId;
		tag.name = name;
		tag.icon_tag = iconTag;
		
		area_map->insert(std::make_pair(tag, std::vector<Tag>()));
		return 0;
		}, &area_map, nullptr);
	for (auto& area : area_map)
	{
		std::string sql_get_child = "SELECT areaId, name, iconTag FROM Area WHERE parentId = " + std::to_string(area.first.id) + " ORDER BY sortIndex DESC;";
		sqlite->impl->exec(sql_get_child.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
			auto area_map = (std::map<Tag, std::vector<Tag>>*)data;
			int areaId = atoi(argv[0]);
			std::string name = argv[1];
			std::string iconTag = argv[2];
			
			//Tag tag = { Tag::TagType::Area , areaId, name, iconTag };
			Tag tag;
			tag.tag_type = Tag::TagType::Area;
			tag.id = areaId;
			tag.name = name;
			tag.icon_tag = iconTag;
			
			area_map->insert(std::make_pair(tag, std::vector<Tag>()));
			return 0;
			}, &area_map, nullptr);
	}

	return area_map;
}


std::map<Tag, std::vector<Tag> > get_type_group_map(GenshinImpact_TianLi_Sqlite* sqlite)
{
	std::map<Tag, std::vector<Tag>> type_map;
	auto sql_get_parent = "SELECT typeId, name, iconTag FROM Type WHERE parentId = -1 AND hiddenFlag = 0;";
	sqlite->impl->exec(sql_get_parent, [](void* data, int argc, char** argv, char** azColName) -> int {
		auto type_map = (std::map<Tag, std::vector<Tag>>*)data;
		int typeId = atoi(argv[0]);
		std::string name = argv[1];
		std::string iconTag = argv[2];

		//Tag tag = { Tag::TagType::Type , typeId, name, iconTag };
		Tag tag;
		tag.tag_type = Tag::TagType::Type;
		tag.id = typeId;
		tag.name = name;
		tag.icon_tag = iconTag;
		

		type_map->insert(std::make_pair(tag, std::vector<Tag>()));
		return 0;
		}, &type_map, nullptr);
	for (auto& type : type_map)
	{
		std::string sql_get_child = "SELECT typeId, name, iconTag FROM Type WHERE parentId = " + std::to_string(type.first.id) + " ORDER BY sortIndex DESC;";
		sqlite->impl->exec(sql_get_child.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
			auto type_map = (std::map<Tag, std::vector<Tag>>*)data;
			int typeId = atoi(argv[0]);
			std::string name = argv[1];
			std::string iconTag = argv[2];

			//Tag tag = { Tag::TagType::Type , typeId, name, iconTag };
			Tag tag;
			tag.tag_type = Tag::TagType::Type;
			tag.id = typeId;
			tag.name = name;
			tag.icon_tag = iconTag;

			type_map->insert(std::make_pair(tag, std::vector<Tag>()));
			return 0;
			}, &type_map, nullptr);
	}

	return type_map;
}