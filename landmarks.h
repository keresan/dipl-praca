#ifndef LANDMARKS_H
#define LANDMARKS_H

#include "common.h"

#include <QPair>

/**
 * @brief Struct for store lanndmark
 */
typedef QVector<QPair<cv::Point,bool> > VectorOfLandmarks;

/**
 * @brief Class for store landmarks position.
 */
class Landmarks {

public:
	Landmarks(VectorOfLandmarks _landmarks);
	Landmarks(QString fileName, QString loadPath = Common::pathToLandmarkDir);
	Landmarks() : _landmarks(VectorOfLandmarks(7, qMakePair(cv::Point(0,0), false))) { }

	/**
	 * @brief Types of landmark
	 */
	enum LandmarkNames {
		NoseTip = 0,
		NoseCornerLeft = 1,
		NoseCornerRight = 2,
		NoseBottom = 3,
		NoseRoot = 4,
		EyeInnerCornerLeft = 5,
		EyeInnerCornerRight = 6
	};

	cv::Point pos(Landmarks::LandmarkNames name);
	bool is(Landmarks::LandmarkNames name);
	void set(Landmarks::LandmarkNames name, cv::Point pos);
	void discard(Landmarks::LandmarkNames name);

	void save(QString fileName, QString savePath = Common::pathToLandmarkDir);
	void load(QString fileName, QString loadPath = Common::pathToLandmarkDir);

	VectorOfLandmarks getLandmarks();

	void scale(float scaleFactor);

private:
	/**
	 * @brief Vector of landmarks
	 */
	VectorOfLandmarks _landmarks;

};

#endif // LANDMARKS_H
