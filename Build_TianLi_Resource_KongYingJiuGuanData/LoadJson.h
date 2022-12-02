#pragma once
#include <iostream>
#include <filesystem>
#include <regex>
// 第三方库 Opencv
#include <opencv2/opencv.hpp>
// 第三方库 MeoJson
#include "meojson/json.hpp"
#include "meojson/json5.hpp"
// 第三方库 Sqlite3
#include "sqlite3/sqlite3.h"
// src
#include "src/type/DataJsonApi.h"

struct DataJsonApi
{
	std::vector<KYJGData::Object> objects;
	std::vector<KYJGData::Area> areas;
	std::vector<KYJGData::Icon> icons;
	std::vector<KYJGData::Item> items;
	std::vector<KYJGData::Type> types;
	template<typename Type>
	void insert_object(sqlite3* sql_db, std::vector<Type>& objects)
	{
		int it = 0;
		int end = objects.size();
		for (auto& object : objects)
		{
			sqlite3_exec(sql_db, object.sql_insert_str().c_str(), nullptr, nullptr, nullptr);
			// 控制台输出运行进度条
			std::cout << "\r" << "insert object: " << (it++) << "/" << end << std::flush;
		}
		std::cout << "objects load finish!" << '\n';
	}
	void insert_db(sqlite3* sql_db)
	{
		insert_object<KYJGData::Object>(sql_db, objects);
		insert_object<KYJGData::Area>(sql_db, areas);
		insert_object<KYJGData::Icon>(sql_db, icons);
		insert_object<KYJGData::Item>(sql_db, items);
		insert_object<KYJGData::Type>(sql_db, types);
	}
};


void load_json_api_area(DataJsonApi& data_json_api)
{
	auto json = utils::load_json_api("/area.json");
	for (auto& area : json)
	{
		data_json_api.areas.push_back(KYJGData::Area(area));
	}
}

void load_json_api_item(DataJsonApi& data_json_api)
{
	auto json = utils::load_json_api("/item.json");
	for (auto& item : json)
	{
		data_json_api.items.push_back(KYJGData::Item(item));
	}
}

void load_json_api_icon(DataJsonApi& data_json_api)
{
	auto json = utils::load_json_api("/icon.json");
	for (auto& icon : json)
	{
		data_json_api.icons.push_back(KYJGData::Icon(icon));
	}
}

void load_json_api_type(DataJsonApi& data_json_api)
{
	auto json = utils::load_json_api("/type.json");
	for (auto& type : json)
	{
		data_json_api.types.push_back(KYJGData::Type(type));
	}
}

void load_json_api_object(DataJsonApi& data_json_api)
{
	// 获取path目录下所有文件名
	std::vector<std::string> file_names;
	file_names = utils::get_all_file_names(config::root + config::dir_name);
	
	// 正则匹配 0.json 1.json
	std::vector<std::string> object_list_file_names;
	std::regex reg_object("^[0-9]+.json$");
	std::smatch sm;
	for (auto& file_name : file_names)
	{
		if (std::regex_match(file_name, sm, reg_object))
		{
			object_list_file_names.push_back(file_name);
		}
	}
	
	for (auto& file_name : object_list_file_names)
	{
		auto json = utils::load_json_api("/" + file_name);
		for (auto& object : json)
		{			
			data_json_api.objects.push_back(KYJGData::Object(object));
		}
	}
}
//======================================================================================================================
auto load_json()
{
	DataJsonApi data_json_api;
	load_json_api_area(data_json_api);
	load_json_api_item(data_json_api);
	load_json_api_icon(data_json_api);
	load_json_api_type(data_json_api);
	load_json_api_object(data_json_api);
	return data_json_api;
}
//======================================================================================================================

auto create_sql_str()
{
	std::vector<std::string> sqls;
	sqls.push_back(KYJGData::Object::sql_create_str());
	sqls.push_back(KYJGData::Area::sql_create_str());
	sqls.push_back(KYJGData::Item::sql_create_str());
	sqls.push_back(KYJGData::Icon::sql_create_str());
	sqls.push_back(KYJGData::Type::sql_create_str());
	return sqls;
}

void create_sql_db()
{
	sqlite3* sql_db;
	sqlite3_open(config::sql_db_dir.c_str(),&sql_db);
	
	auto sql_create_str_list = create_sql_str();
	for (auto& sql_create_str : sql_create_str_list)
	{
		sqlite3_exec(sql_db, sql_create_str.c_str(), nullptr, nullptr, nullptr);
	}
	
	sqlite3_close(sql_db);
}

void insert_sql_db(DataJsonApi& data_json_api)
{
	sqlite3* sql_db;
	sqlite3_open(config::sql_db_dir.c_str(), &sql_db);
	
	data_json_api.insert_db(sql_db);
	
	sqlite3_close(sql_db);
}

void delete_sql_db()
{
	// 判断sql文件是否存在
	if (utils::is_file_exist(config::sql_db_dir))
	{
		// 删除sql文件
		utils::delete_file(config::sql_db_dir);
	}
}

