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

// 本地资源文件依赖
const std::string root = "C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource";
const std::string dir_name = "/POI_JSON_API";
const std::string sql_db_dir_root = "C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData";
const std::string sql_db_name = "Data.sqlite";
const std::string sql_db_dir = sql_db_dir_root + "/" + sql_db_name;
const std::string download_tmp_dir = "C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/tmp/";


namespace utils {
	auto get_all_file_names(const std::string& path)
	{
		std::vector<std::string> files;
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			files.push_back(entry.path().filename().string());
		}
		return files;
	}
	auto split(const std::string& s, const std::string& separator = ",") {
		std::vector<std::string> tokens;
		size_t begin_pos = 0;
		while (begin_pos < s.size()) {
			const size_t end_pos = s.find_first_of(separator, begin_pos);
			if (end_pos != std::string::npos) {
				if (end_pos > begin_pos) {
					tokens.emplace_back(&s[begin_pos], end_pos - begin_pos);
				}
				begin_pos = end_pos + 1;
			}
			else {
				tokens.emplace_back(&s[begin_pos]);
				break;
			}
		}
		return tokens;
	}
	bool is_file_exist(const std::string& path)
	{
		return std::filesystem::exists(path);
	}
	void delete_file(const std::string& path)
	{
		std::filesystem::remove(path);
	}
	std::string get_file_name(const std::string& url)
	{
		std::regex reg(R"(([^/]+)\.png)");
		std::smatch sm;
		std::regex_search(url, sm, reg);
		auto name = sm[1].str() + ".png";
		return name;
		
	}
	bool download_file(const std::string& url, const std::string& path)
	{
		std::string cmd = "curl -o " + path + " " + url + " -s";
		return system(cmd.c_str()) == 0;		
	}
	struct Data {
		const char* data;
		int size;
	};
	Data read_file_data(std::string path)
	{
		std::ifstream file(path, std::ios::binary);
		if (!file.is_open()) {
			cv::Mat img = cv::imread("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource/问号.png", cv::IMREAD_UNCHANGED);
			cv::imwrite(path, img);
			file.close();
			file = std::ifstream(path, std::ios::binary);
		}
		cv::Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
		if (img.empty())
		{
			cv::Mat img = cv::imread("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource/问号.png", cv::IMREAD_UNCHANGED);
			cv::imwrite(path, img);
			file.close();
			file = std::ifstream(path, std::ios::binary);
		}
		file.seekg(0, std::ios::end);
		auto size = static_cast<int>(file.tellg());
		file.seekg(0, std::ios::beg);
		char* buffer = new char[size];
		file.read(buffer, size);
		file.close();
		return { buffer, size };
	}
	cv::Mat read_png(std::string path)
	{
		cv::Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
		return img;

	}
}

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
		for (auto& object : objects)
		{
			sqlite3_exec(sql_db, object.sql_insert_str().c_str(), NULL, NULL, NULL);
		}
		std::cout << "objects load finish!" << '\n';

	}
	//void insert_objects(sqlite3* sql_db, std::vector<KYJGData::Object>& objects)
	//{
	//	for (auto& object : objects)
	//	{
	//		sqlite3_exec(sql_db, object.sql_insert_str().c_str(), NULL, NULL, NULL);
	//	}
	//}
	//void insert_areas(sqlite3* sql_db, std::vector<KYJGData::Area>& objects)
	//{
	//	for (auto& object : objects)
	//	{
	//		sqlite3_exec(sql_db, object.sql_insert_str().c_str(), NULL, NULL, NULL);
	//	}
	//}
	//void insert_icons(sqlite3* sql_db, std::vector<KYJGData::Icon>& objects)
	//{
	//	for (auto& object : objects)
	//	{
	//		sqlite3_exec(sql_db, object.sql_insert_str().c_str(), NULL, NULL, NULL);
	//	}
	//}
	//void insert_items(sqlite3* sql_db, std::vector<KYJGData::Item>& objects)
	//{
	//	for (auto& object : objects)
	//	{
	//		sqlite3_exec(sql_db, object.sql_insert_str().c_str(), NULL, NULL, NULL);
	//	}
	//}
	//void insert_types(sqlite3* sql_db, std::vector<KYJGData::Type>& objects)
	//{
	//	for (auto& object : objects)
	//	{
	//		sqlite3_exec(sql_db, object.sql_insert_str().c_str(), NULL, NULL, NULL);
	//	}
	//}

	void insert_db(sqlite3* sql_db)
	{
		insert_object<KYJGData::Object>(sql_db, objects);
		insert_object<KYJGData::Area>(sql_db, areas);
		insert_object<KYJGData::Icon>(sql_db, icons);
		insert_object<KYJGData::Item>(sql_db, items);
		insert_object<KYJGData::Type>(sql_db, types);
		//insert_objects(sql_db, objects);
		//insert_areas(sql_db, areas);
		//insert_icons(sql_db, icons);
		//insert_items(sql_db, items);
		//insert_types(sql_db, types);
	}
};

