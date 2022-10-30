#pragma once
#include <string>
#include <vector>

/* ItemId
{
    "count": 1,
    "iconTag": "风神瞳",
    "itemId": 6
}
*/
struct DataJsonApi_ItemId
{
    int count;
    std::string iconTag;
    int itemId;
	
    std::string to_json()
    {
		std::string json = "{";
		json += "\"count\":" + std::to_string(count) + ",";
		json += "\"iconTag\":\"" + iconTag + "\",";
		json += "\"itemId\":" + std::to_string(itemId) + "";
		json += "}";
        return "{}";// json;
    }
};
typedef std::vector<DataJsonApi_ItemId> DataJsonApi_ItemIdList;
std::string to_json(DataJsonApi_ItemIdList & list)
{
	std::string json = "[";
	for (int i = 0; i < list.size(); i++)
	{
		json += list.at(i).to_json();
		if (i < list.size() - 1)
		{
			json += ",";
		}
	}
	json += "]";
	return json;
}
struct DataJsonApi_ObjectPosition
{
    std::string position;
    double x;
    double y;
};
/* Object
{
    "content": "此处【风龙废墟顶部】有一个【散失的风神瞳】",
    "hiddenFlag": 0,
    "id": 1,
    "itemList": [
        {
            "count": 1,
            "iconTag": "风神瞳",
            "itemId": 6
        }
    ],
    "markerCreatorId": 1,
    "markerTitle": "风神瞳",
    "picture": "",
    "position": "350.20001220703125,-4369.8005001106985",
    "refreshTime": 0,
    "version": 1,
    "videoPath": ""
}
*/
struct DataJsonApi_Object
{
    std::string content;
    int hiddenFlag;
    int id;
    DataJsonApi_ItemIdList itemList;
    int markerCreatorId;
    std::string markerTitle;
    std::string picture;
    DataJsonApi_ObjectPosition position;
    int refreshTime;
    int version;
    std::string videoPath;
    static std::string sql_create_str()
    {
		std::string sql = "CREATE TABLE IF NOT EXISTS Object (";
		sql += "UUID INTEGER PRIMARY KEY,";
		sql += "content TEXT,";
		sql += "hiddenFlag INTEGER,";
		sql += "id INTEGER,";
		sql += "itemList TEXT,";
		sql += "markerCreatorId INTEGER,";
		sql += "markerTitle TEXT,";
		sql += "picture TEXT,";
		sql += "position TEXT,";
		sql += "refreshTime INTEGER,";
		sql += "version INTEGER,";
		sql += "videoPath TEXT";
		sql += ")";
		return sql;
    }
    std::string sql_insert_str()
    {
		return "INSERT INTO Object (content, hiddenFlag, id, itemList, markerCreatorId, markerTitle, picture, position, refreshTime, version, videoPath) VALUES (\"" + content + "\", " + std::to_string(hiddenFlag) + ", " + std::to_string(id) + ", \"" + to_json(itemList) + "\", " + std::to_string(markerCreatorId) + ", \"" + markerTitle + "\", \"" + picture + "\", \"" + position.position + "\", " + std::to_string(refreshTime) + ", " + std::to_string(version) + ", \"" + videoPath + "\");";
    }
};

/* Area
{
    "version": 1,
    "areaId": 6,
    "name": "蒙德",
    "code": "蒙德",
    "content": "蒙德",
    "iconTag": "",
    "parentId": 5,
    "isFinal": true,
    "hiddenFlag": 0,
    "sortIndex": 2
}
*/
struct DataJsonApi_Area
{
    int version;
    int areaId;
    std::string name;
    std::string code;
    std::string content;
    std::string iconTag;
    int parentId;
    bool isFinal;
    int hiddenFlag;
    int sortIndex;
    static std::string sql_create_str()
    {
		std::string sql = "CREATE TABLE IF NOT EXISTS Area (";
		sql += "UUID INTEGER PRIMARY KEY,";
		sql += "version INTEGER,";
		sql += "areaId INTEGER,";
		sql += "name TEXT,";
		sql += "code TEXT,";
		sql += "content TEXT,";
		sql += "iconTag TEXT,";
		sql += "parentId INTEGER,";
		sql += "isFinal INTEGER,";
		sql += "hiddenFlag INTEGER,";
		sql += "sortIndex INTEGER";
		sql += ")";
		return sql;
    }
    std::string sql_insert_str()
    {
		return "INSERT INTO Area (version, areaId, name, code, content, iconTag, parentId, isFinal, hiddenFlag, sortIndex) VALUES (" + std::to_string(version) + ", " + std::to_string(areaId) + ", \"" + name + "\", \"" + code + "\", \"" + content + "\", \"" + iconTag + "\", " + std::to_string(parentId) + ", " + std::to_string(isFinal) + ", " + std::to_string(hiddenFlag) + ", " + std::to_string(sortIndex) + ");";
    }
};

