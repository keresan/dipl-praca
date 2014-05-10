#include "comparator.h"

Comparator::Comparator() {

}


void Comparator::compare(tFeatures &features1, tFeatures &features2, QVector<double> &result, Comparator::CompareMethod method) {
	assert(features1.rows == features2.rows);

	result.clear();

	for(int i = 0; i < features1.rows; i++) {
		cv::Mat f1,f2;
		f1 = features1.row(i);
		f2 = features2.row(i);
		double distance = compareFeature(f1,f2, method);
		result.append(distance);

	}


}


double Comparator::compareFeature(cv::Mat &feature1, cv::Mat &feature2, Comparator::CompareMethod method) {
	assert(feature1.rows == feature2.rows == 1);
	switch(method) {
		case EuclidianDistance:
			return cv::norm(feature1.row(0), feature2.row(0), cv::NORM_L2);
		break;
		default:
			assert(false);
	}
}
