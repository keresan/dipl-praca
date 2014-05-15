#include "eigenface.h"

EigenFace::EigenFace() {

}

void EigenFace::pcaTransformation(QVector<cv::Mat> &images, QStringList &labels) {

	QVector<cv::Mat> testVector;
	testVector.append(images[images.size() - 1]);
	cv::Mat testImage = toRowMatrix(testVector, CV_32FC1);
	images.pop_back();


	cv::Mat data = toRowMatrix(images, CV_32FC1);
	cv::PCA pca(data, cv::noArray(), CV_PCA_DATA_AS_ROW);

	//cv::Mat meanFromPca = pca.mean.clone();
	//cv::Mat eigenvectors = pca.eigenvectors.clone();

	cv::imshow("original", Common::norm_0_255(testImage.reshape(1, images[0].rows)));
	cv::Mat components,backProjection;
	components = pca.project(testImage);

	backProjection = pca.backProject(components);

	cv::imshow("back projection", Common::norm_0_255(backProjection.reshape(1, images[0].rows)));

	/*
	cv::imshow("meanFromPca", Common::norm_0_255(meanFromPca.reshape(1, images[0].rows)));

	for(int i = 0; i < images.count(); i++) {
		cv::Mat grayscale = Common::norm_0_255(eigenvectors.row(i).reshape(1, images[0].rows));
		imshow(labels.at(i).toStdString(), grayscale);
	}
	*/

	//compute distance
	/*
	qDebug() << "max distance: " << cv::norm(eigenvectors.row(0), eigenvectors.row(1), cv::NORM_INF);
	qDebug() << "manhattan distance: " << cv::norm(eigenvectors.row(0), eigenvectors.row(1), cv::NORM_L1);
	qDebug() << "euclidian distance: " << cv::norm(eigenvectors.row(0), eigenvectors.row(1), cv::NORM_L2);

	qDebug() << "L2:" << cv::norm(eigenvectors.row(0), cv::NORM_L2);
	qDebug() << "L2:" << cv::norm(eigenvectors.row(1), cv::NORM_L2);
	*/

}

void EigenFace::pcaTransformation_eigen(QVector<cv::Mat> &images, QStringList &labels) {
	std::vector<cv::Mat> imagesVector;
	std::vector<int> labelsVector;

	for(int i=0; i < images.count(); i++) {
		imagesVector.push_back(images[i]);
		QString editPath(labels[i]);
		editPath.chop(4); //remove .abs
		labelsVector.push_back(editPath.right(3).toInt());
	}

	cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
	model->train(imagesVector, labelsVector);

	cv::Mat eigenvectors = model->getMat("eigenvectors");
	cv::Mat mean = model->getMat("mean");

	cv::imshow("mean",  Common::norm_0_255(mean.reshape(1, imagesVector[0].rows)));

	for(int i = 0; i < eigenvectors.cols; i++) {
		cv::Mat ev = eigenvectors.col(i).clone();
		// Reshape to original size & normalize to [0...255] for imshow.
		cv::Mat grayscale = Common::norm_0_255(ev.reshape(1, imagesVector[0].rows));

		cv::imshow(cv::format("%d",labelsVector[i]),grayscale);

	   // cv::waitKey(0);
	}

}




cv::Mat EigenFace::toRowMatrix(QVector<cv::Mat> &src, int rtype, double alpha, double beta) {

	int n = src.count();
	if(n==0) {
		return cv::Mat();
	}

	// dimensionality of (reshaped) samples
	size_t d = src[0].total();
	// Create resulting data matrix:
	cv::Mat data(n, d, rtype);

	//copy data
	for(int i=0; i < n; i++) {
		//check image
		if(src[0].empty()) {
			assert(false);
		}
		//check, if data can be reshaped
		if(src[i].total() != d) {
			//image has different dimensionality
			assert(false);
		}

		cv::Mat xi = data.row(i);

		if(src[i].isContinuous()) {
			src[i].reshape(1,1).convertTo(xi,rtype,alpha,beta);
		} else {
			src[i].clone().reshape(1,1).convertTo(xi,rtype,alpha,beta);
		}
	}
	return data;
}

void EigenFace::toRowMatrix(cv::Mat &src, cv::Mat &dst, int rtype, double alpha, double beta) {
	src.reshape(1,1).convertTo(dst,rtype,alpha,beta);
}

void EigenFace::train(VectorOfDivideAreas &vector, int numberOfComponents) {

	VectorOfDivideAreas vectorOfAreas;

	convertToVectorOfAreas(vector,vectorOfAreas);

	for(int i = 0; i < vectorOfAreas.size(); i++) {
			cv::Mat matrix = toRowMatrix(vectorOfAreas[i],CV_32F);
			cv::PCA pca(matrix, cv::noArray(),CV_PCA_DATA_AS_ROW, numberOfComponents);
			_pcaSubspaces.append(pca);

			//show mean face
			//cv::Mat meanFromPca = _pcaSubspaces.at(i).mean.clone();
			//qDebug() << meanFromPca.rows << "x" << meanFromPca.cols;
			//imshow(QString::number(i).toStdString(), Common::norm_0_255(meanFromPca.reshape(1, vectorOfAreas[i][0].rows)));
	}

	//qDebug() << "pcaArray size:" << _pcaArray.size();

}

