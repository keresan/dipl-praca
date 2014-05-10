#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "QStringList"
#include "QDir"

#include "averageface.h"
#include "facealigner.h"
#include "depthmap.h"
#include "glwidget.h"
#include "eigenface.h"
#include "landmarkdetector.h"
#include "landmarks.h"
#include "facedivider.h"
#include "comparator.h"

class Controller {
public:
	Controller();

	void averageLandmarks(QVector<cv::Mat> &faces, QStringList &labels, Landmarks &avgLandmarks);
	void createPcaSubspaces(QVector<cv::Mat> &faces,Landmarks &avgLandmarks, EigenFace &eigenface, FaceDivider::DivideMethod method);

	bool detectLandmarks(const cv::Mat &depthmap, Landmarks &landmarks);

	void processFace(cv::Mat &depthmap, QString label,
					 Landmarks &averageLandmarks,
					 bool &result,
					 FaceDivider::DivideMethod method,
					 EigenFace &subspaces,
					 tFeatures &featuresVector);


};

#endif // CONTROLLER_H
