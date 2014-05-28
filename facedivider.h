#ifndef FACEDIVIDER_H
#define FACEDIVIDER_H

#include "landmarks.h"

typedef QVector<cv::Mat> tFaceAreas;

/**
 * @brief Class for divide face to areas
 */
class FaceDivider {
public:

	/**
	 * @brief Method of divide
	 */
	enum DivideMethod {
		method0,
		method1,
		method2,
		method3,
		method1rigid,
		method2rigid,
		method3rigid
	};

	FaceDivider(cv::Mat &depthMap, Landmarks &landmarks, Landmarks &averageLandmarks);

	void divide(DivideMethod method,tFaceAreas &areas);


	void divideByMethod0(tFaceAreas &areas);
	void divideByMethod1(tFaceAreas &areas);
	void divideByMethod2(tFaceAreas &areas);
	void divideByMethod3(tFaceAreas &areas);
	void divideByMethod1rigid(tFaceAreas &areas);
	void divideByMethod2rigid(tFaceAreas &areas);
	void divideByMethod3rigid(tFaceAreas &areas);


	void resizeArea(cv::Mat &src, cv::Mat &dst, int height, int width);

	void setResizeParam(int finalWidth);


private:
	/**
	 * @brief Face to divide
	 */
	cv::Mat _depthmap;
	/**
	 * @brief Face will be divided used this landmarks
	 */
	Landmarks _landmarks;

	/**
	 * @brief Need for scale areas
	 */
	Landmarks _averageLandmarks;

};

#endif // FACEDIVIDER_H
