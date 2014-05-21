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
#include "mesh.h"

class Controller {
public:
	Controller();

	void averageLandmarks(QVector<cv::Mat> &faces, QStringList &labels, Landmarks &avgLandmarks);

	void createDepthmap(Mesh &face, Mesh &modelFace, cv::Mat &depthmap, double &distance, int &iterations);

	void createPcaSubspaces(QVector<cv::Mat> &faces,Landmarks &avgLandmarks, EigenFace &eigenface, FaceDivider::DivideMethod method);

	bool detectLandmarks(const cv::Mat &depthmap, Landmarks &landmarks);

	void processFace(cv::Mat &depthmap,
					 QString label,
					 Landmarks &averageLandmarks,
					 bool &result,
					 FaceDivider::DivideMethod method,
					 EigenFace &subspaces,
					 tFeatures &featuresVector,
					 bool showBackProjections = false);


	void procesFaceArena(cv::Mat &depthmap,
						 QString label,
						 Landmarks &averageLandmarks,
						 bool &result,
						 FaceDivider::DivideMethod method,
						 QVector<cv::Mat> &featuresVector);


};

#endif // CONTROLLER_H
