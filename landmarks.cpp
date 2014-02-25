#include "landmarks.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>




void Landmarks::setColor(QColor color) {_color = color;}
void Landmarks::set(LandmarkNames name, cv::Point3d pos) { _points[name] = pos;}


QColor Landmarks::color() { return _color;}
bool Landmarks::is(LandmarkNames name) {return _points[name] != cv::Point3d(-9999,-9999,-9999); }
QVector<cv::Point3d>* Landmarks::getLandmarks() { return &_points;}
cv::Point3d Landmarks::pos(LandmarkNames name) {return _points[name];}


bool Landmarks::is(unsigned int name) {
    assert(name < _points.count());

    return _points[name] != cv::Point3d(-9999,-9999,-9999);

}

void Landmarks::set(unsigned int name, cv::Point3d pos) {
    assert(name < _points.count());
    _points[name] = pos;
}

cv::Point3d Landmarks::pos(unsigned int name) {
    assert(name < _points.count());
    return _points[name];
}
