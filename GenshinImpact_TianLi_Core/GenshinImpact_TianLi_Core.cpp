#include "pch.h"
#include "GenshinImpact_TianLi_Core.h"

#include "..\GenshinImpact_TianLi_Resource\GenshinImpact_TianLi_Resource.h"
#pragma comment(lib,"GenshinImpact_TianLi_Resource.lib")

#include "..\GenshinImpact_TianLi_Sqlite\GenshinImpact_TianLi_Sqlite.h"
#pragma comment(lib,"GenshinImpact_TianLi_Sqlite.lib")

#include "..\GenshinImpact_TianLi_Track\GenshinImpact_TianLi_Track.h"
#pragma comment(lib,"GenshinImpact_TianLi_Track.lib")

//#include "..\GenshinImpact_TianLi_Map\GenshinImpact_TianLi_Map.h"
//#pragma comment(lib,"GenshinImpact_TianLi_Map.lib")

cv::Mat get_image_tag(GenshinImpact_TianLi_Sqlite * sqlite,const std::string& area, const std::string& type, const std::string& item, const std::string& object)
{
	static cv::Mat empty_object_image;//itemsItemsVector[0].image;
	static bool is_frist = true;
	if (is_frist)
	{
		empty_object_image = cv::Mat(cv::Size(32, 32), CV_8UC4, cv::Scalar(200, 200, 200, 0));
		// �?2为直径绘制宽度为3的白色圆�?
		cv::circle(empty_object_image, cv::Point(16, 16), 12, cv::Scalar(255, 255, 255, 128), 3, cv::LINE_AA);

		is_frist = false;
	}

	cv::Mat image_mat;
	unsigned char* image_buffer = nullptr;
	int size = 0;
	if (!type.empty() && object.empty())
	{
		sqlite->GetTypeImage(type.c_str(), image_buffer, size);
	}
	if (type.empty() && !object.empty())
	{
		sqlite->GetItemImage(object.c_str(), image_buffer, size);
	}
	if (image_buffer != nullptr)
	{
		image_mat = cv::imdecode(cv::Mat(1, size, CV_8UC1, image_buffer), cv::IMREAD_UNCHANGED);
		if (image_mat.empty())
		{
			image_mat = empty_object_image;
		}
		//if (image_mat.rows >= 32 || image_mat.cols >= 32)
		//{
		//	// image_mat = empty_object_image;
		//	cv::resize(image_mat, image_mat, cv::Size(32, 32));
		//}
	}
	else
	{
		image_mat = empty_object_image;
	}
	return image_mat;
}
GenshinImpact_TianLi_Core::GenshinImpact_TianLi_Core()
{
	sqlite = new GenshinImpact_TianLi_Sqlite();
	
	resource = GenshinImpact_TianLi_Resource::GetInstance();
	
	track = new GenshinImpact_TianLi_Track();
	//map = new GenshinImpact_TianLi_Map();

	//=============== init
	// init sqlite
	TianLi::SqliteDbMem db_mem = resource->LoadSqlite_KYJGDB();
	sqlite->OpenSqlite(db_mem.ptr, static_cast<int>(db_mem.size));
	
	// init resource
	std::function<cv::Mat(std::string area, std::string type, std::string item, std::string object)> map_get_image_callback = std::bind(&get_image_tag, sqlite, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	resource->SetGetImageBufferCallback(map_get_image_callback);

	
}

GenshinImpact_TianLi_Core::~GenshinImpact_TianLi_Core()
{
	delete resource;
	delete sqlite;
	delete track;
	//delete map;
}

//GenshinImpact_TianLi_Core& GenshinImpact_TianLi_Core::GetInstance()
//{
//	static GenshinImpact_TianLi_Core instance;
//	return instance;
//}
