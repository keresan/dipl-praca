#ifndef DEPTHMAP_H
#define DEPTHMAP_H

#include "mesh.h"
#include "common.h"

 #include <QVarLengthArray>

class DepthMap {
public:
	DepthMap(Mesh &face, cv::Point2d topLeft = cv::Point2d(-150,150),
			 cv::Point2d bottomRight = cv::Point2d(150,-150),
			 int pixelsX = 300,
			 int piselxY = 300);

    cv::Mat depthMap;

	bool mapPointToIndecies(cv::Point2d p, int &row, int &col);
	bool mapIndeciesToPoint(int row, int col, cv::Point2f &p);

	int selectFromDepthMap(cv::Point2d tl, cv::Point2d br, QVector<cv::Point2f> &vector);

    void printPoints();

private:
	double _min, _max;
	//spread of depthMap
	cv::Point2d _topLeft, _bottomRight;

	void createDepthMap(Mesh &face);

	double weightedArtMean(double x, double y, double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2);
	double linearInterpolation(double x, double y, double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2);

	void policing(double min = -250, double max = 200);
	void computeMinMax();

	inline double max(double a, double b, double c) {
		if(a>b) {
			if(a>c) {
				return a;
			} else {
				return c;
			}
		} else {
			if(b>c) {
				return b;
			} else {
				return c;
			}
		}
	}

	inline double min(double a, double b, double c) {
		if(a<b) {
			if(a<c) {
				return a;
			} else {
				return c;
			}
		} else {
			if(b<c) {
				return b;
			} else {
				return c;
			}
		}
	}

};

#endif // DEPTHMAP_H
