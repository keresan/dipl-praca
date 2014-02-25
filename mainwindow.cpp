#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);

    Run testToRun;



    this->setCentralWidget(testToRun.window);

    testToRun.test1();


}

MainWindow::~MainWindow() {
    delete ui;
}
