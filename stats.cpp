#include "stats.h"

Stats::Stats(cv::Mat imposterResult, cv::Mat genuineResult) {

	if(imposterResult.cols != genuineResult.cols) {
		throw std::runtime_error("Stats() input matrices mishmash");
	}

	_impResult = imposterResult;
	_genResult = genuineResult;

	statValuesVector.fill(StatValues(),_impResult.cols);

	qDebug() << "statValuesVector.size():" << statValuesVector.size();

}

void Stats::computeMinMaxAvg() {

	for(int i = 0; i < statValuesVector.size(); i++) {

		cv::Mat concat;
		cv::vconcat(_genResult.col(i), _impResult.col(i), concat);

		double min,max,mean;
		mean = cv::mean(concat)[0];

		cv::minMaxLoc(concat,&min, &max);

		statValuesVector[i].max = max;
		statValuesVector[i].min = min;
		statValuesVector[i].mean = mean;
	}
}

void Stats::computeHistogram(bool normAsPercentage) {

	for(int c = 0; c < _impResult.cols; c++) {

		cv::Mat inputImposer = _impResult.col(0);
		cv::Mat inputGenuine = _genResult.col(0);
		cv::MatND histImposer, histGenuine;
		int dims = 1;
		int channels[] = {0};

		int histSize = 100;
		float hranges[] = {-13,13};
		const float* ranges[] = {hranges};

		cv::calcHist(&inputImposer,1,channels,cv::Mat(),histImposer,dims,&histSize,ranges,true,false);
		cv::calcHist(&inputGenuine,1,channels,cv::Mat(),histGenuine,dims,&histSize,ranges,true,false);

		//normalize histogram
		if(normAsPercentage) {
			for(int i = 0; i < histImposer.rows; i++) {
				histImposer.at<float>(i) = histImposer.at<float>(i) / inputImposer.rows;
				histGenuine.at<float>(i) = histGenuine.at<float>(i) / inputGenuine.rows;
			}
		}
	}
}

void Stats::printHist(int col) {

	if(col <0 || col >= statValuesVector.size()) {
		throw std::runtime_error("printHist(): col out-of-range");
	}


	for(int r = 0; r < statValuesVector.at(col).histGenuine.rows; r++) {
		//qDebug("%.4f\t%.4f", histGenuine.at<float>(i), histImposer.at<float>(i));

		QString number1, number2;

		convertDotInNumber(statValuesVector.at(col).histGenuine.at<float>(r),number1);
		convertDotInNumber(statValuesVector.at(col).histImposer.at<float>(r),number2);

		qDebug("%s\t%s", number1.toStdString().c_str(), number2.toStdString().c_str());
	}
}

void Stats::convertDotInNumber(float number, QString &strNumber, int precision) {

	strNumber = QString::number(number,'f',precision);
	strNumber.replace(".",",");

}


void Stats::computeEer() {

	computeMinMaxAvg();

	for(int i = 0; i < statValuesVector.size(); i++) {

		float minDistance = 100;
		float step = qAbs(statValuesVector.at(i).max - statValuesVector.at(i).min) / 2000;

		for(float threshold = statValuesVector.at(i).min; threshold < statValuesVector.at(i).max; threshold += step) {

			float fmr, fnmr;
			computeFmrFnrm(i, fmr, fnmr, threshold);

			float actualDistance = qAbs(fmr - fnmr);

			if( actualDistance< minDistance) {
				minDistance = actualDistance;
				statValuesVector[i].eer = (fmr+fnmr)/2;
			}
		}
	}
}

void Stats::computeFmrFnrm(int col, float &fmr, float &fnmr, float threshold) {

	if(col <0 || col >= statValuesVector.size()) {
		throw std::runtime_error("printHist(): col out-of-range");
	}

	int fmrCount = 0;
	int fnmrCount = 0;

	for(int r = 0; r < _genResult.rows; r++) {
		if(_genResult.at<float>(r,col) >= threshold) {
			//match
			fnmrCount++;
		}
	}

	for(int r = 0; r < _impResult.rows; r++) {
		if(_impResult.at<float>(r,col) < threshold) {
			//false match
			fmrCount++;
		}
	}

	fmr = fmrCount / (float)_impResult.rows;
	fnmr = fnmrCount / (float)_genResult.rows;
}


void Stats::saveEer(QString fileName, QString dirPath) {
	QDir dir(dirPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::WRITE);

	storage << Common::vectorSizeLabel.toStdString() << statValuesVector.size();

	for(int i =0; i < statValuesVector.size(); i++) {
		storage  << Common::scnColumnLabel.toStdString()+std::to_string(i) << "{";

		//storage  << Common::scnMeanLabel.toStdString() << statValuesVector.at(i).mean;
		//storage  << Common::scnMaxLabel.toStdString() << statValuesVector.at(i).min;
		//storage  << Common::scnMinLabel.toStdString() << statValuesVector.at(i).max;
		storage  << Common::statErrLabel.toStdString() << statValuesVector.at(i).eer;

		//storage << Common::statHistGenuineLabel.toStdString() << statValuesVector.at(i).histGenuine;
		//storage << Common::statHistImposterLabel.toStdString() << statValuesVector.at(i).histImposer;

		storage << "}";
	}

	storage.release();
}
void Stats::loadEer(QVector<float> &errVector, QString fileName, QString dirPath) {

	QDir dir(dirPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::READ);

	int size;
	storage[Common::vectorSizeLabel.toStdString()] >> size;

	errVector.clear();

	for(int i =0; i < size; i++) {
		float tmp;
		storage[Common::scnColumnLabel.toStdString()+std::to_string(i)][Common::statErrLabel.toStdString()] >> tmp;
		errVector.append(tmp);
	}
	storage.release();
}
