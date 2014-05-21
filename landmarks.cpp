#include "landmarks.h"

Landmarks::Landmarks(VectorOfLandmarks landmarks) {
	this->_landmarks = landmarks;
}

Landmarks::Landmarks(QString fileName, QString loadPath) {
	load(fileName,loadPath);
}

cv::Point Landmarks::pos(Landmarks::LandmarkNames name) {
	return _landmarks[name].first;
}

bool Landmarks::is(Landmarks::LandmarkNames name) {
	return _landmarks[name].second;
}

void Landmarks::set(Landmarks::LandmarkNames name, cv::Point pos) {
	_landmarks[name].first = pos;
	_landmarks[name].second = true;
}

void Landmarks::discard(Landmarks::LandmarkNames name) {
	_landmarks[name].second = false;
}


VectorOfLandmarks Landmarks::getLandmarks() {
	return _landmarks;
}

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

void Landmarks::scale(float scaleFactor) {
	if(scaleFactor <= 0) {
		throw std::runtime_error("scale(): scaleFactor must by positive");
	}

	for(int i = 0; i < _landmarks.size(); i++) {
		_landmarks[i].first.x = qRound( _landmarks.at(i).first.x * scaleFactor);
		_landmarks[i].first.y = qRound( _landmarks.at(i).first.y * scaleFactor);

	}
}