void alter_png_sql_db(DataJsonApi& data_json_api)
{
	sqlite3* sql_db;
	sqlite3_open(config::sql_db_dir.c_str(), &sql_db);
	
	auto& icons = data_json_api.icons;
	
	std::string sql;
	// ALTER 
	// Icon 表中插入一列data是图片的二进制
	sql = "ALTER TABLE Icon ADD COLUMN data BLOB";
	sqlite3_exec(sql_db, sql.c_str(), nullptr, nullptr, nullptr);
	// Icon 表中插入一列x是图片的宽
	sql = "ALTER TABLE Icon ADD COLUMN x INTEGER";
	sqlite3_exec(sql_db, sql.c_str(), nullptr, nullptr, nullptr);
	// Icon 表中插入一列y是图片的高
	sql = "ALTER TABLE Icon ADD COLUMN y INTEGER";
	sqlite3_exec(sql_db, sql.c_str(), nullptr, nullptr, nullptr);

	for (auto icon : icons)
	{
		auto url = icon.url;
		
		// 下载url到download_tmp_dir
		auto file_name = utils::get_file_name(url);
		auto name = utils::url_decode(file_name);
		std::cout << name << std::endl;
		auto file_path = config::download_tmp_dir + file_name;
		if (utils::download_file(url, file_path))
		{
			// 读取图片二进制
			auto data = utils::read_file_data(file_path);
			auto mat = cv::imread(file_path,cv::IMREAD_UNCHANGED);
			const char* sql="";
			// 更新数据库
			sql = "UPDATE Icon SET data = ?, x = ?, y = ? WHERE url = ?";
			sqlite3_stmt* stmt;
			sqlite3_prepare_v2(sql_db, sql, -1, &stmt, nullptr);
			sqlite3_bind_blob(stmt, 1, data.data, data.size, SQLITE_STATIC);
			sqlite3_bind_int(stmt, 2, mat.cols);
			sqlite3_bind_int(stmt, 3, mat.rows);
			sqlite3_bind_text(stmt, 4, url.c_str(), url.size(), SQLITE_STATIC);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			// 测试从数据库读取图片
			sql = "SELECT data FROM Icon WHERE url = ?";
			sqlite3_prepare_v2(sql_db, sql, -1, &stmt, nullptr);
			sqlite3_bind_text(stmt, 1, url.c_str(), url.size(), SQLITE_STATIC);
			if (sqlite3_step(stmt) == SQLITE_ROW)
			{
				auto data = sqlite3_column_blob(stmt, 0);
				auto size = sqlite3_column_bytes(stmt, 0);
				// 转为cv::Mat
				auto mat = cv::imdecode(cv::Mat(1, size, CV_8UC1, (void*)data), cv::IMREAD_COLOR);
				if (mat.empty()) continue;
				// 显示图片
				cv::imshow("test", mat);
				cv::waitKey(100);
			}
			else
			{
				
			}
			// 删除图片
			//utils::delete_file(file_path);
		}
		
	}
	
	sqlite3_close(sql_db);
}


void update_png_sql_db()
{
	sqlite3* sql_db;
	sqlite3_open(config::sql_db_dir.c_str(), &sql_db);

	std::string sql;
	// Select Icon
	sql = "SELECT url FROM Icon";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(sql_db, sql.c_str(), -1, &stmt, nullptr);
	
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		auto url = (const char*)sqlite3_column_text(stmt, 0);
		// 下载url到download_tmp_dir
		auto file_name = utils::get_file_name(url);
		auto name = utils::url_decode(file_name);
		std::cout << name << " ";
		std::cout <<"[ "<< url <<" ]"<< std::endl;
		auto file_path = config::download_tmp_dir + file_name;
		if (utils::download_file(url, file_path))
		{
			// 读取图片二进制
			auto data = utils::read_file_data(file_path);
			auto mat = cv::imread(file_path, cv::IMREAD_UNCHANGED);
			const char* sql = "";
			// 更新数据库
			sql = "UPDATE Icon SET data = ?, x = ?, y = ? WHERE url = ?";
			sqlite3_stmt* stmt;
			sqlite3_prepare_v2(sql_db, sql, -1, &stmt, nullptr);
			sqlite3_bind_blob(stmt, 1, data.data, data.size, SQLITE_STATIC);
			sqlite3_bind_int(stmt, 2, mat.cols);
			sqlite3_bind_int(stmt, 3, mat.rows);
			sqlite3_bind_text(stmt, 4, url, strlen(url), SQLITE_STATIC);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			// 测试从数据库读取图片
			sql = "SELECT data FROM Icon WHERE url = ?";
			sqlite3_prepare_v2(sql_db, sql, -1, &stmt, nullptr);
			sqlite3_bind_text(stmt, 1, url, strlen(url), SQLITE_STATIC);
			if (sqlite3_step(stmt) == SQLITE_ROW)
			{
				auto data = sqlite3_column_blob(stmt, 0);
				auto size = sqlite3_column_bytes(stmt, 0);
				// 转为cv::Mat
				auto mat = cv::imdecode(cv::Mat(1, size, CV_8UC1, (void*)data), cv::IMREAD_COLOR);
				if (mat.empty()) continue;
				// 显示图片
				cv::imshow("test", mat);
				cv::waitKey(100);
			}
			else
			{

			}
			// 删除图片
			//utils::delete_file(file_path);
		}
	}
	sqlite3_close(sql_db);
}