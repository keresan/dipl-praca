#ifndef EIGENFACE_H
#define EIGENFACE_H

#include "common.h"
#include "facedivider.h"

/**
 * syntakticky je to to iste, ale samantika je ina !
 * VectorOfFaces - vektor tvari, kde kazda tvar sa sklada z jednotlivych oblasti
 * VectorOfAreas - vektor vektorov oblasti, kde v danom vektore je rovnaka oblast tvare)
 */
typedef QVector<tFaceAreas> VectorOfDivideFaces;
typedef QVector< QVector<cv::Mat> > VectorOfDivideAreas;

/**
 * @brief On each row of matrix is one feature vector of area
 */
typedef cv::Mat tFeatures;

/**
 * @brief Class for Eigenface method
 */
class EigenFace {
public:
	EigenFace();

	static cv::Mat toRowMatrix(QVector<cv::Mat> &src, int rtype, double alpha = 1, double beta = 0);
	static void toRowMatrix(cv::Mat &src, cv::Mat &dst, int rtype, double alpha = 1, double beta = 0);

	void train(VectorOfDivideAreas &vector, int numberOfComponents = 0);

	void saveSubspaces(QString fileName, QString savePath = Common::pathToSubspacesDir);
	void loadSubspaces(QString fileName, QString loadPath = Common::pathToSubspacesDir);

	void showMeans();

	void project(VectorOfDivideFaces &faces, QVector<cv::Mat> &results);
	void project(tFaceAreas &areas, tFeatures &features);

	void backProject(tFeatures &features, QVector<cv::Mat> &result);

private:
	void convertToVectorOfAreas(VectorOfDivideFaces &src, VectorOfDivideAreas &dst);
	void convertToVectorOfFaces(VectorOfDivideAreas &src, VectorOfDivideFaces &dst);

	/**
	 * @brief PCA subspace for each face area
	 */
	QVector<cv::PCA> _pcaSubspaces;

};


#endif // EIGENFACE_H
