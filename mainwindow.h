#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "run.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief Need for show up models of face
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
