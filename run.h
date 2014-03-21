#ifndef RUN_H
#define RUN_H

#include "averageface.h"
#include "facealigner.h"
#include "depthmap.h"
#include "glwidget.h"
#include <QTime>

/**
 * @brief The Run class - run batch of process
 */
class Run {
public:
    Run(QMainWindow *parent = 0);

    void test1();

    void test_show();
    void test_alignFace();

    void test_depth();
    void test_depth2();
    void test_depth3();
    void test_depth_select();



    GLWidget *window;
    QMainWindow *parent;
};



#endif // RUN_H
