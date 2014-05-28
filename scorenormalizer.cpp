#include "scorenormalizer.h"

ScoreNormalizer::ScoreNormalizer() {

}

/**
 * @brief Constructor.
 * @param method Method of score normalization
 * @param trainData Data from traning set. Need for set parameters for score normmalization
 */
ScoreNormalizer::ScoreNormalizer(ScoreNormalizer::NormalizeMethod method, cv::Mat &trainData) {

	_method = method;
	switch(method) {
		case zScore:
			prepareZScore(trainData);
		break;
		default:
			throw std::runtime_error("ScoreNormalizer(): unknown normalization method");
	}
}

/**
 * @brief Constructor. Prepare parameters for score normalization.
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

/**
 * @brief Constructor. Load parameter of normalization from file.
 * @param method Method of normalization
 * @param fileName File to load
 * @param dirPath Directory with file
 */
ScoreNormalizer::ScoreNormalizer(ScoreNormalizer::NormalizeMethod method, QString fileName, QString dirPath) {
	_method = method;
	load(fileName,dirPath);
}

/**
 * @brief Normalize vector of values
 * @param inputScore Vector of float
 * @param normScore Normalize vector of float
 */
void ScoreNormalizer::normalize(QVector<float> &inputScore, QVector<float> &normScore) {
	switch(_method) {
		case zScore:
			normalizeZScore(inputScore,normScore);
		break;
		default:
			assert(false);
	}
}

/**
 * @brief Compute parameters for zscore normalization.
 * @param trainData Datesets from which compute parameters
 */
void ScoreNormalizer::prepareZScore(cv::Mat &trainData) {

	for(int c = 0; c < trainData.cols; c++) {
		cv::Scalar mean, std;

		cv::meanStdDev(trainData.col(c),mean,std);

		_mean.append(mean[0]);
		_stdDeviaton.append(std[0]);
	}
}

/**
 * @brief Concat two datasets. Multiple smallest one to be equal as biggest one.
 * @param src1dst
 * @param src2
 */
void ScoreNormalizer::concatAsComparable(cv::Mat &src1dst, cv::Mat &src2) {

	//which one is bigger ?
	cv::Mat big, small;
	if(src1dst.rows > src2.rows) {
		big = src1dst;
		small = src2;
	} else {
		big = src2;
		small = src1dst;
	}

	int diff = big.rows / small.rows;
	cv::Mat tmp = small;
	for(int i = 0; i < diff; i++) {
		cv::vconcat(small,tmp,small);
	}

	//contac big and small
	cv::vconcat(big,small,src1dst);



}
/**
 * @brief Normaliza score with zscore method.
 * @param inputScore Input score
 * @param normScore Normalize score
 */
void ScoreNormalizer::normalizeZScore(QVector<float> &inputScore, QVector<float> &normScore) {

	normScore.clear();

	for(int i = 0; i < inputScore.size(); i++) {

			double value = ( inputScore.at(i) - _mean.at(i) ) / _stdDeviaton.at(i);
			normScore.append(value);
	}
}

/**
 * @brief Save normalization parameters to file.
 * @param fileName Name of file
 * @param dirPath Directory with file
 */
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

/**
 * @brief Load normalization parameters from file.
 * @param fileName Name of file
 * @param dirPath Directory with file
 */
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

