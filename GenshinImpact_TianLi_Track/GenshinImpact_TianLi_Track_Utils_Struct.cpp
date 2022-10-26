#include "pch.h"
#include "GenshinImpact_TianLi_Track_Utils_Struct.h"

Kalman::Kalman()
{
	type = FilterType::Kalman;

	KF = cv::KalmanFilter(stateNum, measureNum, 0);
	state = cv::Mat(stateNum, 1, CV_32F); //state(x,y,detaX,detaY)
	processNoise = cv::Mat(stateNum, 1, CV_32F);
	measurement = cv::Mat::zeros(measureNum, 1, CV_32F);	//measurement(x,y)

	randn(state, cv::Scalar::all(0), cv::Scalar::all(0.1)); //�������һ������������0����׼��Ϊ0.1;
	KF.transitionMatrix = (cv::Mat_<float>(4, 4) <<
		1, 0, 1, 0,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1);//Ԫ�ص�����󣬰���;

	//setIdentity: ���ŵĵ�λ�ԽǾ���;
	//!< measurement matrix (H) �۲�ģ��
	setIdentity(KF.measurementMatrix);

	//!< process noise covariance matrix (Q)
	// wk �ǹ������������ٶ�����Ͼ�ֵΪ�㣬Э�������ΪQk(Q)�Ķ�Ԫ��̬�ֲ�;
	setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-5));

	//!< measurement noise covariance matrix (R)
	//vk �ǹ۲����������ֵΪ�㣬Э�������ΪRk,�ҷ�����̬�ֲ�;
	setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-1));

	//!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/  A����F: transitionMatrix
	//Ԥ�����Э�������;
	setIdentity(KF.errorCovPost, cv::Scalar::all(1));

	//!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
	//initialize post state of kalman filter at random 
	randn(KF.statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));
}

Kalman::~Kalman()
{
}

cv::Point2d Kalman::filterting(const cv::Point2d& pos)
{
	cv::Mat prediction = KF.predict();
	cv::Point2d predictPt = cv::Point2d(prediction.at<float>(0), prediction.at<float>(1));

	//3.update measurement
	measurement.at<float>(0, 0) = static_cast<float>(pos.x);
	measurement.at<float>(1, 0) = static_cast<float>(pos.y);

	//4.update
	KF.correct(measurement);

	cv::Point2d resP = cv::Point2d(KF.statePost.at<float>(0), KF.statePost.at<float>(1));
	return resP;
}

cv::Point2d Kalman::re_init_filterting(const cv::Point2d& pos)
{
	KF.init(stateNum, measureNum, 0);

	state = cv::Mat(stateNum, 1, CV_32F); //state(x,y,detaX,detaY)
	processNoise = cv::Mat(stateNum, 1, CV_32F);
	measurement = cv::Mat::zeros(measureNum, 1, CV_32F);	//measurement(x,y)

	randn(state, cv::Scalar::all(0), cv::Scalar::all(0.1)); //�������һ������������0����׼��Ϊ0.1;
	KF.transitionMatrix = (cv::Mat_<float>(4, 4) <<
		1, 0, 1, 0,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1);//Ԫ�ص�����󣬰���;

	//setIdentity: ���ŵĵ�λ�ԽǾ���;
	//!< measurement matrix (H) �۲�ģ��
	setIdentity(KF.measurementMatrix);

	//!< process noise covariance matrix (Q)
	// wk �ǹ������������ٶ�����Ͼ�ֵΪ�㣬Э�������ΪQk(Q)�Ķ�Ԫ��̬�ֲ�;
	setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-5));

	//!< measurement noise covariance matrix (R)
	//vk �ǹ۲����������ֵΪ�㣬Э�������ΪRk,�ҷ�����̬�ֲ�;
	setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-1));

	//!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/  A����F: transitionMatrix
	//Ԥ�����Э�������;
	setIdentity(KF.errorCovPost, cv::Scalar::all(1));

	//!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
	//initialize post state of kalman filter at random 
	randn(KF.statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));

	KF.statePost.at<float>(0) = static_cast<float>(pos.x);
	KF.statePost.at<float>(1) = static_cast<float>(pos.y);

	cv::Mat prediction = KF.predict();
	cv::Point2d predictPt = cv::Point2d(prediction.at<float>(0), prediction.at<float>(1));

	//3.update measurement
	measurement.at<float>(0, 0) = static_cast<float>(pos.x);
	measurement.at<float>(1, 0) = static_cast<float>(pos.y);

	//4.update
	KF.correct(measurement);

	cv::Point2d resP = cv::Point2d(KF.statePost.at<float>(0), KF.statePost.at<float>(1));

	return resP;
}