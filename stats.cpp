#include "stats.h"

Stats::Stats() {

}

void Stats::computeMinMaxAvg(cv::Mat &values, cv::Mat &result) {

	result = cv::Mat::zeros(3,values.cols,values.type());

	for(int c = 0; c < values.cols; c++) {
		double min,max,mean;
		mean = cv::mean(values.col(c))[0];

		cv::minMaxLoc(values.col(c),&min, &max);

		result.at<float>(0,c) = mean;
		result.at<float>(1,c) = min;
		result.at<float>(2,c) = max;

	}

}

void Stats::computeHistogram(cv::Mat &values) {

	;


}
