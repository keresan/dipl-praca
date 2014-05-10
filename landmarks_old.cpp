#include "landmarks_old.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

void LandmarksOld::setColor(QColor color) {_color = color;}
void LandmarksOld::set(LandmarkNamesOld name, cv::Point3d pos) { points[name] = pos;}


QColor LandmarksOld::color() { return _color;}
bool LandmarksOld::is(LandmarkNamesOld name) {return points[name] != cv::Point3d(-9999,-9999,-9999); }
QVector<cv::Point3d>* LandmarksOld::getLandmarks() { return &points;}
cv::Point3d LandmarksOld::pos(LandmarkNamesOld name) {return points[name];}


bool LandmarksOld::is(unsigned int name) {
	assert(name < (unsigned int)points.count());

    return points[name] != cv::Point3d(-9999,-9999,-9999);

}

void LandmarksOld::set(unsigned int name, cv::Point3d pos) {
	assert(name < (unsigned int)points.count());
    points[name] = pos;
}

cv::Point3d LandmarksOld::pos(unsigned int name) {
	assert(name < (unsigned int)points.count());
    return points[name];
}
