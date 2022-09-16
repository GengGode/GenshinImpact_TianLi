// Build_TianLi_Resource_GIMAP_KEYPOINT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

int main()
{
	cv::Mat map = cv::imread("../GenshinImpact_TianLi_Resource/resource/GIMAP.png");
	
	std::vector<cv::KeyPoint> Kp_Map;
	cv::Mat Dp_Map;
	
	cv::Ptr<cv::xfeatures2d::SURF>  detector = cv::xfeatures2d::SURF::create(400);
	detector->detectAndCompute(map, cv::noArray(), Kp_Map, Dp_Map);
	
	cv::FileStorage fs("../GenshinImpact_TianLi_Resource/resource/GIMAP_COMPUTE.xml", cv::FileStorage::WRITE);
	fs << "keypoints" << Kp_Map;
	fs << "descriptors" << Dp_Map;
	fs.release();

	// 测试
	
	cv::FileStorage fs2("../GenshinImpact_TianLi_Resource/resource/GIMAP_COMPUTE.xml", cv::FileStorage::READ);

	std::vector<cv::KeyPoint>  keypoints;
	cv::Mat  descriptors;
	
	
	fs2["keypoints"] >> keypoints;
	fs2["descriptors"] >> descriptors;
	fs2.release();

	if (Kp_Map.size() == keypoints.size())
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
