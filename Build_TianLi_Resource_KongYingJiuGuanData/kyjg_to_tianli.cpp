#include "kyjg_to_tianli.h"

#include "LoadJson.h"
int to_tianli()
{
	system("chcp 65001");

	//auto data = load_json();
	//delete_sql_db();
	//create_sql_db();
	//insert_sql_db(data);
	//alter_png_sql_db(data);

	
	update_png_sql_db();
	
	return 0;
}

//
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