#include "eigenface.h"

EigenFace::EigenFace() {

}


/**
 * @brief Convert vector of matrix to one mat by rows
 * @param src Vector of matrices
 * @param rtype Type of converted matrix
 * @param alpha Alpha
 * @param beta Beta
 * @return Converted matrix
 */
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

/**
 * @brief Reshape matrix to row
 * @param src Source matrix
 * @param rtype Type of converted matrix
 * @param alpha Alpha
 * @param beta Beta
 * @return Converted matrix
 */
void EigenFace::toRowMatrix(cv::Mat &src, cv::Mat &dst, int rtype, double alpha, double beta) {

	if(src.isContinuous()) {
		src.reshape(1,1).convertTo(dst,rtype,alpha,beta);
	} else {
		src.clone().reshape(1,1).convertTo(dst,rtype,alpha,beta);
	}
}

/**
 * @brief Create PCA subspaces for each face are
 * @param vector Vector of face areas
 * @param numberOfComponents Number of component to create PCA subspace
 */
void EigenFace::train(VectorOfDivideAreas &vector, int numberOfComponents) {

	VectorOfDivideAreas vectorOfAreas;

	convertToVectorOfAreas(vector,vectorOfAreas);

	for(int i = 0; i < vectorOfAreas.size(); i++) {
			cv::Mat matrix = toRowMatrix(vectorOfAreas[i],CV_32F);

			cv::PCA pca(matrix, cv::noArray(),CV_PCA_DATA_AS_ROW, numberOfComponents);
			_pcaSubspaces.append(pca);
	}
}

/**
 * @brief Convert from vector of areas of face to vector of divide areas of face
 * @param src Source vector
 * @param dst Destination vector
 */
void EigenFace::convertToVectorOfAreas(VectorOfDivideFaces &src, VectorOfDivideAreas &dst) {

	dst.fill(QVector<cv::Mat>(),src.first().size());

	for(int i = 0; i < src.size(); i++) {

		for(int j = 0; j < src.first().size(); j++) {
			dst[j].append(src.at(i)[j]);
		}
	}
	//qDebug() << "dst size: " << dst.size() << "x" << dst.first().size();
}

/**
 * @brief Convert from vector of divide areas of face to vector of areas of face
 * @param src Source vector
 * @param dst Destination vector
 */
void EigenFace::convertToVectorOfFaces(VectorOfDivideAreas &src, VectorOfDivideFaces &dst) {

	dst.fill(QVector<cv::Mat>(),src.first().size());

	for(int i = 0; i < src.size(); i++) {
		for( int j = 0; j < src.first().size(); j++) {
			dst[i].append(src.at(i)[j]);
		}
	}
}

/**
 * @brief Save PCA subspaces to file
 * @param fileName File to save
 * @param savePath Directory with file
 */
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

/**
 * @brief Load PCA subspaces from file
 * @param fileName File to load
 * @param loadPath Directory with file
 */
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

/**
 * @brief Show mean vector for each face area.
 */
void EigenFace::showMeans() {
	for(unsigned int i = 0; i < 2; i++) {
		cv::Mat meanFromPca = _pcaSubspaces.at(i).mean.clone();
		qDebug() << meanFromPca.rows << "x" << meanFromPca.cols;
	}
}

/**
 * @brief Project vector of face areas to PCA subspaces
 * @param faces Face areas to project
 * @param results Vector of features
 */
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

/**
 * @brief Project one face to pca subspace
 * @param areas Face areas to convert
 * @param features Vector of features
 */
void EigenFace::project(tFaceAreas &areas, tFeatures &features) {

	if(_pcaSubspaces.size() != areas.size())  {
		throw std::runtime_error("EigenFace::project(): wrong pca subspace");
	}


	for(int i = 0; i < areas.size(); i++) {
		//array of face areas convert to row matrix
		cv::Mat src;

		toRowMatrix(areas[i],src,CV_32F);

		//project vector of face are to vector of subspaces
		cv::Mat projection;
		_pcaSubspaces.at(i).project(src, projection);

		features.push_back(projection);
	}
}

/**
 * @brief Back projection from PCA subspace
 * @param features Vector of features
 * @param result Back projection
 */
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

