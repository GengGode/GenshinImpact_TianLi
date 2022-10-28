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
};
typedef std::vector<DataJsonApi_ItemId> DataJsonApi_ItemIdList;
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
};

typedef int DataJsonApi_TypeId;
typedef std::vector<DataJsonApi_TypeId> DataJsonApi_TypeIdList;
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
};