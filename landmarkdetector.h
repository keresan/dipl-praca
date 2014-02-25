#ifndef LANDMARKDETECTOR_H
#define LANDMARKDETECTOR_H

#include "common.h"

class LandmarkDetector
{
public:
    LandmarkDetector();

    cv::Point3d detectNoseTip();
};

#endif // LANDMARKDETECTOR_H
