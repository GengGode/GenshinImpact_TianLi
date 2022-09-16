#pragma once
#include "GenshinImpact_TianLi_Track_Utils_Struct.h"

inline int getMaxID(double lis[], int len)
{
	int maxId = 0;
	for (int i = 1; i < len; i++)
	{
		if (lis[i] > lis[maxId])
		{
			maxId = i;
		}
	}
	return maxId;
}

inline void get_uid(const GenshinScreen& genshin_screen, GenshinUID& out_genshin_uid)
{
	static const int uid_width = 9;
	static cv::Mat uid_header_template; 
	static std::vector<cv::Mat> uid_number_template;
	static bool is_first = true;
	if (is_first)
	{
		std::vector<cv::Mat> uid_header_template_vec;
		cv::split( GenshinImpact_TianLi_Resource::GetInstance()->GINUMUID[10], uid_header_template_vec);
		uid_header_template = uid_header_template_vec[3];
		uid_number_template.resize(10);
		for (int i = 0; i < 10; i++)
		{
			std::vector<cv::Mat> uid_number_template_vec;
			cv::split(GenshinImpact_TianLi_Resource::GetInstance()->GINUMUID[i], uid_number_template_vec);
			uid_number_template[i] = uid_number_template_vec[3];
		}
		
		is_first = false;
	}
	if (genshin_screen.img_uid.empty())return;

	std::vector<cv::Mat> channels;
	split(genshin_screen.img_uid, channels);

	cv::Mat giUIDRef;
	giUIDRef = channels[3];

	int _uid = 0;
	int _NumBit[9] = { 0 };

	int bitCount = 1;
	cv::Mat matchTmp;
	cv::Mat Roi;
	
	
	giUIDRef.copyTo(Roi);

	if (uid_header_template.rows > Roi.rows)
	{
		cv::resize(Roi, Roi, cv::Size(cvRound(1.0 * uid_header_template.rows / Roi.rows * Roi.cols), uid_header_template.rows), 0);
	}

	cv::matchTemplate(Roi, uid_header_template, matchTmp, cv::TM_CCOEFF_NORMED);

	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	//Ñ°ÕÒ×î¼ÑÆ¥ÅäÎ»ÖÃ
	cv::minMaxLoc(matchTmp, &minVal, &maxVal, &minLoc, &maxLoc);
	if (maxVal > 0.75)
	{
		int x_uid_ = maxLoc.x + uid_header_template.cols + 7;
		int y_uid_ = 0;
		double tmplis[10] = { 0 };
		int tmplisx[10] = { 0 };
		for (int p = 8; p >= 0; p--)
		{
			_NumBit[p] = 0;
			for (int i = 0; i < 10; i++)
			{
				cv::Rect r(x_uid_, y_uid_, uid_number_template[i].cols + 2, giUIDRef.rows);//180-46/9->140/9->16->16*9=90+54=144
				if (x_uid_ + r.width > giUIDRef.cols)
				{
					r = cv::Rect(giUIDRef.cols - uid_number_template[i].cols - 2, y_uid_, uid_number_template[i].cols + 2, giUIDRef.rows);
				}

				cv::Mat numCheckUID = uid_number_template[i];
				Roi = giUIDRef(r);

				cv::matchTemplate(Roi, numCheckUID, matchTmp, cv::TM_CCOEFF_NORMED);

				double minVali, maxVali;
				cv::Point minLoci, maxLoci;
				//Ñ°ÕÒ×î¼ÑÆ¥ÅäÎ»ÖÃ
				cv::minMaxLoc(matchTmp, &minVali, &maxVali, &minLoci, &maxLoci);

				tmplis[i] = maxVali;
				tmplisx[i] = maxLoci.x + numCheckUID.cols - 1;
				if (maxVali > 0.91)
				{
					_NumBit[p] = i;
					x_uid_ = x_uid_ + maxLoci.x + numCheckUID.cols - 1;
					break;
				}
				if (i == 10 - 1)
				{
					_NumBit[p] = getMaxID(tmplis, 10);
					x_uid_ = x_uid_ + tmplisx[_NumBit[p]];
				}
			}
		}
	}
	_uid = 0;
	for (int i = 0; i < 9; i++)
	{
		_uid += _NumBit[i] * bitCount;
		bitCount = bitCount * 10;
	}
	if (_uid == 0)
	{
		return;
	}
	out_genshin_uid.uid = _uid;
}