void EigenFace::convertToVectorOfAreas(VectorOfDivideFaces &src, VectorOfDivideAreas &dst) {

	dst.fill(QVector<cv::Mat>(),src.first().size());

	for(int i = 0; i < src.size(); i++) {

		for(int j = 0; j < src.first().size(); j++) {
			dst[j].append(src.at(i)[j]);
		}
	}
	qDebug() << "dst size: " << dst.size() << "x" << dst.first().size();
}

void EigenFace::convertToVectorOfFaces(VectorOfDivideAreas &src, VectorOfDivideFaces &dst) {

	dst.fill(QVector<cv::Mat>(),src.first().size());

	for(int i = 0; i < src.size(); i++) {
		for( int j = 0; j < src.first().size(); j++) {
			dst[i].append(src.at(i)[j]);
		}
	}
}


void EigenFace::saveSubspaces(QString fileName, QString savePath) {
	QDir dir(savePath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString()+".xml",
							cv::FileStorage::WRITE);

	storage << Common::vectorSizeLabel.toStdString() << _pcaSubspaces.size();
	for(int i=0 ; i<_pcaSubspaces.size(); i++) {
		storage << Common::eigenEigenvaluesLabel.toStdString()+ std::to_string(i) << _pcaSubspaces[i].eigenvalues;
		storage << Common::eigenEigenvectorLabel.toStdString()+ std::to_string(i) << _pcaSubspaces[i].eigenvectors;
		storage << Common::eigenMeanLabel.toStdString()+ std::to_string(i) << _pcaSubspaces[i].mean;
	}
	storage.release();
}

void EigenFace::loadSubspaces(QString fileName, QString loadPath) {
	QDir dir(loadPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString()+".xml",
							cv::FileStorage::READ);

	int n;
	storage[Common::vectorSizeLabel.toStdString()] >> n;


	for(int i= 0; i < n; i++) {
		cv::Mat eigenvectors, eigenvalues, mean;
		storage[Common::eigenEigenvectorLabel.toStdString()+ std::to_string(i)] >> eigenvectors;
		storage[Common::eigenEigenvaluesLabel.toStdString()+ std::to_string(i)] >> eigenvalues;
		storage[Common::eigenMeanLabel.toStdString()+ std::to_string(i)] >> mean;

		cv::PCA pca;
		pca.eigenvectors = eigenvectors;
		pca.eigenvalues = eigenvalues;
		pca.mean = mean;

		_pcaSubspaces.append(pca);
	}
}

void EigenFace::showMeans() {
	for(unsigned int i = 0; i < 2; i++) {
		cv::Mat meanFromPca = _pcaSubspaces.at(i).mean.clone();
		qDebug() << meanFromPca.rows << "x" << meanFromPca.cols;
	}
}

void EigenFace::project(VectorOfDivideFaces &faces, QVector<cv::Mat> &results) {
	VectorOfDivideAreas vectorOfAreas;

	convertToVectorOfAreas(faces, vectorOfAreas);

	for(int i = 0; i < vectorOfAreas.size(); i++) {
		//array of face areas convert to row matrix
		cv::Mat src = toRowMatrix(vectorOfAreas[i],CV_32F);
		cv::Mat dst;
		_pcaSubspaces.at(i).project(src, dst);

		results.append(dst);
	}
	//convertToVectorOfFaces(resultVector,results);
}

void EigenFace::project(tFaceAreas &areas, tFeatures &features) {

	assert(_pcaSubspaces.size() == areas.size());

	for(int i = 0; i < areas.size(); i++) {
		//array of face areas convert to row matrix
		cv::Mat src;

		toRowMatrix(areas[i],src,CV_32F);
		//qDebug() << areas[i].rows << "x" << areas[i].cols << "->" << src.rows << "x" << src.cols;

		cv::Mat projection;
		_pcaSubspaces.at(i).project(src, projection);

		//qDebug() << "projection:" << projection.rows << "x" << projection.cols;
		features.push_back(projection);
	}
}

void EigenFace::backProject(tFeatures &features, QVector<cv::Mat> &result) {
	assert(_pcaSubspaces.size() == features.rows);

	for(int i = 0; i < features.rows; i++) {
		//array of face areas convert to row matrix
		cv::Mat dst;
		_pcaSubspaces.at(i).backProject(features.row(i), dst);

		//qDebug() << 1 << "x" << features.cols << "->" << dst.rows << "x" << dst.cols;
		result.append(dst);
	}
}