json::array load_json_api(std::string file_name)
{
	//const std::string file_name = "/area.json";
	const std::string file_path = root + dir_name + file_name;
	std::ifstream ifs(file_path, std::ios::in | std::ios::binary);
	if (!ifs.is_open())
	{
		std::cout << "open file failed" << std::endl;
		return json::array();
	}
	std::string json_string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();
	auto json = json::parse(json_string);
	if (!json)
	{
		std::cout << "parse json failed" << std::endl;
		return json::array();
	}
	return json.value().as_array();
}

void load_json_api_area(DataJsonApi& data_json_api)
{
	auto json = load_json_api("/area.json");
	for (auto& area : json)
	{
		data_json_api.areas.push_back(KYJGData::Area(area));
	}
}

void load_json_api_item(DataJsonApi& data_json_api)
{
	auto json = load_json_api("/item.json");
	for (auto& item : json)
	{
		KYJGData::Item item_;
		item_.areaId = item["areaId"].as_integer();
		item_.count = item["count"].as_integer();
		item_.defaultContent = item["defaultContent"].as_string();
		item_.defaultCount = item["defaultCount"].as_integer();
		item_.defaultRefreshTime = item["defaultRefreshTime"].as_integer();
		item_.hiddenFlag = item["hiddenFlag"].as_integer();
		item_.iconStyleType = item["iconStyleType"].as_integer();
		item_.iconTag = item["iconTag"].as_string();
		item_.itemId = item["itemId"].as_integer();
		item_.name = item["name"].as_string();
		item_.sortIndex = item["sortIndex"].as_integer();
		item_.version = item["version"].as_integer();
		for (auto& type_id : item["typeIdList"].as_array())
		{
			item_.typeIdList.push_back(type_id.as_integer());
			
		}
		data_json_api.items.push_back(item_);
	}
}

void load_json_api_icon(DataJsonApi& data_json_api)
{
	auto json = load_json_api("/icon.json");
	for (auto& icon : json)
	{
		KYJGData::Icon icon_;
		icon_.iconId = icon["iconId"].as_integer();
		icon_.tag = icon["tag"].as_string();
		icon_.url = icon["url"].as_string();
		icon_.version = icon["version"].as_integer();
		for (auto& type_id : icon["typeIdList"].as_array())
		{
			icon_.typeIdList.push_back(type_id.as_integer());
		}
		data_json_api.icons.push_back(icon_);
	}
}

void load_json_api_type(DataJsonApi& data_json_api)
{
	auto json = load_json_api("/type.json");
	for (auto& type : json)
	{
		data_json_api.types.push_back(KYJGData::Type(type));
	}
}

