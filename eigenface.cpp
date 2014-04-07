#include "eigenface.h"

EigenFace::EigenFace() {

}

void EigenFace::pcaTransformation(QVector<cv::Mat> &images, QStringList &labels) {

	cv::Mat data = toRowMatrix(images, CV_32FC1);
	cv::PCA pca(data, cv::noArray(), CV_PCA_DATA_AS_ROW);

	cv::Mat meanFromPca = pca.mean.clone();
	cv::Mat eigenvectors = pca.eigenvectors.clone();

	cv::imshow("meanFromPca", EigenFace::norm_0_255(meanFromPca.reshape(1, images[0].rows)));

	for(int i = 0; i < images.count(); i++) {
		cv::Mat grayscale = EigenFace::norm_0_255(eigenvectors.row(i).reshape(1, images[0].rows));
		imshow(labels.at(i).toStdString(), grayscale);
	}

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

	cv::imshow("mean",  norm_0_255(mean.reshape(1, imagesVector[0].rows)));

	for(int i = 0; i < eigenvectors.cols; i++) {
		cv::Mat ev = eigenvectors.col(i).clone();
		// Reshape to original size & normalize to [0...255] for imshow.
		cv::Mat grayscale = norm_0_255(ev.reshape(1, imagesVector[0].rows));

		cv::imshow(cv::format("%d",labelsVector[i]),grayscale);

	   // cv::waitKey(0);
	}

}


cv::Mat EigenFace::norm_0_255(cv::InputArray _src) {
	cv::Mat src = _src.getMat();
	// Create and return normalized image:
	cv::Mat dst;
	switch(src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
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
