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
		CityblockDistance
	};

	void static compare(tFeatures &features1, tFeatures &features2, QVector<float> &result, Comparator::CompareMethod method);

	double static compareFeature(cv::Mat &feature1, cv::Mat &feature2, Comparator::CompareMethod method);
};

#endif // COMPARATOR_H
