#include "stats.h"

/**
 * @brief Constructor
 * @param imposterResult Results of imposter users comparation
 * @param genuineResult Results of genuine users comparation
 */
Stats::Stats(cv::Mat imposterResult, cv::Mat genuineResult) {

	if(imposterResult.cols != genuineResult.cols) {
		throw std::runtime_error("Stats() input matrices mishmash");
	}

	_impResult = imposterResult;
	_genResult = genuineResult;

	statValuesVector.fill(StatValues(),_impResult.cols);

	computeMinMaxAvg();

}

/**
 * @brief Compute minimum, maximu and average value for each face area.
 */
void Stats::computeMinMaxAvg() {

	for(int i = 0; i < statValuesVector.size(); i++) {

		cv::Mat concat;
		cv::vconcat(_genResult.col(i), _impResult.col(i), concat);

		double min,max,mean;
		//mean = cv::mean(concat)[0];
		cv::minMaxLoc(concat,&min, &max);

		statValuesVector[i].max = max;
		statValuesVector[i].min = min;

		//qDebug() << min << max;
		//statValuesVector[i].mean = mean;
	}
}

/**
 * @brief Compute min and max value to set range for histogram. This method is overloaded.
 * @param col Id of face area for what compute histogram
 * @param normAsPercentage Normalize values in histogram as percenage of all
 */
void Stats::computeHist(int col, bool normAsPercentage) {

	float min = statValuesVector.at(col).min;
	float max = statValuesVector.at(col).max;


	if(min < 0) {
		min = qCeil(min);
	} else {
		min = qFloor(min);
	}

	if(max < 0) {
		max = qFloor(max);
	} else {
		max = qCeil(max);
	}

	computeHist(col,min,max,normAsPercentage);
}

/**
 * @brief Compute histogram. This method is overloaded.
 * @param col Id of face area for what compute histogram
 * @param min Lower value of histogramr ange
 * @param max Upper value of histogram range
 * @param normAsPercentage
 */
void Stats::computeHist(int col,float min, float max,bool normAsPercentage) {

	if(col <0 || col >= statValuesVector.size()) {
		throw std::runtime_error("computeHist(): col out-of-range");
	}

		cv::Mat inputImposer = _impResult.col(0);
		cv::Mat inputGenuine = _genResult.col(0);
		//cv::MatND histImposer, histGenuine;
		int dims = 1;
		int channels[] = {0};

		int histSize = 100;
		float hranges[] = {min,max};
		const float* ranges[] = {hranges};

		cv::calcHist(&inputImposer,1,channels,cv::Mat(),statValuesVector[col].histImposer,dims,&histSize,ranges,true,false);
		cv::calcHist(&inputGenuine,1,channels,cv::Mat(),statValuesVector[col].histGenuine,dims,&histSize,ranges,true,false);

		//normalize histogram
		if(normAsPercentage) {
			for(int i = 0; i < statValuesVector.at(col).histImposer.rows; i++) {
				statValuesVector[col].histImposer.at<float>(i) = statValuesVector.at(col).histImposer.at<float>(i) / _impResult.rows;
				statValuesVector[col].histGenuine.at<float>(i) = statValuesVector.at(col).histGenuine.at<float>(i) / _genResult.rows;
			}
		}

}

/**
 * @brief Print histogram values for genuine and imposter users
 * @param col Id of face area for what print histogram
 */
void Stats::printHist(int col) {

	if(col <0 || col >= statValuesVector.size()) {
		throw std::runtime_error("printHist(): col out-of-range");
	}

	qDebug() << "rows:" << statValuesVector.at(col).histGenuine.rows;
	for(int r = 0; r < statValuesVector.at(col).histGenuine.rows; r++) {
		//qDebug("%.4f\t%.4f", histGenuine.at<float>(i), histImposer.at<float>(i));

		QString number1, number2;

		convertDotInNumber(statValuesVector.at(col).histGenuine.at<float>(r),number1);
		convertDotInNumber(statValuesVector.at(col).histImposer.at<float>(r),number2);

		qDebug("%s\t%s", number1.toStdString().c_str(), number2.toStdString().c_str());
	}
}

/**
 * @brief Convert decimal dot do decimal comma.
 * @param number Number to convert
 * @param strNumber Output converted number
 * @param precision Precision of converted number
 */
void Stats::convertDotInNumber(float number, QString &strNumber, int precision) {

	strNumber = QString::number(number,'f',precision);
	strNumber.replace(".",",");
}

/**
 * @brief Compute equal error rate for each face area.
 */
void Stats::computeEer() {

	for(int i = 0; i < statValuesVector.size(); i++) {

		float minDistance = 100;
		float step = qAbs(statValuesVector.at(i).max - statValuesVector.at(i).min) / 2000;

		//qDebug() << "min max:" << statValuesVector.at(i).min << statValuesVector.at(i).max;
		//qDebug() << "step:" << step;
		for(float threshold = statValuesVector.at(i).min; threshold < statValuesVector.at(i).max; threshold += step) {

			float fmr, fnmr;
			computeFmrFnrm(i, fmr, fnmr, threshold);

			float actualDistance = qAbs(fmr - fnmr);

			if( actualDistance< minDistance) {
				minDistance = actualDistance;
				statValuesVector[i].eer = (fmr+fnmr)/2;

				//qDebug() << threshold << fmr << fnmr << actualDistance;
			}
		}
	}
}

/**
 * @brief Compute FMR and FNMR for particular treshold
 * @param col Dd of face area
 * @param fmr Output FMR
 * @param fnmr Output FNMR
 * @param threshold Treshold
 */
void Stats::computeFmrFnrm(int col, float &fmr, float &fnmr, float threshold) {

	if(col <0 || col >= statValuesVector.size()) {
		throw std::runtime_error("printHist(): col index out-of-range");
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

/**
 * @brief Print values of FMR and FNMR for tresholds
 * @param col Id of face area for what print FMR and FNMR
 * @param thresholdStart Compute stats from this number
 * @param thresholdStop End computing stats from this number
 * @param step Step of computed stats
 */
void Stats::printGraphFmrFnmr(int col, float thresholdStart, float thresholdStop, float step) {

	for(float t = thresholdStart; t <= thresholdStop; t += step) {
		float fmr, fnmr;
		computeFmrFnrm(col,fmr,fnmr,t);


		QString fmrStr, fnmrStr,tStr;
		convertDotInNumber(fmr,fmrStr);
		convertDotInNumber(fnmr,fnmrStr);
		convertDotInNumber(t,tStr);

		qDebug("%s\t%s\t%s",tStr.toStdString().c_str() ,fmrStr.toStdString().c_str(), fnmrStr.toStdString().c_str());

	}
}

/**
 * @brief Save EER to file
 * @param fileName File name to save
 * @param dirPath Directory to save
 */
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

/**
 * @brief Load EER from file.
 * @param errVector Vector of loaded err.
 * @param fileName File name to load.
 * @param dirPath Directory in witch file is.
 */
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
