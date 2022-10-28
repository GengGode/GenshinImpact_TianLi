#pragma once

#include "meojson/json.hpp"
#include "meojson/json5.hpp"

#include "src/type/DataJsonApi.h"
#include <iostream>
#include <filesystem>
#include <regex>

const std::string root = "C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource";
const std::string dir_name = "/POI_JSON_API";
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
}

struct DataJsonApi
{
	std::vector<DataJsonApi_Object> objects;
	std::vector<DataJsonApi_Area> areas;
	std::vector<DataJsonApi_Icon> icons;
	std::vector<DataJsonApi_Item> items;
	std::vector<DataJsonApi_Type> types;
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
		DataJsonApi_Area area_;
		area_.version = area["version"].as_integer();
		area_.areaId = area["areaId"].as_integer();
		area_.name = area["name"].as_string();
		area_.code = area["code"].as_string();
		area_.content = area["content"].as_string();
		area_.iconTag = area["iconTag"].as_string();
		area_.parentId = area["parentId"].as_integer();
		area_.isFinal = area["isFinal"].as_boolean();
		area_.hiddenFlag = area["hiddenFlag"].as_integer();
		area_.sortIndex = area["sortIndex"].as_integer();
		data_json_api.areas.push_back(area_);
	}
}

void load_json_api_item(DataJsonApi& data_json_api)
{
	auto json = load_json_api("/item.json");
	for (auto& item : json)
	{
		DataJsonApi_Item item_;
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
		DataJsonApi_Icon icon_;
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
		DataJsonApi_Type type_;
		type_.version = type["version"].as_integer();
		type_.typeId = type["typeId"].as_integer();
		type_.iconTag = type["iconTag"].as_string();
		type_.name = type["name"].as_string();
		type_.content = type["content"].as_string();
		type_.parentId = type["parentId"].as_integer();
		type_.isFinal = type["isFinal"].as_boolean();
		type_.hiddenFlag = type["hiddenFlag"].as_integer();
		type_.sortIndex = type["sortIndex"].as_integer();
		data_json_api.types.push_back(type_);
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
			DataJsonApi_Object object_;
			object_.content = object["content"].as_string();
			object_.hiddenFlag = object["hiddenFlag"].as_integer();
			object_.id = object["id"].as_integer();
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
				DataJsonApi_ItemId item_;
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