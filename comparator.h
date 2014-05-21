#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "common.h"
#include "eigenface.h"

class Comparator
{
public:
	Comparator();
	enum CompareMethod {
		EuclidianDistance,
		CityblockDistance,
		CorrelationDistance
	};

	void static compare(tFeatures &features1, tFeatures &features2, QVector<float> &result, Comparator::CompareMethod method);
	void static compare(QVector<cv::Mat> &features1, QVector<cv::Mat> &features2, QVector<float> &result, Comparator::CompareMethod method);

	double static compareFeature(cv::Mat &feature1, cv::Mat &feature2, Comparator::CompareMethod method);


	double static correlationDistance(cv::Mat &feature1, cv::Mat &feature2);
	double static correlationMetric(cv::Mat &feature1, cv::Mat &feature2);
};

#endif // COMPARATOR_H
