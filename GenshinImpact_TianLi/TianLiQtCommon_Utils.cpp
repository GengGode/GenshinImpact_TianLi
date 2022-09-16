#include "TianLiQtCommon_Utils.h"
#include <QMutex>
#include <QImage>
#include <QPainter>

#include <Windows.h>
/*
TianLiQtCommon_Utils::TianLiQtCommon_Utils(QObject *parent)
	: QObject(parent)
{
}

TianLiQtCommon_Utils& TianLiQtCommon_Utils::GetInstance()
{
	// TODO: �ڴ˴����� return ���
	static QMutex mutex;
	static QScopedPointer<TianLiQtCommon_Utils> instance;
	if (instance.isNull())
	{
		mutex.lock();
		if (instance.isNull())
		{
			instance.reset(new TianLiQtCommon_Utils(NULL));
		}
		mutex.unlock();
	}
	return *instance.data();
}

TianLiQtCommon_Utils::~TianLiQtCommon_Utils()
{
}
*/

namespace TianLi::Utils
{
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
}

