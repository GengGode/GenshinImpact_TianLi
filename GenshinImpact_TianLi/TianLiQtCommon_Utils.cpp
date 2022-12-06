#include "TianLiQtCommon_Utils.h"
#include <QMutex>
#include <QImage>
#include <QPainter>

#include "WindowsAttribute.h"

namespace TianLi::Utils
{
	void set_window_blur_bebind(HWND handle)
	{
		SetWindowBlurBehind(handle);
	}
	
	// 九宫格扩展图片算法
	QImage border_image(QImage image, int w, int h, int clip_top, int clip_right, int clip_bottom, int clip_left)
	{
		QImage new_image(w, h, QImage::Format_ARGB32);
		new_image.fill(Qt::transparent);
		QRect target;
		QRect source;
		QPainter painter(&new_image);

		// left_top
		// target 范围为 起点：0,0 大小：clip_left, clip_top
		target = QRect(0, 0, clip_left, clip_top);
		// source 范围为 起点：0,0 大小：clip_left, clip_top
		source = QRect(0, 0, clip_left, clip_top);
		painter.drawImage(target, image, source);

		// left_bottom
		// target 范围为 起点：0,h-clip_bottom 大小：clip_left, clip_bottom
		target = QRect(0, h - clip_bottom, clip_left, clip_bottom);
		// source 范围为 起点：0,image.height()-clip_bottom 大小：clip_left, clip_bottom
		source = QRect(0, image.height() - clip_bottom, clip_left, clip_bottom);
		painter.drawImage(target, image, source);

		// right_top
		// target 范围为 起点：w-clip_right,0 大小：clip_right, clip_top
		target = QRect(w - clip_right, 0, clip_right, clip_top);
		// source 范围为 起点：image.width()-clip_right,0 大小：clip_right, clip_top
		source = QRect(image.width() - clip_right, 0, clip_right, clip_top);
		painter.drawImage(target, image, source);

		// right_bottom
		// target 范围为 起点：w-clip_right,h-clip_bottom 大小：clip_right, clip_bottom
		target = QRect(w - clip_right, h - clip_bottom, clip_right, clip_bottom);
		// source 范围为 起点：image.width()-clip_right,image.height()-clip_bottom 大小：clip_right, clip_bottom
		source = QRect(image.width() - clip_right, image.height() - clip_bottom, clip_right, clip_bottom);
		painter.drawImage(target, image, source);

		// top
		// target 范围为 起点：clip_left,0 大小：w-clip_left-clip_right, clip_top
		target = QRect(clip_left, 0, w - clip_left - clip_right, clip_top);
		// source 范围为 起点：clip_left,0 大小：image.width()-clip_left-clip_right, clip_top
		source = QRect(clip_left, 0, image.width() - clip_left - clip_right, clip_top);
		painter.drawImage(target, image, source);

		// bottom
		// target 范围为 起点：clip_left,h-clip_bottom 大小：w-clip_left-clip_right, clip_bottom
		target = QRect(clip_left, h - clip_bottom, w - clip_left - clip_right, clip_bottom);
		// source 范围为 起点：clip_left,image.height()-clip_bottom 大小：image.width()-clip_left-clip_right, clip_bottom
		source = QRect(clip_left, image.height() - clip_bottom, image.width() - clip_left - clip_right, clip_bottom);
		painter.drawImage(target, image, source);

		// left
		// target 范围为 起点：0,clip_top 大小：clip_left, h-clip_top-clip_bottom
		target = QRect(0, clip_top, clip_left, h - clip_top - clip_bottom);
		// source 范围为 起点：0,clip_top 大小：clip_left, image.height()-clip_top-clip_bottom
		source = QRect(0, clip_top, clip_left, image.height() - clip_top - clip_bottom);
		painter.drawImage(target, image, source);

		// right
		// target 范围为 起点：w-clip_right,clip_top 大小：clip_right, h-clip_top-clip_bottom
		target = QRect(w - clip_right, clip_top, clip_right, h - clip_top - clip_bottom);
		// source 范围为 起点：image.width()-clip_right,clip_top 大小：clip_right, image.height()-clip_top-clip_bottom
		source = QRect(image.width() - clip_right, clip_top, clip_right, image.height() - clip_top - clip_bottom);
		painter.drawImage(target, image, source);

		// center
		// target 范围为 起点：clip_left,clip_top 大小：w-clip_left-clip_right, h-clip_top-clip_bottom
		target = QRect(clip_left, clip_top, w - clip_left - clip_right, h - clip_top - clip_bottom);
		// source 范围为 起点：clip_left,clip_top 大小：image.width()-clip_left-clip_right, image.height()-clip_top-clip_bottom
		source = QRect(clip_left, clip_top, image.width() - clip_left - clip_right, image.height() - clip_top - clip_bottom);
		painter.drawImage(target, image, source);

		painter.end();
		return new_image;
	}

