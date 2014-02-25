#ifndef LANDMARKS_H
#define LANDMARKS_H

#include <QString>
#include <QVector>
#include <opencv2/core/core.hpp>
#include <QColor>

#include "common.h"

class Landmarks
{
public:

    QVector<cv::Point3d> _points;
    Landmarks() : _points(QVector<cv::Point3d>(8, cv::Point3d(-9999,-9999,-9999))), _color(Qt::yellow) { }

    enum LandmarkNames {
        RightOuterEye = 0,
        RightInnerEye = 1,
        LeftInnerEye = 2,
        LeftOuterEye = 3,
        NoseTip = 4,
        RightMouthCorner = 5,
        LeftMouthCorner = 6,
        mean = 7
    };

    QColor color();
    cv::Point3d pos(LandmarkNames name);
    cv::Point3d pos(unsigned int name);

    bool is(LandmarkNames name);
    bool is(unsigned int name);

    void setColor(QColor color);

    void set(LandmarkNames name, cv::Point3d pos);
    void set(unsigned int name, cv::Point3d pos);

    QVector<cv::Point3d>* getLandmarks();


private:

    QColor _color;
};


#endif // LANDMARKS_H
