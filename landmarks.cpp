#include "landmarks.h"

/**
 * @brief Constructor. Initialize by vector of landmarks
 * @param landmarks Vector of landmarks
 */
Landmarks::Landmarks(VectorOfLandmarks landmarks) {
	this->_landmarks = landmarks;
}

/**
 * @brief Constructor. Initialized by values from file
 * @param fileName File with Vector of landmarks.
 * @param loadPath Directory with file.
 */
Landmarks::Landmarks(QString fileName, QString loadPath) {
	load(fileName,loadPath);
}

/**
 * @brief Get position of landmark
 * @param name Landmark name
 * @return Position
 */
cv::Point Landmarks::pos(Landmarks::LandmarkNames name) {
	return _landmarks[name].first;
}

/**
 * @brief Check if landmarh has been set.
 * @param name Landmark name
 * @return Dicision
 */
bool Landmarks::is(Landmarks::LandmarkNames name) {
	return _landmarks[name].second;
}

/**
 * @brief Set position of landmark
 * @param name Landmark name
 * @param pos Landmark position
 */
void Landmarks::set(Landmarks::LandmarkNames name, cv::Point pos) {
	_landmarks[name].first = pos;
	_landmarks[name].second = true;
}

/**
 * @brief Unset landmark position
 * @param name Landmark name
 */
void Landmarks::discard(Landmarks::LandmarkNames name) {
	_landmarks[name].second = false;
}


/**
 * @brief Get vector of landmarks
 * @return Vector of landmarks
 */
VectorOfLandmarks Landmarks::getLandmarks() {
	return _landmarks;
}

/**
 * @brief Save landmarks to file
 * @param fileName File to save
 * @param savePath Directory with file
 */
void Landmarks::save(QString fileName, QString savePath) {
	QDir dir(savePath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::WRITE);

	storage << Common::vectorSizeLabel.toStdString() << _landmarks.size();

	for(int i =0; i < _landmarks.size(); i++) {
		storage << Common::lmSavePosLabel.toStdString()+std::to_string(i) << _landmarks.at(i).first;
		storage << Common::lmSaveIsLabel.toStdString()+std::to_string(i) << _landmarks.at(i).second;
	}
	storage.release();
}

/**
 * @brief Load landmarks to file
 * @param fileName File to load
 * @param loadPath Directory with file
 */
void Landmarks::load(QString fileName, QString loadPath) {
	QDir dir(loadPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::READ);

	int size;
	storage[Common::vectorSizeLabel.toStdString()] >> size;

	for(int i = 0; i < size; i++) {
		cv::Point point;
		bool isEntered;
		storage[Common::lmSavePosLabel.toStdString()+std::to_string(i)] >> point;
		storage[Common::lmSaveIsLabel.toStdString()+std::to_string(i)] >> isEntered;
		_landmarks.append(qMakePair(point,isEntered));
	}

	storage.release();
}

/**
 * @brief Multiple landmarks position by value
 * @param scaleFactor Value to mulitple
 */
void Landmarks::scale(float scaleFactor) {
	if(scaleFactor <= 0) {
		throw std::runtime_error("scale(): scaleFactor must by positive");
	}

	for(int i = 0; i < _landmarks.size(); i++) {
		_landmarks[i].first.x = qRound( _landmarks.at(i).first.x * scaleFactor);
		_landmarks[i].first.y = qRound( _landmarks.at(i).first.y * scaleFactor);

	}
}
