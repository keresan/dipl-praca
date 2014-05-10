#ifndef DEPTHMAP_H
#define DEPTHMAP_H

#include "mesh.h"
#include "common.h"
#include "landmarks.h"
#include <QVarLengthArray>

class DepthMap {
public:
	DepthMap(Mesh &face, cv::Point2d topLeft = Common::depthMapTL,
			 cv::Point2d bottomRight = Common::depthMapBR,
			 int pixelsX = Common::depthMapPixelsX,
			 int piselxY = Common::depthMapPixelsY);

    cv::Mat depthMap;

	bool mapPointToIndecies(cv::Point2d p, int &row, int &col);
	bool mapIndeciesToPoint(int row, int col, cv::Point2f &p);

	int selectFromDepthMap(cv::Point2d tl, cv::Point2d br, QVector<cv::Point2f> &vector);

	static void showAllLandmarks(cv::Mat const &depthMap, Landmarks &landmarks, QString label, bool addTime = true);

    void printPoints();

private:
	float _min, _max;
	//spread of depthMap
	cv::Point2d topLeft, bottomRight;

	void createDepthMap(Mesh &face);

	float weightedArtMean(float x, float y, float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2, float z2);
	float linearInterpolation(float x, float y, float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2,float z2);

	void policing(float min = -250, float max = 200, bool shiftToZero = true);
	void computeMinMax();

	inline float max(float a, float b, float c) {
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

	inline float min(float a, float b, float c) {
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