	cv::Mat get_view_map(const cv::Mat& GIMAP, cv::Size view_size, cv::Point2d view_center, double view_map_scale, cv::Rect& map_rect)
	{
		static cv::Mat viewMap;
		static cv::Rect viewMapRect;
		//static cv::Point2d viewMapCenter;

		static int mapSizeWidth = GIMAP.size().width;
		static int mapSizeHeight = GIMAP.size().height;
		const static cv::Point2d originGIMAP(1428, 2937);
		//需要能越过边界，否则大范围显示时无法保证角色箭头处于正确位置

		cv::Point minMapPoint = cv::Point(0, 0);

		cv::Size reMapSize = view_size;
		cv::Point2d reMapCenter = cv::Point(view_size.width / 2, view_size.height / 2) * view_map_scale;
		//cv::Point2d reAutoMapCenter = view_center;
		reMapSize.width = (reMapSize.width * view_map_scale);
		reMapSize.height = (reMapSize.height * view_map_scale);
		if (reMapSize.width > mapSizeWidth)
		{
			reMapSize.width = mapSizeWidth;
		}
		if (reMapSize.height > mapSizeHeight)
		{
			reMapSize.height = mapSizeHeight;
		}


		cv::Point2d LT = view_center - reMapCenter;
		cv::Point2d RB = view_center + cv::Point2d(reMapSize) - reMapCenter;

		minMapPoint = LT;

		if (LT.x < 0)
		{
			minMapPoint.x = 0;
		}
		if (LT.y < 0)
		{
			minMapPoint.y = 0;
		}
		if (RB.x > mapSizeWidth)
		{
			minMapPoint.x = mapSizeWidth - reMapSize.width;
		}
		if (RB.y > mapSizeHeight)
		{
			minMapPoint.y = mapSizeHeight - reMapSize.height;
		}
		viewMapRect = cv::Rect(minMapPoint, reMapSize);
		map_rect = viewMapRect;
		resize(GIMAP(viewMapRect), viewMap, view_size);
		return viewMap;
	}
	cv::Mat create_square_mask(int mask_width, int mask_height, double gradient_width)
	{
		int mask_center_size_width = mask_width - gradient_width * 2;
		int mask_center_size_height = mask_height - gradient_width * 2;
		int mask_small_size_width = gradient_width;

		cv::Mat mask = cv::Mat::zeros(mask_height, mask_width, CV_8UC1);

		/*
			m00:m,m		m01:n,m		m02:m,m

			m10:m,n		m11:n,n		m12:m,n

			m20:m,m		m21:n,m		m22:m,m

			m = mask_small_size_width
			n = mask_center_size_width
		*/
		cv::Mat mask_00 = mask(cv::Rect(0, 0, mask_small_size_width, mask_small_size_width));
		cv::Mat mask_01 = mask(cv::Rect(mask_small_size_width, 0, mask_center_size_width, mask_small_size_width));
		cv::Mat mask_02 = mask(cv::Rect(mask_small_size_width + mask_center_size_width, 0, mask_small_size_width, mask_small_size_width));
		cv::Mat mask_10 = mask(cv::Rect(0, mask_small_size_width, mask_small_size_width, mask_center_size_height));
		cv::Mat mask_11 = mask(cv::Rect(mask_small_size_width, mask_small_size_width, mask_center_size_width, mask_center_size_height));
		cv::Mat mask_12 = mask(cv::Rect(mask_small_size_width + mask_center_size_width, mask_small_size_width, mask_small_size_width, mask_center_size_height));
		cv::Mat mask_20 = mask(cv::Rect(0, mask_small_size_width + mask_center_size_height, mask_small_size_width, mask_small_size_width));
		cv::Mat mask_21 = mask(cv::Rect(mask_small_size_width, mask_small_size_width + mask_center_size_height, mask_center_size_width, mask_small_size_width));
		cv::Mat mask_22 = mask(cv::Rect(mask_small_size_width + mask_center_size_width, mask_small_size_width + mask_center_size_height, mask_small_size_width, mask_small_size_width));
		// 顶角四块为扇形渐变 从0到255
		// 左上
		for (int i = 0; i < mask_small_size_width; i++)
		{
			for (int j = 0; j < mask_small_size_width; j++)
			{
				int center_x = mask_small_size_width;
				int center_y = mask_small_size_width;
				int center_r = mask_small_size_width;

				double distance = sqrt(pow(i - center_x, 2) + pow(j - center_y, 2));
				if (distance <= center_r)
				{
					mask_00.at<uchar>(i, j) = 255 * (1 - distance / center_r);
				}
			}
		}
		// 右上
		for (int i = 0; i < mask_small_size_width; i++)
		{
			for (int j = 0; j < mask_small_size_width; j++)
			{
				int center_x = mask_small_size_width;
				int center_y = 0;
				int center_r = mask_small_size_width;

				double distance = sqrt(pow(i - center_x, 2) + pow(j - center_y, 2));
				if (distance <= center_r)
				{
					mask_02.at<uchar>(i, j) = 255 * (1 - distance / center_r);
				}
			}
		}
		// 左下
		for (int i = 0; i < mask_small_size_width; i++)
		{
			for (int j = 0; j < mask_small_size_width; j++)
			{
				int center_x = 0;
				int center_y = mask_small_size_width;
				int center_r = mask_small_size_width;

				double distance = sqrt(pow(i - center_x, 2) + pow(j - center_y, 2));
				if (distance <= center_r)
				{
					mask_20.at<uchar>(i, j) = 255 * (1 - distance / center_r);
				}
			}
		}
		// 右下
		for (int i = 0; i < mask_small_size_width; i++)
		{
			for (int j = 0; j < mask_small_size_width; j++)
			{
				int center_x = 0;
				int center_y = 0;
				int center_r = mask_small_size_width;

				double distance = sqrt(pow(i - center_x, 2) + pow(j - center_y, 2));
				if (distance <= center_r)
				{
					mask_22.at<uchar>(i, j) = 255 * (1 - distance / center_r);
				}
			}
		}
		// 边角四块为线性渐变 从0到255
		// 上
		for (int i = 0; i < mask_small_size_width; i++)
		{
			for (int j = 0; j < mask_center_size_width; j++)
			{
				mask_01.at<uchar>(i, j) = 255 * (double)i / mask_small_size_width;
			}
		}
		// 下
		for (int i = 0; i < mask_small_size_width; i++)
		{
			for (int j = 0; j < mask_center_size_width; j++)
			{
				mask_21.at<uchar>(i, j) = 255 * (1 - (double)i / mask_small_size_width);
			}
		}
		// 左
		for (int i = 0; i < mask_center_size_height; i++)
		{
			for (int j = 0; j < mask_small_size_width; j++)
			{
				mask_10.at<uchar>(i, j) = 255 * (double)j / mask_small_size_width;
			}
		}
		// 右
		for (int i = 0; i < mask_center_size_height; i++)
		{
			for (int j = 0; j < mask_small_size_width; j++)
			{
				mask_12.at<uchar>(i, j) = 255 * (1 - (double)j / mask_small_size_width);
			}
		}

		// 中间部分为255
		mask_11 = cv::Scalar(255);

		// 返回mask
		return mask;
	}
	cv::Mat create_circular_mask(int mask_width, int mask_height, double gradient_width)
	{
		cv::Mat mask = cv::Mat::zeros(mask_height, mask_width, CV_8UC1);
		cv::Point2d center(mask_width / 2, mask_height / 2);
		double radius = min(mask_width, mask_height) / 2;
		double gradient_radius = radius - gradient_width;
		for (int i = 0; i < mask_height; i++)
		{
			for (int j = 0; j < mask_width; j++)
			{
				double distance = cv::norm(center - cv::Point2d(j, i));
				if (distance < gradient_radius)
				{
					mask.at<uchar>(i, j) = 255;
				}
				else if (distance < radius)
				{
					mask.at<uchar>(i, j) = 255 * (radius - distance) / gradient_width;
				}
			}
		}
		return mask;
	}
	QImage mat_2_qimage(cv::Mat const& src)
	{
		QImage dest((const uchar*)(src.data), src.cols, src.rows, src.cols * (src.channels()), QImage::Format_ARGB32);
		dest.bits(); // enforce deep copy, see documentation 
		// of QImage::QImage ( const uchar * data, int width, int height, Format format )
		return dest;
	}
	cv::Mat qimage_2_mat(QImage const& src)
	{
		cv::Mat tmp(src.height(), src.width(), CV_8UC4, (uchar*)src.bits(), src.bytesPerLine());
		cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
		result = tmp.clone();
		return result;
	}

