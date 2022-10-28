#include "kyjg_to_tianli.h"

#include "load.h"
#include <cassert>

sqlite3* db;

void open_build_sqlite()
{
	sqlite3_open("../GenshinImpact_TianLi_Resource/resource/build_KongYingJiuGuanData.sqlite", &db);
	// 启用外键约束
	sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
	
}

void close_build_sqlite()
{
	sqlite3_close(db);
}
unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

auto& get_kyjg_data()
{
	static auto kyjg = load();
	return kyjg;
}

int test_()
{
	system("chcp 65001");

	auto kyjg=load();

	//open_build_sqlite();
	//close_build_sqlite();

	std::string url = kyjg.icon[20].url;

	std::regex reg("/icons/.+.png$");
	std::string string;
	string = UrlDecode(url);
	std::smatch sm;
	std::regex_search(string, sm, reg);
	for (auto i : sm)
	{
		string = i;
	}

	std::string png_file_name = string.find("/icons/") != std::string::npos ? string.substr(7) : string;
	
	std::cout << png_file_name << std::endl;

	std::string name = png_file_name;
	std::string param = "curl " + url + " --output \"C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/download/" + name + "\"";
	system(param.c_str());

	std::string path = "C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/download";

	cv::Mat img = cv::imread(path + "/" + png_file_name, -1);

	if (img.empty())
	{
		std::cout << "img.empty()" << std::endl;
		return 0;
	}

	cv::imshow("img", img);
	return 0;
}



