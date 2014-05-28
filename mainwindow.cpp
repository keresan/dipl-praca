#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent) {

	//Users/martin/Documents/[]sklad/frgc_data/created_depthmap/

	QString rootDirectory = "/Users/martin/Documents/[]sklad/frgc_data/POKUS";
	QString modelDirectory = "/Volumes/data/sklad/FRGC_databaza/Spring2003range";

	Run testToRun(rootDirectory, modelDirectory, this);

	this->setCentralWidget(testToRun.window);

	//testToRun.test_alignFace();
	//testToRun.test_alignFace2();
	//testToRun.test_selectGrid();
	//testToRun.test_crop();
	//testToRun.test_show();
	//testToRun.test_depthMapMapping();
	//testToRun.test_showDepthMap();
	//testToRun.test_depth_select();

	/*
	QString pathToMarkedLandmarks = "/Users/martin/Documents/[]sklad/frgc_data/faces_marked";
	QString pathToFirstFace = pathToMarkedLandmarks+"/02463d550.abs_landmarks.txt";
	testToRun.createAverageFace(pathToMarkedLandmarks,pathToFirstFace);

	testToRun.normalizeAverageFace();
	*/

	//testToRun.test_loadDeptmap();
	//testToRun.test_showLandmarks();
	//testToRun.test_eigen();
	//testToRun.test_divideFace();
	//testToRun.test_processFace();

	//testToRun.createDepthmaps();

	testToRun.initPCA();

	testToRun.compareFacesInit();

	testToRun.compareFaces();

	testToRun.showResults();

}

MainWindow::~MainWindow() {

}
