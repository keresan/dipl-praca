#ifndef EIGENFACE_H
#define EIGENFACE_H

#include "common.h"

class EigenFace {
public:
	EigenFace();

	void pcaTransformation(QVector<cv::Mat> &images, QStringList &labels);

	void pcaTransformation_eigen(QVector<cv::Mat> &images, QStringList &labels);

	static cv::Mat toRowMatrix(QVector<cv::Mat> &src, int rtype, double alpha = 1, double beta = 0);

	//static cv::Mat averageImg(cv::Mat &src, cv::Mat & dst, double dstWeight);

	cv::PCA pca;

	static cv::Mat norm_0_255(cv::InputArray _src);
};


#endif // EIGENFACE_H