void cont_area()
{
	auto kyjg = get_kyjg_data();

	// 创建sqlite测试数据库
	sqlite3* testdb;
	sqlite3_open("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/test.db", &testdb);
	
	// 创建表 area
	// id INTEGER PRIMARY KEY AUTOINCREMENT
	// areaId INTEGER
	// name TEXT
	// content TEXT
	// iconTag TEXT
	// sortIndex INTEGER
	std::string sql = "CREATE TABLE IF NOT EXISTS area (id INTEGER PRIMARY KEY AUTOINCREMENT, areaId INTEGER, name TEXT, content TEXT, iconTag TEXT, sortIndex INTEGER);";
	sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);
	
	for (int i = 0; i < kyjg.area.size(); i++)
	{
		// 准备数据
		int areaId = kyjg.area[i].areaId;
		std::string name = kyjg.area[i].name;
		std::string content = kyjg.area[i].content;
		std::string iconTag = kyjg.area[i].iconTag;
		int sortIndex = kyjg.area[i].sortIndex;

		// 插入数据
		sql = "INSERT INTO area (areaId, name, content, iconTag, sortIndex) VALUES (" + std::to_string(areaId) + ", '" + name + "', '" + content + "', '" + iconTag + "', " + std::to_string(sortIndex) + ");";
		sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);
	}
	
	// 关闭数据库
	sqlite3_close(db);
}
void cont_type()
{
	auto kyjg = get_kyjg_data();

	// 创建sqlite测试数据库
	sqlite3* testdb;
	sqlite3_open("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/test.db", &testdb);
	
	// 创建表 kongyingjiuguan_data_type
	// id INTEGER PRIMARY KEY AUTOINCREMENT
	// typeId INTEGER
	// name TEXT
	// content TEXT
	// iconTag TEXT
	// sortIndex INTEGER
	// parentId INTEGER
	std::string sql = "CREATE TABLE IF NOT EXISTS kongyingjiuguan_data_type (id INTEGER PRIMARY KEY AUTOINCREMENT, typeId INTEGER, name TEXT, content TEXT, iconTag TEXT, sortIndex INTEGER, parentId INTEGER);";
	sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);
	
	for (int i = 0; i < kyjg.type.size(); i++)
	{
		// 准备数据
		int typeId = kyjg.type[i].typeId;
		std::string name = kyjg.type[i].name;
		std::string content = kyjg.type[i].content;
		std::string iconTag = kyjg.type[i].iconTag;
		int sortIndex = kyjg.type[i].sortIndex;
		int parentId = kyjg.type[i].parentId;

		// 插入数据
		sql = "INSERT INTO kongyingjiuguan_data_type (typeId, name, content, iconTag, sortIndex, parentId) VALUES (" + std::to_string(typeId) + ", '" + name + "', '" + content + "', '" + iconTag + "', " + std::to_string(sortIndex) + ", " + std::to_string(parentId) + ");";
		sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);
	}

	// 关闭数据库
	sqlite3_close(db);
}
void cont_icon()
{
	auto kyjg = get_kyjg_data();

	// 创建sqlite测试数据库
	sqlite3* testdb;
	sqlite3_open("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/test.db", &testdb);

	// 创建表 icon
	std::string sql = "CREATE TABLE IF NOT EXISTS icon (id INTEGER PRIMARY KEY AUTOINCREMENT,iconId INTEGER, tag TEXT, name TEXT, icon TEXT, width INTEGER, height INTEGER, data BLOB);";
	sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);


	for (int i = 0; i < kyjg.icon.size(); i++)
	{
		int iconId = kyjg.icon[i].iconId;
		std::string tag = kyjg.icon[i].tag;
		std::string url = kyjg.icon[i].url;

		std::regex reg("/icons/.+.png$");
		std::string string;
		//string = UrlDecode(url);
		string = url;
		std::smatch sm;
		std::regex_search(string, sm, reg);
		for (auto i : sm)
		{
			string = i;
		}

		std::string png_file_name = string.find("/icons/") != std::string::npos ? string.substr(7) : string;

		std::cout << png_file_name << std::endl;

		//std::string name = UrlDecode(png_file_name);
		std::string name = png_file_name;

		//std::string param = "curl " + url + " --output \"C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/download/" + name + "\"";
		//system(param.c_str());

		std::string path = "C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/download";

		cv::Mat img = cv::imread(path + "/" + png_file_name, -1);

		if (img.empty())
		{
			std::cout << "img.empty()" << std::endl;
			continue;
		}

		// 准备数据
		
		sql = "INSERT INTO icon (iconId, tag, name, icon, width, height, data) VALUES (" + std::to_string(iconId) + ", '" + tag + ", '" + name + "', '" + url + "', " + std::to_string(img.cols) + ", " + std::to_string(img.rows) + ", ?);";
		sqlite3_stmt* stmt;
		sqlite3_prepare_v2(testdb, sql.c_str(), -1, &stmt, NULL);
		sqlite3_bind_blob(stmt, 1, img.data, img.total() * img.elemSize(), SQLITE_STATIC);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}

	// 关闭数据库
	sqlite3_close(db);
}
void cont_item()
{
	auto kyjg = get_kyjg_data();

	// 创建sqlite测试数据库
	sqlite3* testdb;
	sqlite3_open("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/test.db", &testdb);
	
	// 创建表 item
	// -> KYJGItem
	// id INTEGER PRIMARY KEY AUTOINCREMENT
	// itemId INTEGER
	// name TEXT
	// content TEXT
	// iconTag TEXT
	// sortIndex INTEGER
	// areaId INTEGER
	// iconStyleType INTEGER
	// defaultRefreshTime INTEGER
	// count INTEGER
	// typeIdList TEXT
	std::string sql = "CREATE TABLE IF NOT EXISTS item (id INTEGER PRIMARY KEY AUTOINCREMENT, itemId INTEGER, name TEXT, content TEXT, iconTag TEXT, sortIndex INTEGER, areaId INTEGER, iconStyleType INTEGER, defaultRefreshTime INTEGER, count INTEGER, typeIdList TEXT);";
	sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);
	
	for(auto& item : kyjg.items)
	{
		for (int i = 0; i < item.size(); i++)
		{
			int itemId = item[i].itemId;
			std::string name = item[i].name;
			std::string content = item[i].defaultContent;
			std::string iconTag = item[i].iconTag;
			int sortIndex = item[i].sortIndex;
			int areaId = item[i].areaId;
			int iconStyleType = item[i].iconStyleType;
			int defaultRefreshTime = item[i].defaultRefreshTime;
			int count = item[i].count;
			std::string typeIdList = [=] (const auto & type_id_list) {
				std::string str;
				for(auto type_id: type_id_list)
				{
					str += std::to_string(type_id);
					if (type_id != type_id_list.back())
					{
						str += ",";
					}
				}
				return str;
			}(item[i].typeIdList);

			// 插入数据
			sql = "INSERT INTO item (itemId, name, content, iconTag, sortIndex, areaId, iconStyleType, defaultRefreshTime, count, typeIdList) VALUES (" + std::to_string(itemId) + ", '" + name + "', '" + content + "', '" + iconTag + "', " + std::to_string(sortIndex) + ", " + std::to_string(areaId) + ", " + std::to_string(iconStyleType) + ", " + std::to_string(defaultRefreshTime) + ", " + std::to_string(count) + ", '" + typeIdList + "');";
			sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);
		}
	}

	// 关闭数据库
	sqlite3_close(testdb);

}
void cont_items()
{

}
void cont_object()
{
	auto kyjg = get_kyjg_data();

	// 创建sqlite测试数据库
	sqlite3* testdb;
	sqlite3_open("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/test.db", &testdb);
	sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);

	/*
			struct KYJGObjectItem
		{
			int itemId;
			int count;
			std::string iconTag;
		};
	*/
	// 创建表 object_item
	// -> KYJGObjectItem
	// id INTEGER PRIMARY KEY AUTOINCREMENT
	// itemId INTEGER
	// count INTEGER
	// iconTag TEXT
	std::string sql = "CREATE TABLE IF NOT EXISTS object_item (id INTEGER PRIMARY KEY AUTOINCREMENT, itemId INTEGER, count INTEGER, iconTag TEXT);";
	sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);
	
	// 创建表 object
	// -> KYJGObject
	/*
	struct KYJGObject
	{
		int id;
		std::string markerTitle;
		std::string position;
		std::string content;
		std::string picture;
		std::string videoPath;
		std::string markerExtraContent;
	
		int parentId;

		std::vector<KYJGObjectItem> itemList;
	}
	*/
	// id INTEGER PRIMARY KEY AUTOINCREMENT
	// objectId INTEGER
	// name TEXT
	// content TEXT
	// position TEXT
	// picture TEXT
	// videoPath TEXT
	// markerExtraContent TEXT
	// parentId INTEGER
	// 外键约束
	// FOREIGN KEY(itemId) REFERENCES object_item(itemId)
	
	sql = "CREATE TABLE IF NOT EXISTS object (id INTEGER PRIMARY KEY AUTOINCREMENT, objectId INTEGER, name TEXT, content TEXT, position TEXT, picture TEXT, videoPath TEXT, markerExtraContent TEXT, parentId INTEGER, FOREIGN KEY(itemId) REFERENCES object_item(itemId));";
	sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);
	
	for (auto& object : kyjg.objects)
	{
		for (int i = 0; i < object.size(); i++)
		{
			int objectId = object[i].id;
			std::string name = object[i].markerTitle;
			std::string content = object[i].content;
			std::string position = object[i].position;
			std::string picture = object[i].picture;
			std::string videoPath = object[i].videoPath;
			std::string markerExtraContent = object[i].markerExtraContent;
			int parentId = object[i].parentId;

			// 插入数据
			sql = "INSERT INTO object (objectId, name, content, position, picture, videoPath, markerExtraContent, parentId) VALUES (" + std::to_string(objectId) + ", '" + name + "', '" + content + "', '" + position + "', '" + picture + "', '" + videoPath + "', '" + markerExtraContent + "', " + std::to_string(parentId) + ");";
			sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);

			for (int j = 0; j < object[i].itemList.size(); j++)
			{
				int itemId = object[i].itemList[j].itemId;
				int count = object[i].itemList[j].count;
				std::string iconTag = object[i].itemList[j].iconTag;

				// 插入数据
				sql = "INSERT INTO object_item (itemId, count, iconTag) VALUES (" + std::to_string(itemId) + ", " + std::to_string(count) + ", '" + iconTag + "');";
				sqlite3_exec(testdb, sql.c_str(), NULL, NULL, NULL);
			}
		}
	}
	
	// 关闭数据库
	sqlite3_close(testdb);
}
void cont_objects()
{

}