	void add_rgba_image(cv::Mat& src1, cv::Mat& src2, cv::Mat& dst, double alpha)
	{
		//assert(src1.size() != src2.size());
		assert(src1.channels() == 4 && src2.channels() == 4);

		std::vector<cv::Mat> src1_split;
		std::vector<cv::Mat> src2_split;
		std::vector<cv::Mat> dst_merge;

		cv::split(src1, src1_split);
		cv::split(src2, src2_split);

		// TODO:fix 修改透明度只是改变了叠加层的透明度，并未显示底层的画面
		for (int i = 0; i < 3; i++)
		{
			auto dst_src1 = src1_split[i].mul(~src2_split[3], 1.0 / 255.0);
			auto dst_src2 = src2_split[i].mul(src2_split[3], alpha / 255.0);
			auto dst_channel = dst_src1 + dst_src2;
			dst_merge.push_back(dst_channel);
		}
		cv::Mat alpha_dst = src1_split[3] + src2_split[3] * alpha;
		dst_merge.push_back(alpha_dst);
		cv::merge(dst_merge, dst);
	}
	
	cv::Mat draw_object_border(cv::Mat src, bool is_show)
	{
		cv::Scalar show_color = cv::Scalar(240, 240, 240, 255);
		if (is_show)
		{
			show_color = cv::Scalar(255, 255, 25, 255);
		}

		int resize = 49;
		int rect_w = 57;
		int rect_h = 64;
		int center_x = 28;
		int center_y = 28;
		
		cv::Mat dst = src;

		cv::circle(dst, cv::Point(center_x, center_y), center_x - 2, cv::Scalar(0, 0, 0, 48), 2, cv::LINE_AA);
		cv::circle(dst, cv::Point(center_x, center_y), center_x - 6, show_color, 4, cv::LINE_AA);
		cv::ellipse(dst, cv::Point(center_x, center_y), cv::Size(center_x - 6, center_x - 6), 90, -40, 40, cv::Scalar(255, 255, 255, 255), 4, cv::LINE_AA);
		/*{
			int box_r = 13;
			cv::Mat box_img = cv::Mat(box_r + box_r, box_r + box_r, CV_8UC4, cv::Scalar(0, 0, 0, 0));
			int center_box_y = center_y + center_y - 6;
			cv::Point pts[4];
			pts[0] = cv::Point(center_x, center_box_y + box_r);
			pts[1] = cv::Point(center_x + box_r, center_box_y);
			pts[2] = cv::Point(center_x, center_box_y - box_r);
			pts[3] = cv::Point(center_x - box_r, center_box_y);
			cv::fillConvexPoly(dst, pts, 4, cv::Scalar(0, 0, 0, 48));
			cv::Rect roi_rect = cv::Rect(center_x - box_r, center_box_y - box_r, box_r + box_r, box_r + box_r);
			cv::Mat box_roi = dst(roi_rect);

		}*/
		{
			int box_r = 11;
			cv::Mat box_img=cv::Mat(box_r + box_r+1, box_r + box_r+1,CV_8UC4,cv::Scalar(0,0,0,0));
			int center_box_y = center_y + center_y - 4;
			cv::Point pts[4];
			pts[0] = cv::Point(box_r, box_r + box_r);
			pts[1] = cv::Point(box_r + box_r, box_r);
			pts[2] = cv::Point(box_r, box_r - box_r);
			pts[3] = cv::Point(box_r - box_r, box_r);
			cv::fillConvexPoly(box_img, pts, 4, cv::Scalar(0, 0, 0, 48));
			cv::Rect roi_rect=cv::Rect(center_x - box_r, center_box_y - box_r, box_r+ box_r+1, box_r+ box_r+1);
			cv::Mat box_roi = dst(roi_rect);

			add_rgba_image(box_roi, box_img, box_roi,1.0);
		}
		{
			int center_box_y = center_y + center_y - 4;
			int box_r = 7;
			cv::Point pts[4];
			pts[0] = cv::Point(center_x, center_box_y + box_r);
			pts[1] = cv::Point(center_x + box_r, center_box_y);
			pts[2] = cv::Point(center_x, center_box_y - box_r);
			pts[3] = cv::Point(center_x - box_r, center_box_y);
			cv::fillConvexPoly(dst, pts, 4, cv::Scalar(255, 255, 255, 255));
		}
		{
			int box_r = 7;
			int center_box_y = center_y + center_y - box_r;
			cv::ellipse(dst, cv::Point(center_x, center_box_y-4), cv::Size(4, 4), 90, -45, 45, show_color, -1, cv::LINE_AA,0);
		}
		return dst;
	}

}

