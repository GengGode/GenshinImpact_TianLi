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
	
	// �Ź�����չͼƬ�㷨
	QImage border_image(QImage image, int w, int h, int clip_top, int clip_right, int clip_bottom, int clip_left)
	{
		QImage new_image(w, h, QImage::Format_ARGB32);
		new_image.fill(Qt::transparent);
		QRect target;
		QRect source;
		QPainter painter(&new_image);

		// left_top
		// target ��ΧΪ ��㣺0,0 ��С��clip_left, clip_top
		target = QRect(0, 0, clip_left, clip_top);
		// source ��ΧΪ ��㣺0,0 ��С��clip_left, clip_top
		source = QRect(0, 0, clip_left, clip_top);
		painter.drawImage(target, image, source);

		// left_bottom
		// target ��ΧΪ ��㣺0,h-clip_bottom ��С��clip_left, clip_bottom
		target = QRect(0, h - clip_bottom, clip_left, clip_bottom);
		// source ��ΧΪ ��㣺0,image.height()-clip_bottom ��С��clip_left, clip_bottom
		source = QRect(0, image.height() - clip_bottom, clip_left, clip_bottom);
		painter.drawImage(target, image, source);

		// right_top
		// target ��ΧΪ ��㣺w-clip_right,0 ��С��clip_right, clip_top
		target = QRect(w - clip_right, 0, clip_right, clip_top);
		// source ��ΧΪ ��㣺image.width()-clip_right,0 ��С��clip_right, clip_top
		source = QRect(image.width() - clip_right, 0, clip_right, clip_top);
		painter.drawImage(target, image, source);

		// right_bottom
		// target ��ΧΪ ��㣺w-clip_right,h-clip_bottom ��С��clip_right, clip_bottom
		target = QRect(w - clip_right, h - clip_bottom, clip_right, clip_bottom);
		// source ��ΧΪ ��㣺image.width()-clip_right,image.height()-clip_bottom ��С��clip_right, clip_bottom
		source = QRect(image.width() - clip_right, image.height() - clip_bottom, clip_right, clip_bottom);
		painter.drawImage(target, image, source);

		// top
		// target ��ΧΪ ��㣺clip_left,0 ��С��w-clip_left-clip_right, clip_top
		target = QRect(clip_left, 0, w - clip_left - clip_right, clip_top);
		// source ��ΧΪ ��㣺clip_left,0 ��С��image.width()-clip_left-clip_right, clip_top
		source = QRect(clip_left, 0, image.width() - clip_left - clip_right, clip_top);
		painter.drawImage(target, image, source);

		// bottom
		// target ��ΧΪ ��㣺clip_left,h-clip_bottom ��С��w-clip_left-clip_right, clip_bottom
		target = QRect(clip_left, h - clip_bottom, w - clip_left - clip_right, clip_bottom);
		// source ��ΧΪ ��㣺clip_left,image.height()-clip_bottom ��С��image.width()-clip_left-clip_right, clip_bottom
		source = QRect(clip_left, image.height() - clip_bottom, image.width() - clip_left - clip_right, clip_bottom);
		painter.drawImage(target, image, source);

		// left
		// target ��ΧΪ ��㣺0,clip_top ��С��clip_left, h-clip_top-clip_bottom
		target = QRect(0, clip_top, clip_left, h - clip_top - clip_bottom);
		// source ��ΧΪ ��㣺0,clip_top ��С��clip_left, image.height()-clip_top-clip_bottom
		source = QRect(0, clip_top, clip_left, image.height() - clip_top - clip_bottom);
		painter.drawImage(target, image, source);

		// right
		// target ��ΧΪ ��㣺w-clip_right,clip_top ��С��clip_right, h-clip_top-clip_bottom
		target = QRect(w - clip_right, clip_top, clip_right, h - clip_top - clip_bottom);
		// source ��ΧΪ ��㣺image.width()-clip_right,clip_top ��С��clip_right, image.height()-clip_top-clip_bottom
		source = QRect(image.width() - clip_right, clip_top, clip_right, image.height() - clip_top - clip_bottom);
		painter.drawImage(target, image, source);

		// center
		// target ��ΧΪ ��㣺clip_left,clip_top ��С��w-clip_left-clip_right, h-clip_top-clip_bottom
		target = QRect(clip_left, clip_top, w - clip_left - clip_right, h - clip_top - clip_bottom);
		// source ��ΧΪ ��㣺clip_left,clip_top ��С��image.width()-clip_left-clip_right, image.height()-clip_top-clip_bottom
		source = QRect(clip_left, clip_top, image.width() - clip_left - clip_right, image.height() - clip_top - clip_bottom);
		painter.drawImage(target, image, source);

		painter.end();
		return new_image;
	}

	cv::Mat get_view_map(cv::Mat& GIMAP, cv::Size view_map_size, cv::Point2d view_map_center, double view_map_scale)
	{
		static cv::Mat viewMap;
		static cv::Rect viewMapRect;
		//static cv::Point2d viewMapCenter;

		static int mapSizeWidth = GIMAP.size().width;
		static int mapSizeHeight = GIMAP.size().height;
		const static cv::Point2d originGIMAP(1428, 2937);
		//��Ҫ��Խ���߽磬�����Χ��ʾʱ�޷���֤��ɫ��ͷ������ȷλ��

		cv::Point minMapPoint = cv::Point(0, 0);

		cv::Size reMapSize = view_map_size;
		cv::Point2d reMapCenter = cv::Point(view_map_size.width / 2, view_map_size.height / 2) * view_map_scale;
		//cv::Point2d reAutoMapCenter = view_map_center;
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


		cv::Point2d LT = view_map_center - reMapCenter;
		cv::Point2d RB = view_map_center + cv::Point2d(reMapSize) - reMapCenter;

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

		resize(GIMAP(viewMapRect), viewMap, view_map_size);
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
		// �����Ŀ�Ϊ���ν��� ��0��255
		// ����
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
		// ����
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
		// ����
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
		// ����
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
		// �߽��Ŀ�Ϊ���Խ��� ��0��255
		// ��
		for (int i = 0; i < mask_small_size_width; i++)
		{
			for (int j = 0; j < mask_center_size_width; j++)
			{
				mask_01.at<uchar>(i, j) = 255 * (double)i / mask_small_size_width;
			}
		}
		// ��
		for (int i = 0; i < mask_small_size_width; i++)
		{
			for (int j = 0; j < mask_center_size_width; j++)
			{
				mask_21.at<uchar>(i, j) = 255 * (1 - (double)i / mask_small_size_width);
			}
		}
		// ��
		for (int i = 0; i < mask_center_size_height; i++)
		{
			for (int j = 0; j < mask_small_size_width; j++)
			{
				mask_10.at<uchar>(i, j) = 255 * (double)j / mask_small_size_width;
			}
		}
		// ��
		for (int i = 0; i < mask_center_size_height; i++)
		{
			for (int j = 0; j < mask_small_size_width; j++)
			{
				mask_12.at<uchar>(i, j) = 255 * (1 - (double)j / mask_small_size_width);
			}
		}

		// �м䲿��Ϊ255
		mask_11 = cv::Scalar(255);

		// ����mask
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
}

