#ifndef EIGENFACE_H
#define EIGENFACE_H

#include "common.h"

class EigenFace {
public:
	EigenFace();

	void pcaTransformation();

	cv::PCA pca;

	static cv::Mat norm_0_255(cv::InputArray _src);
};


#endif // EIGENFACE_H
