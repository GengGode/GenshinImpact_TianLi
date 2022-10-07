#pragma once
#include "meojson/json.hpp"

#include <iostream>
#include <set>
#include <vector>
#include <map>

#include <filesystem>
#include <regex>

auto load_json(std::string json_file);

struct KYJGObject
{
	int version;
	int id;
	std::string markerTitle;
	std::string position;
	std::string content;
	std::string picture;
	int markerCreatorId;
	
	bool pictureCreatorId_is_null;
	int pictureCreatorId;
	
	std::string videoPath;
	
	bool markerExtraContent_is_null;
	std::string markerExtraContent;
	
	bool parentId_is_null;
	int parentId;
	
	bool isRelated_is_null;
	int isRelated;
	
	int refreshTime;
	int hiddenFlag;
	struct KYJGObjectItem
	{
		int itemId;
		int count;
		std::string iconTag;
	};
	std::vector<KYJGObjectItem> itemList;
};
std::vector<KYJGObject> load_object(std::string json_file);
std::vector<std::vector<KYJGObject>> load_objects(std::string json_path, int begin_id, int end_id);

struct KYJGArea
{
	int version;
	int areaId;
	std::string name;
	std::string content;
	std::string iconTag;
	bool isFinal;
	int hiddenFlag;
	int sortIndex;
};
std::vector<KYJGArea> load_area(std::string json_file);

struct KYJGType
{
	int version;
	int typeId;
	std::string iconTag;
	std::string name;
	std::string content;
	int parentId;
	bool isFinal;
	int hiddenFlag;
	int sortIndex;
};
std::vector<KYJGType> load_type(std::string json_file);


struct KYJGIcon
{
	int version;
	int iconId;
	std::string tag;
	std::string url;
	std::vector<int> typeIdList;
};
std::vector<KYJGIcon> load_icon(std::string json_file);


struct KYJGItem
{
	int version;
	int itemId;
	std::string name;
	int areaId;
	std::string defaultContent;
	std::string iconTag;
	int iconStyleType;
	int hiddenFlag;
	int defaultRefreshTime;
	int sortIndex;
	int defaultCount;
	int count;
	std::vector<int> typeIdList;
};
std::vector<KYJGItem> load_item(std::string json_file);
std::vector<std::vector<KYJGItem>> load_items(std::string json_path, int begin_id, int end_id);


struct KongYingJiuGuan
{
	std::vector<std::vector<KYJGObject>>objects;
	std::vector<KYJGArea> area;
	std::vector<KYJGType> type;
	std::vector<KYJGIcon> icon;
	std::vector<std::vector<KYJGItem>> items;
};


void test_load_object();
void test_load_area();

void test_load_type();

void test_load_icon();

void test_load_item();

void test_load_items();


std::vector<std::string> get_all_file_names(std::string path);

KongYingJiuGuan load();