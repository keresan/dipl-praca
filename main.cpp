#include "mainwindow.h"

#include "averageface.h"
#include "facealigner.h"

#include <QApplication>

#include "glwidget.h"
#include <QTime>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //face 1
   // Mesh face1 = Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true);
//    Mesh face1 = Mesh::fromABS("/Users/martin/Documents/sklad/frgc_data/Fall2003range/02463d548.abs","/Users/martin/Documents/sklad/frgc_data/Fall2003range/02463d549.ppm", true);

    //face2
    //Mesh face2 = Mesh::fromABS("/Users/martin/Documents/sklad/frgc_data/Fall2003range/02463d546.abs", true);
    //Mesh face1 = Mesh::fromABS("/Users/martin/Documents/sklad/frgc_data/Fall2003range/02463d546.abs","/Users/martin/Documents/sklad/frgc_data/Fall2003range/02463d547.ppm", true);

    //Mesh averageFace = Mesh::fromOBJ("/Users/martin/Documents/sklad/frgc_data/averageFace.obj_final.obj",true);


    //Mesh face1 = Mesh::fromABS("/Users/martin/Documents/sklad/frgc_data/Fall2003range/02463d550.abs", true);

    //Mesh face1 = Mesh::fromABS("/Users/martin/Documents/sklad/frgc_data/Fall2003range/04221d431.abs", true);
    //Mesh face2 = Mesh::fromABS("/Users/martin/Documents/sklad/frgc_data/Fall2003range/04202d438.abs", true);

    //cv::Point3d mean = face1.getMeanPoint();
    //face1 = face1.crop(mean, 70,70,100,100);
    //face1.centralize();


    //Mesh gridMesh = Mesh::create2DGrid(cv::Point3d(-50,90,0), cv::Point3d(50,-60,0),2,2);

    //Mesh newMesh = face1.getNearest2dPoints(gridMesh, 5);

    //qDebug() << "expectedPoints: " << gridMesh.pointsMat.rows;
    //qDebug() << "new mesh: " << newMesh.pointsMat.rows;

    //Mesh gridMesh = Mesh::create2dGrid(cv::Point3d(-70,130,0), cv::Point3d(70,100,0),2,2);

    //Mesh newMesh = face1.getExtract2dGrid(gridMesh);
    //face1 = face1.crop(cv::Point3d(0,0,0),70,70,130,100);

    //newMesh.writeOBJ("/Users/martin/Documents/skola/diplomka/pomocne/02463d548.abs_grid_2.obj",'.');

    //GLWidget window;
    //window.setWindowTitle("pokus1");
    //window.addFace(&gridMesh);

    //window.addFace(&newMesh);
    //window.show();


    //Mesh newMesh;
    //AverageFace model("/Users/martin/Documents/sklad/frgc_data/Fall2003range/","02463d550.abs_landmarks-start.txt");
    //model.process("/Users/martin/Documents/sklad/frgc_data/averageFace_2.obj");


    /*
    averageFace._color = QColor(Qt::green);

    face1.rotate(0,0,0.5);
    face1._color = QColor(Qt::yellow);
    face2._color = QColor(Qt::blue);

    GLWidget window;

    window.addFace(&face1);
    //window.addFace(&face2);
    window.addFace(&averageFace);
    window.show();

    averageFace.printStats();

    QTime myTimer;
    myTimer.start();
    */

    /*
    for(int i = 0; i < 30; i++) {
        qDebug() << i;
        Common::delay(2);
        FaceAligner::align(face1, averageFace, 5);
        window.repaint();
    }
    */



    /*
    transformValues* values = FaceAligner::align(face1, averageFace, 500, 50);
    int miliSec = myTimer.elapsed();
    qDebug() << "miliSec "<< miliSec;
    Common::delay(2);

    for(int i = values->rotation.count() -1; i >=0; i--) {
        Matrix r = values->rotation.at(i);
        cv::Point3d p = values->translate.at(i);
        r = r.t();
        p = -p;

        AverageFace::transform(face1.pointsMat, r);
        AverageFace::translate(face1.pointsMat,p);
        AverageFace::transform(averageFace.pointsMat, r);
        AverageFace::translate(averageFace.pointsMat,p);

        window.repaint();
    }
    */


    //face1 = face1.radiusSelect(30);
    //window.repaint();


    //qDebug() << "after rotation:";
    //face1.printStats();
   // window.repaint();

    //Common::printMatrix(inverseMatrix);

    MainWindow w;
    w.show();

    return a.exec();
}
