#ifndef LANDMARKS_H
#define LANDMARKS_H

#include "common.h"

#include <QPair>

typedef QVector<QPair<cv::Point,bool> > VectorOfLandmarks;

class Landmarks {

public:
	Landmarks(VectorOfLandmarks _landmarks);
	Landmarks(QString fileName, QString loadPath = Common::lmPathToLmDir);
	Landmarks() : _landmarks(VectorOfLandmarks(7, qMakePair(cv::Point(0,0), false))) { }

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

	void save(QString fileName, QString savePath = Common::lmPathToLmDir);
	void load(QString fileName, QString loadPath = Common::lmPathToLmDir);

	VectorOfLandmarks getLandmarks();

private:
	VectorOfLandmarks _landmarks;

};

#endif // LANDMARKS_H
