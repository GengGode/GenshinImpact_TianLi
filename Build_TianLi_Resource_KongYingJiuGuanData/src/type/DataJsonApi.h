#pragma once
#include <string>
#include <vector>

/* ItemId
{
    "count": 1,
    "iconTag": "����ͫ",
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
    "content": "�˴����������涥������һ����ɢʧ�ķ���ͫ��",
    "hiddenFlag": 0,
    "id": 1,
    "itemList": [
        {
            "count": 1,
            "iconTag": "����ͫ",
            "itemId": 6
        }
    ],
    "markerCreatorId": 1,
    "markerTitle": "����ͫ",
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
    "name": "�ɵ�",
    "code": "�ɵ�",
    "content": "�ɵ�",
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
    "defaultContent": "�˴����ɵµ����衿����һ�����������䡿\r\n������ɻ�á�ð������+60��ԭʯ��40����֮ӡ ��10��Ħ����5000��\r\n����Ӣ�۵ľ����2��������ħ���4��xx�ġ���ѧ����2��ʥ���4�ǣ���2��ʥ���3�ǣ���4��",
    "defaultCount": 1,
    "defaultRefreshTime": 0,
    "hiddenFlag": 0,
    "iconStyleType": 0,
    "iconTag": "������",
    "itemId": 31,
    "name": "������",
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
    "tag": "��ʯ��Ƭ",
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
    "iconTag": "icon_����",
    "name": "����Ʒ��",
    "content": "���� - {{area.name}}",
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