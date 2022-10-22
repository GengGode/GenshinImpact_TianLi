#include "pch.h"
#include "GenshinImpact_TianLi_Map_Utils.h"

/// <summary>
/// ���� RGBA ͼ���� ��Ƭ���� �ķ�ʽ
/// </summary>
/// <param name="src1">��ͼ RGBA</param>
/// <param name="src2">Ҫ����ͼ�� RGBA</param>
/// <param name="dst">���ͼ�� RGBA</param>
/// <param name="alpha">Ҫ����ͼ���͸���� 0-1</param>
void TianLi::Map::Utils::add_rgba_image(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst, double alpha)
{
	//assert(src1.size() != src2.size());
	assert(src1.channels() == 4 && src2.channels() == 4);
	
	// ����͸��ͨ������ Color = Src1.rgb * Src1.a + Src2.rgb * Src2.a * alpha
	
	std::vector<cv::Mat> src1_split;
	std::vector<cv::Mat> src2_split;
	std::vector<cv::Mat> dst_merge;

	cv::split(src1, src1_split);
	cv::split(src2, src2_split);
	
	for (int i = 0; i < 3; i++)
	{
		cv::Mat dst_src1 = src1_split[i].mul(src1_split[3], 1.0 / 255.0);
		cv::Mat dst_src2 = src2_split[i].mul(src2_split[3], alpha / 255.0);
		cv::Mat dst_channel = dst_src1 + dst_src2;
		dst_merge.push_back(dst_channel);
	}

	cv::Mat alpha_dst = src1_split[3] + src2_split[3] * alpha;
	// cv::scaleAdd(src2_split[3], alpha, src1_split[3], alpha_dst);
	dst_merge.push_back(alpha_dst);
	
	cv::merge(dst_merge, dst);
}
