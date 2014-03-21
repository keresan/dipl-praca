#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
   {


    Run testToRun(this);

    this->setCentralWidget(testToRun.window);

    //testToRun.test_alignFace();
   //testToRun.test1();

   // testToRun.test_show();

   // testToRun.test_depth2();
   testToRun.test_depth3();
    //testToRun.test_depth_select();

}

MainWindow::~MainWindow() {

}
