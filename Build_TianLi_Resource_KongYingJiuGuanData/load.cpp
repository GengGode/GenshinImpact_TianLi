#include "load.h"

auto load_json(std::string json_file)
{
	std::ifstream ifs(json_file, std::ios::in | std::ios::binary);
	if (!ifs.is_open())
	{
		std::cout << "open file failed" << std::endl;
		return json::array();
	}
	std::string json_str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();
	auto json = json::parse(json_str);
	if (!json)
	{
		std::cout << "parse json failed" << std::endl;
		return json::array();
	}
	return json.value().as_array();
}

void test_load_object()
{
	std::cout << "test_load_object";

	auto object_list = load_object("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource/POI_JSON_API/0.json");

	for (auto& object : object_list)
	{
		std::cout << object.id << "  " << object.markerTitle << "  " << object.position << "\n";
	}
}

void test_load_area()
{
	std::cout << "test_load_area";

	auto area_list = load_area("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource/POI_JSON_API/area.json");

	for (auto& area : area_list)
	{
		std::cout << area.areaId << "  " << area.name << "\n";
	}
}

void test_load_type()
{
	std::cout << "test_load_type";

	auto type_list = load_type("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource/POI_JSON_API/type.json");

	for (auto& type : type_list)
	{
		std::cout << type.typeId << "  " << type.name << "\n";
	}
}

void test_load_icon()
{
	std::cout << "test_load_icon";

	auto icon_list = load_icon("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource/POI_JSON_API/icon.json");

	for (auto& icon : icon_list)
	{
		std::cout << icon.iconId << "  " << icon.tag << "\n";
	}
}

void test_load_item()
{
	std::cout << "test_load_item";

	auto item_list = load_item("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource/POI_JSON_API/item_0.json");

	for (auto& item : item_list)
	{
		std::cout << item.itemId << "  " << item.name << "\n";
	}
}

void test_load_items()
{
	std::cout << "test_load_items";

	auto item_list = load_items("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource/POI_JSON_API", 0, 12);

	for (auto& item : item_list)
	{
		std::cout << item.size() << "\n";
		continue;
		for (auto& item2 : item)
		{
			std::cout << item2.itemId << "  " << item2.name << "\n";
		}
	}
}

void test_load_all()
{
	test_load_object();
	test_load_area();
	test_load_type();
	test_load_icon();
	test_load_item();
	test_load_items();
}

std::vector<std::string> get_all_file_names(std::string path)
{
	std::vector<std::string> files;
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		files.push_back(entry.path().filename().string());
	}
	return files;
}

KongYingJiuGuan load()
{
	std::string path = "C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/resource/POI_JSON_API";

	// 获取path目录下所有文件名
	std::vector<std::string> file_names;
	file_names = get_all_file_names(path);
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
	// 正则匹配 item_0.json item_1.json
	std::vector<std::string> item_list_file_names;
	std::regex reg_item("^item_[0-9]+.json$");
	for (auto& file_name : file_names)
	{
		if (std::regex_match(file_name, sm, reg_item))
		{
			item_list_file_names.push_back(file_name);
		}
	}

	std::vector<std::vector<KYJGObject>> objects;
	for (auto& file_name : object_list_file_names)
	{
		auto object = load_object(path + "/" + file_name);
		objects.push_back(object);
	}

	std::vector<std::vector<KYJGItem>> items;
	for (auto& file_name : item_list_file_names)
	{
		auto item = load_item(path + "/" + file_name);
		items.push_back(item);
	}



	//auto objects = load_objects(path, 0, 200);
	auto area = load_area(path + "\\area.json");
	auto type = load_type(path + "\\type.json");
	auto icon = load_icon(path + "\\icon.json");
	//auto items = load_items(path,0,12);


	KongYingJiuGuan kong_ying_jiu_guan;

	kong_ying_jiu_guan.objects = objects;
	kong_ying_jiu_guan.area = area;
	kong_ying_jiu_guan.type = type;
	kong_ying_jiu_guan.icon = icon;
	kong_ying_jiu_guan.items = items;

	return kong_ying_jiu_guan;
}

