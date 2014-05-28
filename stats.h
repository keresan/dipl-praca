#ifndef STATS_H
#define STATS_H

#include "common.h"
#include "qmath.h"

/**
 * @brief Statistical values for one area face.
 */
class StatValues {
public:
	StatValues() {};

	/**
	 * @brief equal error rate
	 */
	float eer;
	/**
	 * @brief minimum value
	 */
	float min;
	/**
	 * @brief maximum value
	 */
	float max;
	/**
	 * @brief histogram of imposters users
	 */
	cv::MatND histImposer;
	/**
	 * @brief histogram of genuine users
	 */
	cv::MatND histGenuine;
};

/**
 * @brief Class of statistacal data.
 */
class Stats
{
public:
	Stats(cv::Mat imposterResult, cv::Mat genuineResult);

	void computeHist(int col, bool normAsPercentage = true);
	void computeHist(int col, float min, float max, bool normAsPercentage = true);
	void printHist(int col);

	void computeMinMaxAvg();
	void computeFmrFnrm(int col, float &fmr, float &fnmr, float threshold);
	void printGraphFmrFnmr(int col, float thresholdStart, float thresholdStop, float step);
	void computeEer();

	void static convertDotInNumber(float number,QString &strNumber,int precision = 6);

	/**
	 * @brief Vector of statistical data for each face area
	 */
	QVector<StatValues> statValuesVector;

	void saveEer(QString fileName, QString dirPath = Common::pathToScnData);
	static void loadEer(QVector<float> &errVector, QString fileName, QString dirPath = Common::pathToScnData);

private:

	/**
	 * @brief Imposter users set
	 */
	cv::Mat _impResult;
	/**
	 * @brief Genuine users set
	 */
	cv::Mat _genResult;
};


#endif // STATS_H
