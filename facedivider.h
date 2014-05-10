#ifndef FACEDIVIDER_H
#define FACEDIVIDER_H

#include "landmarks.h"

typedef QVector<cv::Mat> tFaceAreas;

class FaceDivider {
public:

	enum DivideMethod {
		toHalfByNoseBottom,
		method2,
		method3
	};

	FaceDivider(cv::Mat &depthMap, Landmarks &landmarks, Landmarks &averageLandmarks);

	void divide(DivideMethod method,tFaceAreas &areas);

	void divideHalfByNoseBottom(tFaceAreas &areas);

	void divideByMethod2(tFaceAreas &areas);
	void divideByMethod3(tFaceAreas &areas);

	void resizeArea(cv::Mat &src, cv::Mat &dst, int height, int width);


private:
	cv::Mat _depthMap;
	Landmarks _landmarks;
	Landmarks _averageLandmarks;

};

#endif // FACEDIVIDER_H