void load_json_api_object(DataJsonApi& data_json_api)
{
	// 获取path目录下所有文件名
	std::vector<std::string> file_names;
	file_names = utils::get_all_file_names(root + dir_name);
	
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
		auto json = load_json_api("/" + file_name);
		for (auto& object : json)
		{
			KYJGData::Object object_;
			object_.content = object["content"].as_string();
			object_.hiddenFlag = object["hiddenFlag"].as_integer();
			object_.objectId = object["id"].as_integer();
			object_.markerCreatorId = object["markerCreatorId"].as_integer();
			object_.markerTitle = object["markerTitle"].as_string();
			object_.picture = object["picture"].as_string();
			object_.refreshTime = object["refreshTime"].as_integer();
			object_.version = object["version"].as_integer();
			object_.videoPath = object["videoPath"].as_string();
			object_.position.position = object["position"].as_string();
			auto position_list = utils::split(object_.position.position, ",");
			object_.position.x = std::stod(position_list[0]);
			object_.position.y = std::stod(position_list[1]);
			for (auto& item : object["itemList"].as_array())
			{
				KYJGData::Object_ItemList_One item_;
				item_.count = item["count"].as_integer();
				item_.iconTag = item["iconTag"].as_string();
				item_.itemId = item["itemId"].as_integer();
				object_.itemList.push_back(item_);
			}
			data_json_api.objects.push_back(object_);
		}
	}
}

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
	sqlite3_open(sql_db_dir.c_str(),&sql_db);
	
	auto sql_create_str_list = create_sql_str();
	for (auto& sql_create_str : sql_create_str_list)
	{
		sqlite3_exec(sql_db, sql_create_str.c_str(), NULL, NULL, NULL);
	}
	
	sqlite3_close(sql_db);
}

void insert_sql_db(DataJsonApi& data_json_api)
{
	sqlite3* sql_db;
	sqlite3_open(sql_db_dir.c_str(), &sql_db);
	
	data_json_api.insert_db(sql_db);
	
	sqlite3_close(sql_db);
}

void delete_sql_db()
{
	// 判断sql文件是否存在
	if (utils::is_file_exist(sql_db_dir))
	{
		// 删除sql文件
		utils::delete_file(sql_db_dir);
	}
}

void alter_png_sql_db(DataJsonApi& data_json_api)
{
	sqlite3* sql_db;
	sqlite3_open(sql_db_dir.c_str(), &sql_db);
	
	auto& icons = data_json_api.icons;
	
	std::string sql;
	// ALTER 
	// Icon 表中插入一列data是图片的二进制
	sql = "ALTER TABLE Icon ADD COLUMN data BLOB";
	sqlite3_exec(sql_db, sql.c_str(), NULL, NULL, NULL);
	// Icon 表中插入一列x是图片的宽
	sql = "ALTER TABLE Icon ADD COLUMN x INTEGER";
	sqlite3_exec(sql_db, sql.c_str(), NULL, NULL, NULL);
	// Icon 表中插入一列y是图片的高
	sql = "ALTER TABLE Icon ADD COLUMN y INTEGER";
	sqlite3_exec(sql_db, sql.c_str(), NULL, NULL, NULL);

	for (auto icon : icons)
	{
		auto url = icon.url;
		
		// 下载url到download_tmp_dir
		auto file_name = utils::get_file_name(url);
		auto file_path = download_tmp_dir + file_name;
		if (utils::download_file(url, file_path))
		{
			// 读取图片二进制
			auto data = utils::read_file_data(file_path);
			auto mat = cv::imread(file_path,cv::IMREAD_UNCHANGED);
			const char* sql="";
			// 更新数据库
			sql = "UPDATE Icon SET data = ?, x = ?, y = ? WHERE url = ?";
			sqlite3_stmt* stmt;
			sqlite3_prepare_v2(sql_db, sql, -1, &stmt, NULL);
			sqlite3_bind_blob(stmt, 1, data.data, data.size, SQLITE_STATIC);
			sqlite3_bind_int(stmt, 2, mat.cols);
			sqlite3_bind_int(stmt, 3, mat.rows);
			sqlite3_bind_text(stmt, 4, url.c_str(), url.size(), SQLITE_STATIC);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			// 测试从数据库读取图片
			sql = "SELECT data FROM Icon WHERE url = ?";
			sqlite3_prepare_v2(sql_db, sql, -1, &stmt, NULL);
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
			utils::delete_file(file_path);
		}
		
	}
	
	sqlite3_close(sql_db);
}