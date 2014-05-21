#ifndef STATS_H
#define STATS_H

#include "common.h"

class StatValues {
public:
	StatValues() {};

	float eer;
	float min;
	float max;
	float mean;
	cv::MatND histImposer;
	cv::MatND histGenuine;



};

class Stats
{
public:
	Stats(cv::Mat imposterResult, cv::Mat genuineResult);

	void computeHistogram(bool normAsPercentage = true);
	void printHist(int col);

	void computeMinMaxAvg();
	void computeFmrFnrm(int col, float &fmr, float &fnmr, float threshold);
	void computeEer();

	void static convertDotInNumber(float number,QString &strNumber,int precision = 6);


	QVector<StatValues> statValuesVector;

	void saveEer(QString fileName, QString dirPath = Common::pathToScnData);
	static void loadEer(QVector<float> &errVector, QString fileName, QString dirPath = Common::pathToScnData);



private:

	cv::Mat _impResult, _genResult;



};


#endif // STATS_H
