#ifndef LANDMARKDETECTOR_H
#define LANDMARKDETECTOR_H

#include "common.h"
#include "landmarks.h"
#include "eigenface.h"

/**
 * @brief Class for landmark detection
 */
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
	/**
	 * @brief Depthmap to detect landmarks.
	 */
	cv::Mat _depthMap;

	void derivateVector(std::vector<float> &src, std::vector<float> &dst);

};

#endif // LANDMARKDETECTOR_H
