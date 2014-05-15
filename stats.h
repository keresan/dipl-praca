#ifndef STATS_H
#define STATS_H

#include "common.h"

class Stats
{
public:
	Stats();

	static void computeMinMaxAvg(cv::Mat &values, cv::Mat &result);

	void computeHistogram(cv::Mat &values);
};


#endif // STATS_H
