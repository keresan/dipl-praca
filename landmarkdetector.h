#ifndef LANDMARKDETECTOR_H
#define LANDMARKDETECTOR_H

#include "common.h"
#include "landmarks.h"
#include "eigenface.h"

class LandmarkDetector {
public:
	LandmarkDetector(cv::Mat depthMap);

	void detectNoseTip();
	void detectNoseRoot();
	void detectNoseCorners();
	void detectInnerEyeCorners();
	void detectNoseBottom();

	bool detectAll(Landmarks &landmarks);

	static bool checkLandmarks(Landmarks &srcLandmarks, Landmarks &refLandmarks);

	cv::Point noseTip;
	cv::Point noseRoot;
	cv::Point eyeInnerCornerLeft;
	cv::Point eyeInnerCornerRight;
	cv::Point noseCornerLeft;
	cv::Point noseCornerRight;
	cv::Point noseBottom;

private:
	cv::Mat _depthMap;
	/**
	* od pozicie nosa sa urcia dalsie pozicie.
	*
	*/


	void derivateVector(std::vector<float> &src, std::vector<float> &dst);
	void averageBlur(std::vector<float> &src, std::vector<float> &dst, int ksize);


};

#endif // LANDMARKDETECTOR_H
