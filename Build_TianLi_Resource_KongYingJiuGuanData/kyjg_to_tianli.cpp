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
//	// ��ɫ��������ͫ
//	objects star_map = stars.filte(x, y, r);
//	// ���ֵ���ͫ
//	objects see_star_list;
//
//	// �����ֵ���ͫ���롾��ɫ��������ͫ�������ƥ��{id,id,dis}
//	std::map<std::pair<int,int>, double> see_star_match;
//	// �������ֵ���ͫ
//	for (auto& see_star : see_star_list)
//	{
//		// ��ĳһ�����ֵ���ͫ���롾��ɫ��������ͫ���ľ����б�{id:dis}
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
//		// �ҵ�������С�ġ���ɫ��������ͫ����id
//		int match_id = min_id;//get_min_dis_id(dis_list);
//
//		// �������С��8
//		if (dis_list[match_id] <= 8)
//		{
//			// ��¼ƥ��
//			see_star_match[{see_star.id, match_id}] = dis_list[match_id];
//		}
//	}
//
//}