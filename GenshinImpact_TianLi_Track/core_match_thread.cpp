#include "pch.h"
#include "core_match_thread.h"


#include <thread>
#include <Windows.h>

#include <opencv2/imgproc/imgproc_c.h>

ATM_TM_SurfMap::ATM_TM_SurfMap()
{
	hisP[0] = Point();
	hisP[1] = Point();
	hisP[2] = Point();
}

ATM_TM_SurfMap::~ATM_TM_SurfMap()
{
}

void ATM_TM_SurfMap::setMap(Mat mapMat)
{
	_mapMat = mapMat;
}

void ATM_TM_SurfMap::setMinMap(Mat minMapMat)
{
	_minMapMat = minMapMat;
}

void ATM_TM_SurfMap::Init()
{
	if (isInit)return;
	detector = xfeatures2d::SURF::create(minHessian);
	detector->detectAndCompute(_mapMat, noArray(), Kp_Map, Dp_Map);
	isInit = true;
}

void ATM_TM_SurfMap::SURFMatch()
{
	//static Point hisP[3];

	Mat img_scene(_mapMat);
	Mat img_object(_minMapMat(Rect(30, 30, _minMapMat.cols - 60, _minMapMat.rows - 60)));

	//someSizeR = cvCeil(img_object.rows / 2);

	if (img_object.empty())
	{
		return;
	}

	isContinuity = false;
	Point2d dp1 = hisP[1] - hisP[0];
	Point2d dp2 = hisP[2] - hisP[1];

	//角色移动连续性判断
	if ((dis(dp1) + dis(dp2)) < 2000)
	{
		if (hisP[2].x > someSizeR && hisP[2].x < img_scene.cols - someSizeR && hisP[2].y>someSizeR && hisP[2].y < img_scene.rows - someSizeR)
		{
			isContinuity = true;
			if (isContinuity)
			{

				//				
				//				//惯导部分
				//				if (_minMapLastMat.empty() == false)
				//				{
				//					Mat someMap(_minMapLastMat);//上一帧小地图画面
				//					Mat minMap(img_object);//当前帧小地图画面
				//
				//					detectorSomeMap = xfeatures2d::SURF::create(minHessian);
				//					detectorSomeMap->detectAndCompute(someMap, noArray(), Kp_SomeMap, Dp_SomeMap);
				//					detectorSomeMap->detectAndCompute(minMap, noArray(), Kp_MinMap, Dp_MinMap);
				//
				//					//没检测到特征点 连续性断开
				//					if (Kp_SomeMap.size() == 0 || Kp_MinMap.size() == 0)
				//					{
				//						isContinuity = false;
				//					}
				//					else
				//					{
				//						Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
				//						std::vector< std::vector<DMatch> > KNN_mTmp;
				//#ifdef _DEBUG
				//						std::vector<DMatch> good_matchesTmp;
				//#endif
				//						matcherTmp->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mTmp, 2);
				//						std::vector<double> lisx;
				//						std::vector<double> lisy;
				//						double sumx = 0;
				//						double sumy = 0;
				//						for (size_t i = 0; i < KNN_mTmp.size(); i++)
				//						{
				//							if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
				//							{
				//#ifdef _DEBUG
				//								good_matchesTmp.push_back(KNN_mTmp[i][0]);
				//#endif
				//								// 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
				//								if (KNN_mTmp[i][0].queryIdx >= Kp_MinMap.size())
				//								{
				//									continue;
				//								}
				//
				//								//mapScale 考虑城镇和大世界两种
				//								lisx.push_back(((minMap.cols / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.x)*1.0 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
				//								lisy.push_back(((minMap.rows / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.y)*1.0 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
				//								sumx += lisx.back();
				//								sumy += lisy.back();
				//							}
				//						}
				//
				//#ifdef _DEBUG
				//						Mat img_matches, imgmap, imgminmap;
				//						drawKeypoints(someMap, Kp_SomeMap, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
				//						drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
				//
				//						drawMatches(img_object, Kp_MinMap, someMap, Kp_SomeMap, good_matchesTmp, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
				//#endif
				//
				//						if (max(lisx.size(), lisy.size()) > 4)
				//						{
				//							if (min(lisx.size(), lisy.size()) >= 10)
				//							{
				//								isOnCity = true;
				//							}
				//							else
				//							{
				//								isOnCity = false;
				//							}
				//							//if(max(lisx.size(), lisy.size()) <=10 )
				//							//{
				//							//	isOnCity = false;
				//							//}
				//
				//							Point2d p = Point2d(sumx / (double)lisx.size(), sumy / lisy.size()); //均值
				//							//double meany = sumy / lisy.size(); //均值
				//							//Point2d p = SPC(lisx, sumx, lisy, sumy);
				//							
				//							double x = (p.x - someMap.cols / 2.0);
				//							double y = (p.y - someMap.rows / 2.0);
				//
				//							pos = Point2d(x + hisP[2].x, y + hisP[2].y);
				//							minMap.copyTo(_minMapLastMat);
				//						}
				//					}
				//
				//
				//				}
				//				else
				//				{
				//					Mat minMap(img_object);//当前帧小地图画面
				//					minMap.copyTo(_minMapLastMat);
				//				}


				if (isOnCity == false)
				{
					//不在城镇中时
					Mat someMap(img_scene(Rect(hisP[2].x - someSizeR, hisP[2].y - someSizeR, someSizeR * 2, someSizeR * 2)));
					Mat minMap(img_object);
					//resize(someMap, someMap, Size(), MatchMatScale, MatchMatScale, 1);
					//resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);

					detectorSomeMap = xfeatures2d::SURF::create(minHessian);
					detectorSomeMap->detectAndCompute(someMap, noArray(), Kp_SomeMap, Dp_SomeMap);
					detectorSomeMap->detectAndCompute(minMap, noArray(), Kp_MinMap, Dp_MinMap);
					if (Kp_SomeMap.size() == 0 || Kp_MinMap.size() == 0)
					{
						isContinuity = false;
					}
					else
					{
						Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
						std::vector< std::vector<DMatch> > KNN_mTmp;
#ifdef _DEBUG
						std::vector<DMatch> good_matchesTmp;
#endif
						matcherTmp->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mTmp, 2);
						std::vector<double> lisx;
						std::vector<double> lisy;
						double sumx = 0;
						double sumy = 0;
						for (size_t i = 0; i < KNN_mTmp.size(); i++)
						{
							if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
							{
#ifdef _DEBUG
								good_matchesTmp.push_back(KNN_mTmp[i][0]);
#endif
								// 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
								if (KNN_mTmp[i][0].queryIdx >= Kp_MinMap.size())
								{
									continue;
								}

								lisx.push_back(((minMap.cols / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.x) * mapScale + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
								lisy.push_back(((minMap.rows / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.y) * mapScale + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
								sumx += lisx.back();
								sumy += lisy.back();
							}
						}
#ifdef _DEBUG
						Mat img_matches, imgmap, imgminmap;
						drawKeypoints(someMap, Kp_SomeMap, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
						drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

						drawMatches(img_object, Kp_MinMap, someMap, Kp_SomeMap, good_matchesTmp, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
#ifdef _DEBUG
						if (var(lisx, sumx, lisy, sumy) > 10)
						{

						}
#endif
						if (min(lisx.size(), lisy.size()) <= 4)
						{

							//有可能处于城镇中

							/***********************/
							//重新从完整中地图取出角色周围部分地图
							img_scene(Rect(hisP[2].x - someSizeR, hisP[2].y - someSizeR, someSizeR * 2, someSizeR * 2)).copyTo(someMap);
							//Mat minMap(img_object);

							resize(someMap, someMap, Size(someSizeR * 4, someSizeR * 4));
							//resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);

							detectorSomeMap = xfeatures2d::SURF::create(minHessian);
							detectorSomeMap->detectAndCompute(someMap, noArray(), Kp_SomeMap, Dp_SomeMap);
							//detectorSomeMap->detectAndCompute(minMap, noArray(), Kp_MinMap, Dp_MinMap);
							if (Kp_SomeMap.size() == 0 || Kp_MinMap.size() == 0)
							{
								isContinuity = false;
							}
							else
							{
								Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
								std::vector< std::vector<DMatch> > KNN_mTmp;
#ifdef _DEBUG
								std::vector<DMatch> good_matchesTmp;
#endif
								matcherTmp->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mTmp, 2);
								//std::vector<double> lisx;
								//std::vector<double> lisy;
								lisx.clear();
								lisy.clear();
								//double sumx = 0;
								//double sumy = 0;
								sumx = 0;
								sumy = 0;

								for (size_t i = 0; i < KNN_mTmp.size(); i++)
								{
									if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
									{
#ifdef _DEBUG
										good_matchesTmp.push_back(KNN_mTmp[i][0]);
#endif
										// 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
										if (KNN_mTmp[i][0].queryIdx >= Kp_MinMap.size())
										{
											continue;
										}

										lisx.push_back(((minMap.cols / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.x) * 0.8667 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
										lisy.push_back(((minMap.rows / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.y) * 0.8667 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
										sumx += lisx.back();
										sumy += lisy.back();
									}
								}
#ifdef _DEBUG
								//Mat img_matches, imgmap, imgminmap;
								drawKeypoints(someMap, Kp_SomeMap, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
								drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

								drawMatches(img_object, Kp_MinMap, someMap, Kp_SomeMap, good_matchesTmp, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
								if (min(lisx.size(), lisy.size()) <= 4)
								{
									isContinuity = false;
								}
								else
								{
									if (min(lisx.size(), lisy.size()) >= 10)
									{
										isOnCity = true;
									}
									else
									{
										isOnCity = false;
									}

									//double meanx = sumx / lisx.size(); //均值
									//double meany = sumy / lisy.size(); //均值
									Point2d p = SPC(lisx, sumx, lisy, sumy);

									//int x = (int)meanx;
									//int y = (int)meany;

									double x = (p.x - someMap.cols / 2.0) / 2.0;
									double y = (p.y - someMap.rows / 2.0) / 2.0;

									pos = Point2d(x + hisP[2].x, y + hisP[2].y);
								}
							}
							/***********************/
							//isContinuity = false;
						}
						else
						{
							isOnCity = false;

							//double meanx = sumx / lisx.size(); //均值
							//double meany = sumy / lisy.size(); //均值
							Point2d p = SPC(lisx, sumx, lisy, sumy);


							double x = p.x;
							double y = p.y;

							pos = Point2d(x + hisP[2].x - someSizeR, y + hisP[2].y - someSizeR);
						}
					}
				}
				else
				{
					//在城镇中
					/***********************/
					//重新从完整中地图取出角色周围部分地图
					Mat someMap(img_scene(Rect(hisP[2].x - someSizeR, hisP[2].y - someSizeR, someSizeR * 2, someSizeR * 2)));
					Mat minMap(img_object);

					resize(someMap, someMap, Size(someSizeR * 4, someSizeR * 4));
					//resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);

					detectorSomeMap = xfeatures2d::SURF::create(minHessian);
					detectorSomeMap->detectAndCompute(someMap, noArray(), Kp_SomeMap, Dp_SomeMap);
					detectorSomeMap->detectAndCompute(minMap, noArray(), Kp_MinMap, Dp_MinMap);

					if (Kp_SomeMap.size() >= 2 && Kp_MinMap.size() >= 2)
					{
						Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
						std::vector< std::vector<DMatch> > KNN_mTmp;
#ifdef _DEBUG
						std::vector<DMatch> good_matchesTmp;
#endif
						matcherTmp->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mTmp, 2);
						std::vector<double> lisx;
						std::vector<double> lisy;
						double sumx = 0;
						double sumy = 0;

						for (size_t i = 0; i < KNN_mTmp.size(); i++)
						{
							if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
							{
#ifdef _DEBUG
								good_matchesTmp.push_back(KNN_mTmp[i][0]);
#endif
								// 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
								if (KNN_mTmp[i][0].queryIdx >= Kp_MinMap.size())
								{
									continue;
								}

								lisx.push_back(((minMap.cols / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.x) * 0.8667 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
								lisy.push_back(((minMap.rows / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.y) * 0.8667 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
								sumx += lisx.back();
								sumy += lisy.back();
							}
						}
#ifdef _DEBUG
						Mat img_matches, imgmap, imgminmap;
						drawKeypoints(someMap, Kp_SomeMap, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
						drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

						drawMatches(img_object, Kp_MinMap, someMap, Kp_SomeMap, good_matchesTmp, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
						if (max(lisx.size(), lisy.size()) > 4)
						{
							if (min(lisx.size(), lisy.size()) >= 10)
							{
								isOnCity = true;
							}
							else
							{
								isOnCity = false;
							}
							//if(max(lisx.size(), lisy.size()) <=10 )
							//{
							//	isOnCity = false;
							//}

							//double meanx = sumx / lisx.size(); //均值
							//double meany = sumy / lisy.size(); //均值
							Point2d p = SPC(lisx, sumx, lisy, sumy);

							double x = (p.x - someMap.cols / 2.0) / 2.0;
							double y = (p.y - someMap.rows / 2.0) / 2.0;

							pos = Point2d(x + hisP[2].x, y + hisP[2].y);
						}
						else
						{
							isContinuity = false;
						}//if (max(lisx.size(), lisy.size()) > 4)
					}
					else
					{
						isContinuity = false;
					}//if (Kp_SomeMap.size() != 0 && Kp_MinMap.size() != 0)
				}// if(isOnCity==false)
			}//if (isContinuity)
		}
	}//if ((dis(dp1) + dis(dp2)) < 2000)
	if (!isContinuity)
	{
		detector->detectAndCompute(img_object, noArray(), Kp_MinMap, Dp_MinMap);

		if (Kp_MinMap.size() == 0)
		{
#ifdef _DEBUG
			cout << "SURF Match Fail Not KeyPoint" << endl;
#endif // _DEBUG
			return;
		}
		else
		{
			Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
			std::vector< std::vector<DMatch> > KNN_m;
#ifdef _DEBUG
			std::vector<DMatch> good_matches;
#endif
			matcher->knnMatch(Dp_MinMap, Dp_Map, KNN_m, 2);

			std::vector<double> lisx;
			std::vector<double> lisy;
			double sumx = 0;
			double sumy = 0;
			for (size_t i = 0; i < KNN_m.size(); i++)
			{
				if (KNN_m[i][0].distance < ratio_thresh * KNN_m[i][1].distance)
				{
#ifdef _DEBUG
					good_matches.push_back(KNN_m[i][0]);
#endif
					if (KNN_m[i][0].queryIdx >= Kp_MinMap.size())
					{
						continue;
					}
					lisx.push_back(((img_object.cols / 2.0 - Kp_MinMap[KNN_m[i][0].queryIdx].pt.x) * mapScale + Kp_Map[KNN_m[i][0].trainIdx].pt.x));
					lisy.push_back(((img_object.rows / 2.0 - Kp_MinMap[KNN_m[i][0].queryIdx].pt.y) * mapScale + Kp_Map[KNN_m[i][0].trainIdx].pt.y));
					sumx += lisx.back();
					sumy += lisy.back();
				}
			}

#ifdef _DEBUG
			Mat img_matches, imgmap, imgminmap;
			drawKeypoints(img_scene, Kp_Map, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
			drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

			drawMatches(img_object, Kp_MinMap, img_scene, Kp_Map, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif

			if (min(lisx.size(), lisy.size()) == 0)
			{
#ifdef _DEBUG
				cout << "SURF Match Fail" << endl;
#endif
				return;
			}
			else
			{
#ifdef _DEBUG
				cout << "SURF Match Point Number: " << lisx.size() << "," << lisy.size() << endl;

#endif

				pos = SPC(lisx, sumx, lisy, sumy);
			}
		}
	}

	hisP[0] = hisP[1];
	hisP[1] = hisP[2];
	hisP[2] = pos;

}

Point2d ATM_TM_SurfMap::SURFMatch(Mat minMapMat)
{
	return Point2d();
}

Point2d ATM_TM_SurfMap::getLocalPos()
{
	return pos;
}

bool ATM_TM_SurfMap::getIsContinuity()
{
	return isContinuity;
}

double ATM_TM_SurfMap::dis(Point2d& p)
{
	return sqrt(p.x * p.x + p.y * p.y);
}

Point2d ATM_TM_SurfMap::SPC(vector<double> lisx, double sumx, vector<double> lisy, double sumy)
{
	//这个剔除异常点算法
	//回头要改
	Point2d mpos;
	double meanx = sumx / lisx.size(); //均值
	double meany = sumy / lisy.size(); //均值
	double x = meanx;
	double y = meany;
	if (min(lisx.size(), lisy.size()) > 3)
	{
		double accumx = 0.0;
		double accumy = 0.0;
		for (int i = 0; i < min(lisx.size(), lisy.size()); i++)
		{
			accumx += (lisx[i] - meanx) * (lisx[i] - meanx);
			accumy += (lisy[i] - meany) * (lisy[i] - meany);
		}

		double stdevx = sqrt(accumx / (lisx.size() - 1)); //标准差
		double stdevy = sqrt(accumy / (lisy.size() - 1)); //标准差

		sumx = 0;
		sumy = 0;
		double numx = 0;
		double numy = 0;
		for (int i = 0; i < min(lisx.size(), lisy.size()); i++)
		{
			if (abs(lisx[i] - meanx) < 1 * stdevx)
			{
				sumx += lisx[i];
				numx++;
			}

			if (abs(lisy[i] - meany) < 1 * stdevy)
			{
				sumy += lisy[i];
				numy++;
			}
		}
		double xx = sumx / numx;
		double yy = sumy / numy;
		mpos = Point2d(xx, yy);
	}
	else
	{
		mpos = Point2d(x, y);
	}
	return mpos;
}

//Point ATM_TM_SurfMap::SPC2(vector<double> lisx, double sumx, vector<double> lisy, double sumy)
//{
//	Point p;
//
//	return Point();
//}

double ATM_TM_SurfMap::var(vector<double> lisx, double sumx, vector<double> lisy, double sumy)
{
	double accumx = 0.0;
	double accumy = 0.0;
	for (int i = 0; i < min(lisx.size(), lisy.size()); i++)
	{
		accumx = (lisx[i] - sumx) * (lisx[i] - sumx);
		accumy = (lisy[i] - sumy) * (lisy[i] - sumy);
	}
	double stdevx = sqrt(accumx / (lisx.size() - 1));
	double stdevy = sqrt(accumy / (lisy.size() - 1));

	return sqrt(stdevx * stdevx + stdevy * stdevy);
}

void ATM_TM_TemplatePaimon::setPaimonTemplate(Mat paimonTemplateMat)
{
	_paimonTemplate = paimonTemplateMat;
}

void ATM_TM_TemplatePaimon::setPaimonMat(Mat paimonMat)
{
	_paimonMat = paimonMat;
}

void ATM_TM_TemplatePaimon::TemplatePaimon()
{
	Mat tmp;
	matchTemplate(_paimonTemplate, _paimonMat, tmp, TM_CCOEFF_NORMED);

	double minVal, maxVal;
	Point minLoc, maxLoc;
	//寻找最佳匹配位置
	minMaxLoc(tmp, &minVal, &maxVal, &minLoc, &maxLoc);
#ifdef _DEBUG
	cout << "Match Template MinVal & MaxVal" << minVal << " , " << maxVal << endl;
#endif
	if (minVal < 0.51 || maxVal == 1)
	{
		isPaimonVisible = false;
	}
	else
	{
		isPaimonVisible = true;
	}
}

bool ATM_TM_TemplatePaimon::getPaimonVisible()
{
	return isPaimonVisible;
}

void ATM_TM_ORBAvatar::setAvatarTemplate(Mat avatarTemplateMat)
{
	_avatarTemplate = avatarTemplateMat;
}

void ATM_TM_ORBAvatar::setAvatarMat(Mat avatarMat)
{
	_avatarMat = avatarMat;
}

void ATM_TM_ORBAvatar::Init()
{
	if (isInit)return;

	isInit = true;
}

bool GreaterSort(DMatch a, DMatch b)
{
	return (a.distance > b.distance);
}

void ATM_TM_ORBAvatar::ORBMatch()
{
	Mat giAvatarRef = _avatarMat;

	resize(giAvatarRef, giAvatarRef, Size(), 2, 2);
	std::vector<Mat> lis;
	split(giAvatarRef, lis);

	threshold(lis[0], gray0, 240, 255, THRESH_BINARY);
	threshold(lis[1], gray1, 212, 255, THRESH_BINARY);
	threshold(lis[2], gray2, 25, 255, THRESH_BINARY_INV);


	bitwise_and(gray1, gray2, and12, gray0);
	resize(and12, and12, Size(), 2, 2, 3);
	Canny(and12, and12, 20, 3 * 20, 3);
	circle(and12, Point(cvCeil(and12.cols / 2), cvCeil(and12.rows / 2)), cvCeil(and12.cols / 4.5), Scalar(0, 0, 0), -1);

	dilate(and12, and12, dilate_element);
	erode(and12, and12, erode_element);

	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarcy;

	findContours(and12, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	std::vector<Rect> boundRect(contours.size());  //定义外接矩形集合
	//std::vector<RotatedRect> box(contours.size()); //定义最小外接矩形集合

	Point2f rect[4];

	std::vector<Point2d> AvatarKeyPoint;
	double AvatarKeyPointLine[3] = { 0 };
	std::vector<Point2f> AvatarKeyLine;
	Point2f KeyLine;

	if (contours.size() != 3)
	{
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		//box[i] = minAreaRect(Mat(contours[i]));  //计算每个轮廓最小外接矩形
		boundRect[i] = boundingRect(Mat(contours[i]));
		AvatarKeyPoint.push_back(Point(cvRound(boundRect[i].x + boundRect[i].width / 2), cvRound(boundRect[i].y + boundRect[i].height / 2)));
	}

	AvatarKeyPointLine[0] = dis(AvatarKeyPoint[2] - AvatarKeyPoint[1]);
	AvatarKeyPointLine[1] = dis(AvatarKeyPoint[2] - AvatarKeyPoint[0]);
	AvatarKeyPointLine[2] = dis(AvatarKeyPoint[1] - AvatarKeyPoint[0]);



	if (AvatarKeyPointLine[0] >= AvatarKeyPointLine[2] && AvatarKeyPointLine[1] >= AvatarKeyPointLine[2])
	{
		AvatarKeyLine.push_back(AvatarKeyPoint[2] - AvatarKeyPoint[1]);
		AvatarKeyLine.push_back(AvatarKeyPoint[2] - AvatarKeyPoint[0]);
	}
	if (AvatarKeyPointLine[0] >= AvatarKeyPointLine[1] && AvatarKeyPointLine[2] >= AvatarKeyPointLine[1])
	{
		AvatarKeyLine.push_back(AvatarKeyPoint[1] - AvatarKeyPoint[0]);
		AvatarKeyLine.push_back(AvatarKeyPoint[1] - AvatarKeyPoint[2]);
	}
	if (AvatarKeyPointLine[1] >= AvatarKeyPointLine[0] && AvatarKeyPointLine[2] >= AvatarKeyPointLine[0])
	{
		AvatarKeyLine.push_back(AvatarKeyPoint[0] - AvatarKeyPoint[1]);
		AvatarKeyLine.push_back(AvatarKeyPoint[0] - AvatarKeyPoint[2]);
	}

	AvatarKeyLine = Vector2UnitVector(AvatarKeyLine);
	KeyLine = AvatarKeyLine[0] + AvatarKeyLine[1];
	rotationAngle = Line2Angle(KeyLine);
}

double ATM_TM_ORBAvatar::getRotationAngle()
{
	return rotationAngle;
}

double ATM_TM_ORBAvatar::dis(Point p)
{
	return sqrt(p.x * p.x + p.y * p.y);
}
std::vector<Point2f> ATM_TM_ORBAvatar::Vector2UnitVector(std::vector<Point2f> pLis)
{
	double length = 1;
	std::vector<Point2f> res;
	for (int i = 0; i < pLis.size(); i++)
	{
		length = sqrt(pLis[i].x * pLis[i].x + pLis[i].y * pLis[i].y);
		res.push_back(Point2f((float)(pLis[i].x / length), (float)(pLis[i].y / length)));
	}
	return res;
}

double ATM_TM_ORBAvatar::Line2Angle(Point2f p)
{
	const double rad2degScale = 180 / CV_PI;
	double res = atan2(-p.y, p.x) * rad2degScale;
	res = res - 90; //从屏幕空间左侧水平线为0度转到竖直向上为0度
	return res;
}

//void ATM_TM_Thread::run()
//{
//	while (isExitThread == false)
//	{
//		if (isRunWork && (*ptr) != nullptr)
//		{
//			ptr(workInput);
//			isRunWork = false;
//			isEndWork = true;
//		}
//		else
//		{
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		}
//	}
//}
//
//ATM_TM_Thread::ATM_TM_Thread()
//{
//	tLoopWork = new thread(&ATM_TM_Thread::run, this);
//}
//
//ATM_TM_Thread::~ATM_TM_Thread()
//{
//	if (tLoopWork != nullptr)
//	{
//		isExitThread = true;
//		tLoopWork->join();
//		delete tLoopWork;
//	}
//}
//
//ATM_TM_Thread::ATM_TM_Thread(void(*funPtr)(Mat &inMat))
//{
//	setFunction(funPtr);
//	tLoopWork = new thread(&ATM_TM_Thread::run, this);
//}
//
//void ATM_TM_Thread::setFunction(void(*funPtr)(Mat &inMat))
//{
//	ptr = funPtr;
//	isExistFunction = true;
//}
//
//void ATM_TM_Thread::start(Mat & inMat)
//{
//	if (isExistFunction == false)
//	{
//		throw"Not Found Work Function";
//	}
//	workInput = inMat;
//	isRunWork = true;
//	isEndWork = false;
//}
//
//bool ATM_TM_Thread::isEnd()
//{
//	return isEndWork;
//}

int ATM_TM_TemplateUID::getMaxID(double lis[], int len)
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

void ATM_TM_TemplateUID::Init()
{
	if (isInit)return;

	isInit = true;
}

void ATM_TM_TemplateUID::setUIDTemplate(Mat* uidTemplateMat)
{
	for (int i = 0; i < 10; i++)
	{
		uidTemplateMat[i].copyTo(giNumUID.n[i]);
	}
	uidTemplateMat[10].copyTo(giNumUID.UID);

}

void ATM_TM_TemplateUID::setUIDMat(Mat uidMat)
{
	if (uidMat.channels() == 4)
	{
		uidMat.copyTo(_uidMat);
	}
	else
	{
		cvtColor(uidMat, _uidMat, CV_RGB2RGBA);
	}
}

void ATM_TM_TemplateUID::TemplateUID()
{
	//待适配更多分辨率的UID

	int bitCount = 1;
	Mat tmp;
	Mat checkUID = giNumUID.UID;
	Mat Roi(_uidMat);

	matchTemplate(Roi, checkUID, tmp, TM_CCOEFF_NORMED);

	double minVal, maxVal;
	Point minLoc, maxLoc;
	//寻找最佳匹配位置
	minMaxLoc(tmp, &minVal, &maxVal, &minLoc, &maxLoc);
	if (maxVal > 0.75)
	{
		int x = maxLoc.x + checkUID.cols + 7;
		int y = 0;
		double tmplis[10] = { 0 };
		int tmplisx[10] = { 0 };
		for (int p = 8; p >= 0; p--)
		{
			_NumBit[p] = 0;
			for (int i = 0; i < giNumUID.max; i++)
			{
				Rect r(x, y, giNumUID.n[i].cols + 2, giNumUID.n[i].rows);//180-46/9->140/9->16->16*9=90+54=144
				if (x + r.width > _uidMat.cols)
				{
					r = Rect(_uidMat.cols - giNumUID.n[i].cols - 2, 0, giNumUID.n[i].cols + 2, giNumUID.n[i].rows);
				}

				Mat numCheckUID = giNumUID.n[i];
				Roi = _uidMat(r);

				matchTemplate(Roi, numCheckUID, tmp, TM_CCOEFF_NORMED);

				double minVali, maxVali;
				Point minLoci, maxLoci;
				//寻找最佳匹配位置
				minMaxLoc(tmp, &minVali, &maxVali, &minLoci, &maxLoci);

				tmplis[i] = maxVali;
				tmplisx[i] = maxLoci.x + numCheckUID.cols - 1;
				if (maxVali > 0.85)
				{
					_NumBit[p] = i;
					x = x + maxLoci.x + numCheckUID.cols - 1;
					break;
				}
				if (i == giNumUID.max - 1)
				{
					_NumBit[p] = getMaxID(tmplis, 10);
					x = x + tmplisx[_NumBit[p]];
				}
			}
			//if (p == 0)
			//{
			//	break;
			//}

		}
	}
	_uid = 0;
	for (int i = 0; i < 9; i++)
	{
		_uid += _NumBit[i] * bitCount;
		bitCount = bitCount * 10;
	}

}

int ATM_TM_TemplateUID::getUID()
{
	return _uid;
}

void ATM_TM_TemplateStar::Init()
{
	if (isInit)return;

	isInit = true;
}

void ATM_TM_TemplateStar::setStarTemplate(Mat starTemplateMat)
{
	_starTemplate = starTemplateMat;
}

void ATM_TM_TemplateStar::setStarMat(Mat starMat)
{
	_starMat = starMat;
}

void ATM_TM_TemplateStar::TemplateStar()
{
	int MAXLOOP = 0;
	bool isLoopMatch = false;
	Mat tmp;
	double minVal, maxVal;
	Point minLoc, maxLoc;

	pos.clear();

	matchTemplate(_starTemplate, _starMat, tmp, TM_CCOEFF_NORMED);
	minMaxLoc(tmp, &minVal, &maxVal, &minLoc, &maxLoc);
#ifdef _DEBUG
	cout << "Match Star MinVal & MaxVal : " << minVal << " , " << maxVal << endl;
#endif
	if (maxVal < 0.66)
	{
		isStarVisible = false;
	}
	else
	{
		isLoopMatch = true;
		isStarVisible = true;
		pos.push_back(maxLoc - Point(_starMat.cols / 2, _starMat.rows / 2) + Point(_starTemplate.cols / 2, _starTemplate.rows / 2));
	}

	while (isLoopMatch)
	{
		_starMat(Rect(maxLoc.x, maxLoc.y, _starTemplate.cols, _starTemplate.rows)) = Scalar(0, 0, 0);
		matchTemplate(_starTemplate, _starMat, tmp, TM_CCOEFF_NORMED);
		minMaxLoc(tmp, &minVal, &maxVal, &minLoc, &maxLoc);
#ifdef _DEBUG
		cout << "Match Star MinVal & MaxVal : " << minVal << " , " << maxVal << endl;
#endif
		if (maxVal < 0.66)
		{
			isLoopMatch = false;
		}
		else
		{
			pos.push_back(maxLoc - Point(_starMat.cols / 2, _starMat.rows / 2) + Point(_starTemplate.cols / 2, _starTemplate.rows / 2));
		}

		MAXLOOP > 10 ? isLoopMatch = false : MAXLOOP++;
	}
}

bool ATM_TM_TemplateStar::getStar()
{
	return isStarVisible;
}

vector<Point2d> ATM_TM_TemplateStar::getStarPos()
{
	return pos;
}
