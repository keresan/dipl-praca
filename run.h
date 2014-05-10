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

#include <QTime>

/**
 * @brief The Run class - run batch of process
 */
class Run {
public:
    Run(QMainWindow *parent = 0);

    void test_selectGrid();

    void test_show();
	void alignFace();
	void test_alignFace2();

	void test_crop();

    void test_depth();
	void depthMapMapping();

	void showDepthMap();

	void createDepthMaps();

	void test_depth_select();

	void createAverageFace();
	void normalizeAverageFace();

	//void eigenface();
	//void eigenface_pca();

	void loadDeptmap();

	void showLandmarks();

	void divideFace();


	void compareFaces();

	void init();

	void processFace();


    GLWidget *window;
    QMainWindow *parent;
};



#endif // RUN_H