typedef int DataJsonApi_TypeId;
typedef std::vector<DataJsonApi_TypeId> DataJsonApi_TypeIdList;
std::string to_json(DataJsonApi_TypeIdList& list)
{
	std::string json = "[";
	for (int i = 0; i < list.size(); i++)
	{
		json += std::to_string(list.at(i));
		if (i < list.size() - 1)
		{
			json += ",";
		}
	}
	json += "]";
	return json;
}
/* Item
{
    "areaId": 6,
    "count": 10,
    "defaultContent": "此处【蒙德地灵龛】内有一个【华丽宝箱】\r\n开启后可获得【冒险阅历+60、原石×40、风之印 ×10、摩拉×5000】\r\n【大英雄的经验×2、精锻用魔矿×4、xx的「哲学」×2、圣遗物（4星）×2、圣遗物（3星）×4】",
    "defaultCount": 1,
    "defaultRefreshTime": 0,
    "hiddenFlag": 0,
    "iconStyleType": 0,
    "iconTag": "地灵龛",
    "itemId": 31,
    "name": "地灵龛",
    "sortIndex": 100,
    "typeIdList": [
        2,
        11
    ],
    "version": 1
}
*/
struct DataJsonApi_Item
{
    int areaId;
    int count;
    std::string defaultContent;
    int defaultCount;
    int defaultRefreshTime;
    int hiddenFlag;
    int iconStyleType;
    std::string iconTag;
    int itemId;
    std::string name;
    int sortIndex;
    DataJsonApi_TypeIdList typeIdList;
    int version;
    static std::string sql_create_str()
    {
		std::string sql = "CREATE TABLE IF NOT EXISTS Item (";
		sql += "UUID INTEGER PRIMARY KEY,";
		sql += "areaId INTEGER,";
		sql += "count INTEGER,";
		sql += "defaultContent TEXT,";
		sql += "defaultCount INTEGER,";
		sql += "defaultRefreshTime INTEGER,";
		sql += "hiddenFlag INTEGER,";
		sql += "iconStyleType INTEGER,";
		sql += "iconTag TEXT,";
		sql += "itemId INTEGER,";
		sql += "name TEXT,";
		sql += "sortIndex INTEGER,";
		sql += "typeIdList TEXT,";
		sql += "version INTEGER";
		sql += ")";
		return sql;
    }
    std::string sql_insert_str()
    {
		return "INSERT INTO Item (areaId, count, defaultContent, defaultCount, defaultRefreshTime, hiddenFlag, iconStyleType, iconTag, itemId, name, sortIndex, typeIdList, version) VALUES (" + std::to_string(areaId) + ", " + std::to_string(count) + ", \"" + defaultContent + "\", " + std::to_string(defaultCount) + ", " + std::to_string(defaultRefreshTime) + ", " + std::to_string(hiddenFlag) + ", " + std::to_string(iconStyleType) + ", \"" + iconTag + "\", " + std::to_string(itemId) + ", \"" + name + "\", " + std::to_string(sortIndex) + ", \"" + to_json(typeIdList) + "\", " + std::to_string(version) + ");";
    }
};

/* Icon
{
    "iconId": 1,
    "tag": "陨石碎片",
    "typeIdList": [],
    "url": "https://assets.yuanshen.site/icons/106.png",
    "version": 1
}
*/
struct DataJsonApi_Icon
{
    int iconId;
    std::string tag;
    DataJsonApi_TypeIdList typeIdList;
    std::string url;
    int version;
    static std::string sql_create_str()
    {
		std::string sql = "CREATE TABLE IF NOT EXISTS Icon (";
		sql += "UUID INTEGER PRIMARY KEY,";
		sql += "iconId INTEGER,";
		sql += "tag TEXT,";
		sql += "typeIdList TEXT,";
		sql += "url TEXT,";
		sql += "version INTEGER";
		sql += ")";
		return sql;
    }
    std::string sql_insert_str()
    {
		return "INSERT INTO Icon (iconId, tag, typeIdList, url, version) VALUES (" + std::to_string(iconId) + ", \"" + tag + "\", \"" + to_json(typeIdList) + "\", \"" + url + "\", " + std::to_string(version) + ");";
    }
};

/* Type
{
    "version": 1,
    "typeId": 10,
    "iconTag": "icon_宝箱",
    "name": "宝箱品质",
    "content": "宝箱 - {{area.name}}",
    "parentId": 9,
    "isFinal": true,
    "hiddenFlag": 0,
    "sortIndex": 14
}
*/
struct DataJsonApi_Type
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
    static std::string sql_create_str()
    {
		std::string sql = "CREATE TABLE IF NOT EXISTS Type (";
		sql += "UUID INTEGER PRIMARY KEY,";
		sql += "version INTEGER,";
		sql += "typeId INTEGER,";
		sql += "iconTag TEXT,";
		sql += "name TEXT,";
		sql += "content TEXT,";
		sql += "parentId INTEGER,";
		sql += "isFinal INTEGER,";
		sql += "hiddenFlag INTEGER,";
		sql += "sortIndex INTEGER";
		sql += ")";
		return sql;
    }
    std::string sql_insert_str()
    {
		return "INSERT INTO Type (version, typeId, iconTag, name, content, parentId, isFinal, hiddenFlag, sortIndex) VALUES (" + std::to_string(version) + ", " + std::to_string(typeId) + ", \"" + iconTag + "\", \"" + name + "\", \"" + content + "\", " + std::to_string(parentId) + ", " + std::to_string(isFinal) + ", " + std::to_string(hiddenFlag) + ", " + std::to_string(sortIndex) + ");";
    }
};