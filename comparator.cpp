#include "comparator.h"

Comparator::Comparator() {}

/**
 * @brief Comparator::compare
 * Compare two matrix of features.
 * @param features1 Features 1
 * @param features2 Features 2
 * @param result Result of comparation
 * @param method Comparation method
 */
void Comparator::compare(tFeatures &features1, tFeatures &features2, QVector<float> &result, Comparator::CompareMethod method) {

	if(features1.rows != features2.rows) {
		throw std::runtime_error("compare(): features size mishmash");
	}

	result.clear();

	for(int i = 0; i < features1.rows; i++) {
		cv::Mat f1,f2;
		f1 = features1.row(i);
		f2 = features2.row(i);
		float distance = compareFeature(f1,f2, method);
		result.append(distance);
	}
}

/**
 * @brief Comparator::compare
 * Compare two vector of matrices of features.
 * @param features1 Features 1
 * @param features2 Features 2
 * @param result Result of comparation
 * @param method Comparation method
 */
void Comparator::compare(QVector<cv::Mat> &features1, QVector<cv::Mat> &features2, QVector<float> &result, Comparator::CompareMethod method) {

	if(features1.size() != features2.size()) {
		throw std::runtime_error("compare(): features size mishmash");
	}

	result.clear();

	for(int i = 0; i < features1.size(); i++) {
		cv::Mat f1,f2;
		f1 = features1.at(i);
		f2 = features2.at(i);

		if(f1.rows != f2.rows || f1.cols != f2.cols) {
			throw std::runtime_error("compare(): features matrices size mishmash");
		}

		float distance = compareFeature(f1,f2, method);
		result.append(distance);
	}
}

/**
 * @brief Comparator::compareFeature
 * Compare two features. Call compare funciton by compare metod.
 * @param feature1 Feature
 * @param feature2 Feature
 * @param method Comparation method
 * @return Result of comparation
 */
double Comparator::compareFeature(cv::Mat &feature1, cv::Mat &feature2, Comparator::CompareMethod method) {
	assert(feature1.rows == feature2.rows);
	assert(feature1.rows == 1);

	cv::Mat f1(feature1.row(0));
	cv::Mat f2(feature2.row(0));

	switch(method) {
		case CityblockDistance:
			return cv::norm(f1,f2, cv::NORM_L1);
		break;
		case EuclidianDistance:
			return cv::norm(f1,f2, cv::NORM_L2);
		break;
		case CorrelationDistance:
			return correlationDistance(f1,f2);
		break;

		default:
			assert(false);
	}
}

/**
 * @brief Comparator::correlationDistance
 * Compute correlation distance btween to feature vectors
 * @param feature1 Features
 * @param feature2 Features
 * @return Correlation
 */
double Comparator::correlationDistance(cv::Mat &feature1, cv::Mat &feature2) {

	return 1.0 - correlationMetric(feature1, feature2);
}


/**
 * @brief Comparator::correlationMetric
 * Compute correlation distance btween to feature vectors
 * @param feature1 Feature
 * @param feature2 Feature
 * @return Correlation
 */
double Comparator::correlationMetric(cv::Mat &feature1, cv::Mat &feature2) {

	cv::Scalar mean1, mean2, std1, std2;
	cv::meanStdDev(feature1,mean1,std1);
	cv::meanStdDev(feature2,mean2,std2);


	double sum = 0.0;
	for (int i = 0; i < feature1.cols; i++) {
		sum += ((feature1.at<float>(0,i) - mean1[0])/std1[0]) * ((feature2.at<float>(0,i) - mean2[0])/std2[0]);
	}

	return (1.0/(feature1.cols-1.0)) * sum;

}
