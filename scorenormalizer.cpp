#include "scorenormalizer.h"

ScoreNormalizer::ScoreNormalizer() {

}

ScoreNormalizer::ScoreNormalizer(ScoreNormalizer::NormalizeMethod method, cv::Mat &trainData) {

	_method = method;
	switch(method) {
		case zScore:
			prepareZScore(trainData);
		break;
		default:
			assert(false);
	}
}

/**
 * @brief ScoreNormalizer::ScoreNormalizer - firstly, equal size of training set - multiple smallest one to be
 * equal size as biggest one
 * @param method
 * @param trainData1
 * @param trainData2
 */
ScoreNormalizer::ScoreNormalizer(ScoreNormalizer::NormalizeMethod method, cv::Mat &trainData1, cv::Mat &trainData2) {
	_method = method;
	switch(method) {
		case zScore:
			concatAsComparable(trainData1,trainData2);
			prepareZScore(trainData1);
		break;
		default:
			assert(false);
	}
}

ScoreNormalizer::ScoreNormalizer(ScoreNormalizer::NormalizeMethod method, QString fileName, QString dirPath) {
	_method = method;
	load(fileName,dirPath);
}




void ScoreNormalizer::normalize(QVector<float> &inputScore, QVector<float> &normScore) {
	switch(_method) {
		case zScore:
			normalizeZScore(inputScore,normScore);
		break;
		default:
			assert(false);
	}
}

void ScoreNormalizer::prepareZScore(cv::Mat &trainData) {

	for(int c = 0; c < trainData.cols; c++) {
		cv::Scalar mean, std;

		cv::meanStdDev(trainData.col(c),mean,std);

		_mean.append(mean[0]);
		_stdDeviaton.append(std[0]);
	}
}

void ScoreNormalizer::concatAsComparable(cv::Mat &src1dst, cv::Mat &src2) {

	/*
	qDebug() << "src1dst:" << src1dst.rows << "x" << src1dst.cols;
	qDebug() << "src2:" << src2.rows << "x" << src2.cols;
	cv::Scalar mean, std;

	cv::Mat tmpConcat;
	cv::vconcat(src1dst,src2,tmpConcat);
	cv::meanStdDev(tmpConcat.col(0),mean, std);
	qDebug() << "tmpConcat:" << mean[0] << std[0];


	cv::meanStdDev(src1dst.col(0),mean, std);
	qDebug() << "src1dst:" << mean[0] << std[0];
	cv::meanStdDev(src2.col(0),mean, std);
	qDebug() << "src2:" << mean[0] << std[0];
	*/

	//which one is bigger ?
	cv::Mat big, small;
	if(src1dst.rows > src2.rows) {
		big = src1dst;
		small = src2;
	} else {
		big = src2;
		small = src1dst;
	}

	// matToSaveGenuine << matToSaveImposter => je nutne rozmnozit matToSaveGenuine aby bola porovnatejnej
	// velkosti ako matToSaveImposter
	// tato operacia je dolezita pri vyuziti z-score, inak bude davat skreslene vysledky
	int diff = big.rows / small.rows;
	cv::Mat tmp = small;
	for(int i = 0; i < diff; i++) {
		cv::vconcat(small,tmp,small);
	}

	/*
	cv::meanStdDev(small.col(0),mean, std);
	qDebug() << "small:" << mean[0] << std[0];
	cv::meanStdDev(big.col(0),mean, std);
	qDebug() << "big:" << mean[0] << std[0];

	qDebug() << "small:" << small.rows << "x" << small.cols;
	qDebug() << "big:" << big.rows << "x" << big.cols;
	*/

	//contac big and small
	cv::vconcat(big,small,src1dst);

	/*
	cv::meanStdDev(src1dst.col(0),mean, std);
	qDebug() << "after concat:" << mean[0] << std[0];
	qDebug() << "after concat:" << src1dst.rows << "x" << src1dst.cols;
	*/

}

void ScoreNormalizer::normalizeZScore(QVector<float> &inputScore, QVector<float> &normScore) {

	normScore.clear();

	for(int i = 0; i < inputScore.size(); i++) {

			double value = ( inputScore.at(i) - _mean.at(i) ) / _stdDeviaton.at(i);
			normScore.append(value);
	}
}


void ScoreNormalizer::save(QString fileName, QString dirPath) {
	QDir dir(dirPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::WRITE);

	storage << Common::vectorSizeLabel.toStdString() << _mean.size();

	for(int i =0; i < _mean.size(); i++) {
		storage  << Common::scnColumnLabel.toStdString()+std::to_string(i) << "{";

		storage  << Common::scnMeanLabel.toStdString() << _mean[i];
		storage  << Common::scnStdDevLabel.toStdString() << _stdDeviaton[i];

		storage << "}";
	}

	storage.release();
}

void ScoreNormalizer::load(QString fileName, QString dirPath) {

	QDir dir(dirPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::READ);

	int size;
	storage[Common::vectorSizeLabel.toStdString()] >> size;

	_mean.clear();
	_stdDeviaton.clear();

	for(int i =0; i < size; i++) {

		float tmp;
		storage[Common::scnColumnLabel.toStdString()+std::to_string(i)][Common::scnMeanLabel.toStdString()] >> tmp;
		_mean.append(tmp);

		storage[Common::scnColumnLabel.toStdString()+std::to_string(i)][Common::scnStdDevLabel.toStdString()] >> tmp;
		_stdDeviaton.append(tmp);

	}

	storage.release();
}

