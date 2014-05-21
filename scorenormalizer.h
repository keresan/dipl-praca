#ifndef SCORENORMALIZER_H
#define SCORENORMALIZER_H

#include "common.h"

class ScoreNormalizer {
public:
	enum NormalizeMethod {
		zScore = 1
	};

	ScoreNormalizer();
	ScoreNormalizer(ScoreNormalizer::NormalizeMethod method, cv::Mat &trainData);
	ScoreNormalizer(ScoreNormalizer::NormalizeMethod method, cv::Mat &trainData1, cv::Mat &trainData2);
	ScoreNormalizer(ScoreNormalizer::NormalizeMethod method, QString fileName, QString dirPath = Common::pathToScnData);

	void normalize(QVector<float> &inputScore, QVector<float> &normScore);

	void save(QString fileName, QString dirPath = Common::pathToScnData);
	void load(QString fileName, QString dirPath = Common::pathToScnData);

private:
	void prepareZScore(cv::Mat &trainData);
	void concatAsComparable(cv::Mat &src1dst, cv::Mat &src2);
	void normalizeZScore(QVector<float> &inputScore, QVector<float> &normScore);

	NormalizeMethod _method;

	QVector<float> _stdDeviaton;
	QVector<float> _mean;

};

#endif // SCORENORMALIZER_H