std::vector<KYJGObject> load_object(std::string json_file)
{
	std::vector<KYJGObject> object_list;

	auto json_value = load_json(json_file);
	for (auto& value : json_value)
	{
		KYJGObject object;
		object.version = value["version"].as_integer();
		object.id = value["id"].as_integer();
		object.markerTitle = value["markerTitle"].as_string();
		object.position = value["position"].as_string();
		object.content = value["content"].as_string();
		object.picture = value["picture"].as_string();
		object.markerCreatorId = value["markerCreatorId"].as_integer();
		object.pictureCreatorId_is_null = value["pictureCreatorId"].is_null();
		if (!object.pictureCreatorId_is_null)
		{
			object.pictureCreatorId = value["pictureCreatorId"].as_integer();
		}
		object.videoPath = value["videoPath"].as_string();
		object.markerExtraContent_is_null = value["markerExtraContent"].is_null();
		if (!object.markerExtraContent_is_null)
		{
			object.markerExtraContent = value["markerExtraContent"].as_string();
		}
		object.parentId_is_null = value["parentId"].is_null();
		if (!object.parentId_is_null)
		{
			object.parentId = value["parentId"].as_integer();
		}
		object.isRelated_is_null = value["isRelated"].is_null();
		if (!object.isRelated_is_null)
		{
			object.isRelated = value["isRelated"].as_integer();
		}
		object.refreshTime = value["refreshTime"].as_integer();
		object.hiddenFlag = value["hiddenFlag"].as_integer();
		auto& valueList = value["itemList"].as_array();
		for (auto& value : valueList)
		{
			KYJGObject::KYJGObjectItem valueMap;
			valueMap.itemId = value["itemId"].as_integer();
			valueMap.count = value["count"].as_integer();
			valueMap.iconTag = value["iconTag"].as_string();
			object.itemList.push_back(valueMap);
		}
		object_list.push_back(object);
	}
	return object_list;
}

std::vector<KYJGArea> load_area(std::string json_file)
{
	std::vector<KYJGArea> area_list;

	auto json_value = load_json(json_file);
	for (auto& value : json_value)
	{
		KYJGArea area;
		area.version = value["version"].as_integer();
		area.areaId = value["areaId"].as_integer();
		area.name = value["name"].as_string();
		area.content = value["content"].as_string();
		area.iconTag = value["iconTag"].as_string();
		area.isFinal = value["isFinal"].as_boolean();
		area.hiddenFlag = value["hiddenFlag"].as_integer();
		area.sortIndex = value["sortIndex"].as_integer();
		area_list.push_back(area);
	}

	return area_list;
}

std::vector<KYJGType> load_type(std::string json_file)
{
	std::vector<KYJGType> type_list;

	auto json_value = load_json(json_file);
	for (auto& value : json_value)
	{
		KYJGType type;
		type.version = value["version"].as_integer();
		type.typeId = value["typeId"].as_integer();
		type.name = value["name"].as_string();
		type.content = value["content"].as_string();
		type.iconTag = value["iconTag"].as_string();
		type.isFinal = value["isFinal"].as_boolean();
		type.hiddenFlag = value["hiddenFlag"].as_integer();
		type.sortIndex = value["sortIndex"].as_integer();
		type_list.push_back(type);
	}

	return type_list;
}

std::vector<KYJGIcon> load_icon(std::string json_file)
{
	std::vector<KYJGIcon> icon_list;

	auto json_value = load_json(json_file);
	for (auto& value : json_value)
	{
		KYJGIcon icon;
		icon.version = value["version"].as_integer();
		icon.iconId = value["iconId"].as_integer();
		icon.tag = value["tag"].as_string();
		icon.url = value["url"].as_string();
		auto& typeIdList = value["typeIdList"].as_array();
		for (auto& typeId : typeIdList)
		{
			icon.typeIdList.push_back((typeId.as_integer()));
		}
		icon_list.push_back(icon);
	}

	return icon_list;
}

std::vector<KYJGItem> load_item(std::string json_file)
{
	std::vector<KYJGItem> item_list;

	auto json_value = load_json(json_file);
	for (auto& value : json_value)
	{
		KYJGItem item;
		item.version = value["version"].as_integer();
		item.itemId = value["itemId"].as_integer();
		item.name = value["name"].as_string();
		item.areaId = value["areaId"].as_integer();
		item.defaultContent = value["defaultContent"].as_string();
		item.iconTag = value["iconTag"].as_string();
		item.iconStyleType = value["iconStyleType"].as_integer();
		item.hiddenFlag = value["hiddenFlag"].as_integer();
		item.defaultRefreshTime = value["defaultRefreshTime"].as_integer();
		item.sortIndex = value["sortIndex"].as_integer();
		item.defaultCount = value["defaultCount"].as_integer();
		item.count = value["count"].as_integer();
		auto& typeIdList = value["typeIdList"].as_array();
		for (auto& typeId : typeIdList)
		{
			item.typeIdList.push_back((typeId.as_integer()));
		}
		item_list.push_back(item);
	}
	return item_list;
}

std::vector<std::vector<KYJGObject>> load_objects(std::string json_path, int begin_id, int end_id)
{
	std::vector<std::vector<KYJGObject>> objects;
	for (int id = begin_id; id < end_id; id++)
	{
		std::string file_name = json_path + "\\" + std::to_string(id) + ".json";
		std::vector<KYJGObject >object_list;
		object_list = load_object(file_name);
		objects.push_back(object_list);
	}
	return objects;
}

std::vector<std::vector<KYJGItem>> load_items(std::string json_path, int begin_id, int end_id)
{
	std::vector<std::vector<KYJGItem>> items;
	for (int id = begin_id; id < end_id; id++)
	{
		std::string file_name = json_path + "\\item_" + std::to_string(id) + ".json";
		std::vector<KYJGItem> item_list;
		item_list = load_item(file_name);
		items.push_back(item_list);
	}
	return items;

}
