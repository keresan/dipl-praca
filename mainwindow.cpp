#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {


    //ui->setupUi(this);

    Run testToRun(this);

    this->setCentralWidget(testToRun.window);

    testToRun.test_alignFace();
   //testToRun.test1();

    //testToRun.test2_show();

    //testToRun.test_depth();
}

MainWindow::~MainWindow() {
    delete ui;
}
