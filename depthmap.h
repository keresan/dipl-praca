#ifndef DEPTHMAP_H
#define DEPTHMAP_H

#include "mesh.h"
#include "common.h"



class DepthMap {
public:
    DepthMap();

    void createDepthMap(Mesh &face);

    cv::Mat depthMap;

    //spread of depthMap
    cv::Point2d topLeft, bottomRight;

    bool mapPointToIndecies(cv::Point2d p, int &row, int &col);
    bool mapIndeciesToPoint(int row, int col, cv::Point2f &p);

    void selectFromDepthMap(cv::Rect rectangle, QVector<cv::Point2f> &vector);

    double averageValueInTraignle(cv::Point2f queryPoint, Mesh &face);

    void test1(Mesh &face);

    void printPoints();


};

#endif // DEPTHMAP_H
