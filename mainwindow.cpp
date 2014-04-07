#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent) {


	Run testToRun(this);

	this->setCentralWidget(testToRun.window);

	// !!! testToRun.createAverageFace();

	//testToRun.normalizeAverageFace();

	//testToRun.test_alignFace();

	//testToRun.test_alignFace2();

	//testToRun.test_selectGrid();
	//testToRun.test_crop();
   //testToRun.test_show();

	//testToRun.depthMapMapping();

	//testToRun.showDepthMap();

	//testToRun.test_depth_select();

	testToRun.createDepthMaps();

	//testToRun.loadImages();

	//testToRun.eigenface();
	//testToRun.eigenface_pca();
}

MainWindow::~MainWindow() {

}
