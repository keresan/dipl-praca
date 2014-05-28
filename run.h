#ifndef RUN_H
#define RUN_H

#include "averageface.h"
#include "facealigner.h"
#include "depthmap.h"
#include "glwidget.h"
#include "eigenface.h"
#include "landmarkdetector.h"
#include "landmarks.h"
#include "facedivider.h"
#include "controller.h"
#include "stats.h"
#include "scorenormalizer.h"
#include "scorefusioner.h"

#include <QTime>


/**
 * @brief Class for running batch of process.
 */
class Run {
public:
	Run(QString rootDir, QString faceModelDir, QMainWindow *parent = 0);

    void test_selectGrid();

    void test_show();
	void test_alignFace();
	void test_alignFace2();

	void test_crop();

	void test_depthMapMapping();

	void test_showDepthMap();

	void createDepthmaps();

	void test_depth_select();

	void createAverageFace(QString pathToMarkedFaces, QString pathToFirstFace);
	void normalizeAverageFace();

	void detectWrongDepthmaps();

	void test_loadDeptmap();

	void test_showLandmarks();
	void test_divideFace();
	void compareFaces();
	void compareFacesInit();

	void initPCA();
	void test_processFace();
	void showResults();

	void compareTwoFaces(QString model1, QString model2);

    GLWidget *window;
    QMainWindow *parent;


private:
	/**
	 * @brief Eigenface method using in app
	 */
	EigenFace _eigenfaceMethod0;

	/**
	 * @brief Divide method using in app
	 */
	FaceDivider::DivideMethod _divideMethod;

	/**
	 * @brief Compare method using in app
	 */
	Comparator::CompareMethod _compareMethod;

	/**
	 * @brief If true, ARENA will be used
	 */
	bool _isArena;
	/**
	 * @brief If true, eigenface will be used
	 */
	bool _isEigenface;

	/**
	 * @brief If true, score will be normalized
	 */
	bool _isNormalize;
};



#endif // RUN_H