void test_icon()
{
	// 读取数据库
	sqlite3* db;
	sqlite3_open("C:/Users/GengG/source/repos/GenshinImpact_TianLi/Build_TianLi_Resource_KongYingJiuGuanData/test.db", &db);

	// 查询数据
	std::string sql;

	// 查询所有数据
	sql = "SELECT * FROM icon;";

	// 查询id为1的数据
	// sql = "SELECT * FROM kongyingjiuguan_data_icon WHERE id = 1;";

	// 查询name为2.png的数据
	// sql = "SELECT * FROM kongyingjiuguan_data_icon WHERE name = '2.png';";

	// 查询icon为https://assets.yuanshen.site/icons/2.png的数据
	// sql = "SELECT * FROM kongyingjiuguan_data_icon WHERE icon = 'https://assets.yuanshen.site/icons/2.png';";

	// 查询width为64的数据
	// sql = "SELECT * FROM kongyingjiuguan_data_icon WHERE width = 64;";

	// 查询height为64的数据
	// sql = "SELECT * FROM kongyingjiuguan_data_icon WHERE height = 64;";

	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		// 获取id
		int id = sqlite3_column_int(stmt, 0);
		std::cout << "id: " << id << std::endl;

		// 获取name
		std::string name = (char*)sqlite3_column_text(stmt, 1);
		std::cout << "name: " << name << std::endl;

		// 获取icon
		std::string icon = (char*)sqlite3_column_text(stmt, 2);
		std::cout << "icon: " << icon << std::endl;

		// 获取width
		int width = sqlite3_column_int(stmt, 3);
		std::cout << "width: " << width << std::endl;

		// 获取height
		int height = sqlite3_column_int(stmt, 4);
		std::cout << "height: " << height << std::endl;

		// 获取data
		int size = sqlite3_column_bytes(stmt, 5);
		std::cout << "size: " << size << std::endl;
		const unsigned char* data = (unsigned char*)sqlite3_column_blob(stmt, 5);
		cv::Mat img(height, width, CV_8UC4, (void*)data);
		if (img.empty())
		{
			std::cout << "img.empty()" << std::endl;
			continue;
		}

		cv::imshow("img", img);
		cv::waitKey(10);
	}

}

#include "LoadJson.h"
int to_tianli()
{
	//cont_area();
	//cont_type();
	//cont_icon();
	//cont_item();
	//cont_items();
	//cont_object();
	//
	//if(0)
	//{
	//	test_icon();

	//}
	//
	auto data = load_json();
	
	//cv::FileStorage fs("/resource/DataJson.xml", cv::FileStorage::WRITE);
	//fs << "data" << data;
	//fs.release();
	//// 测试
	//cv::FileStorage fs_test("/resource/DataJson.xml", cv::FileStorage::READ);
	//DataJsonApi data_test;
	//fs["data"] >> data_test;
	//fs_test.release();
	//
	//if (data.objects.size() == data_test.objects.size())
	//{
	//	return 0;
	//}
	//else
	//{
	//	return -1;
	//}
	
	
	
	
	return 0;
}

//void test()
//{	
//	objects stars;
//	
//	double x=0, y=0, r=0;
//	
//	// 角色附近的神瞳
//	objects star_map = stars.filte(x, y, r);
//	// 发现的神瞳
//	objects see_star_list;
//
//	// 【发现的神瞳】与【角色附近的神瞳】的最佳匹配{id,id,dis}
//	std::map<std::pair<int,int>, double> see_star_match;
//	// 遍历发现的神瞳
//	for (auto& see_star : see_star_list)
//	{
//		// 【某一个发现的神瞳】与【角色附近的神瞳】的距离列表{id:dis}
//		std::map<int, double>dis_list;
//		int min_id = -1;
//		for (auto& star : star_map)
//		{
//			//double dis = sqrt((star.x - see_star.x) ^ 2 + (star.y - see_star.y) ^ 2);
//			double dis = sqrt(
//				(star.x - see_star.x) * (star.x - see_star.x) + 
//				(star.y - see_star.y) * (star.y - see_star.y)
//			);
//			
//			dis_list[star.id] = dis;
//			
//			if (min_id == -1 || dis < dis_list[min_id])
//			{
//				min_id = star.id;
//			}
//		}
//		// 找到距离最小的【角色附近的神瞳】的id
//		int match_id = min_id;//get_min_dis_id(dis_list);
//
//		// 如果距离小于8
//		if (dis_list[match_id] <= 8)
//		{
//			// 记录匹配
//			see_star_match[{see_star.id, match_id}] = dis_list[match_id];
//		}
//	}
//
//}