#include "run.h"

Run::Run(QMainWindow* parent) {

    window = new GLWidget();
    this->parent = parent;
}

void Run::test1() {

    //face 1
    Mesh face1 = Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true);

    Mesh *gridMesh = new Mesh(Mesh::create2dGrid(cv::Point3d(-100,150,0), cv::Point3d(100,150,0),2,2));

    gridMesh->_color = QColor(Qt::green);

    Mesh *newMesh = new Mesh(face1.getExtract2dGrid(*gridMesh));
    //face1 = face1.crop(cv::Point3d(0,0,0),70,70,130,100);

    //newMesh.writeOBJ("/Users/martin/Documents/skola/diplomka/pomocne/02463d548.abs_grid_2.obj",'.');

    window->setWindowTitle("test1");
    window->addFace(gridMesh);
    window->addFace(newMesh);



}

void Run::test_show() {
     Mesh *face1 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));


     window->setWindowTitle("test2_show");
     window->addFace(face1);

}

void Run::test_alignFace() {

    Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));
    Mesh *averageFace = new Mesh(Mesh::fromOBJ("/Users/martin/Documents/[]sklad/frgc_data/averageFace.obj_final.obj",true));

    //face->centralize();
    //averageFace->centralize();

    window->setWindowTitle("test rotate");
    window->addFace(face);
    window->addFace(averageFace);

    face->rotate(0,0,0.5);
    //window->repaint(); // ??
    parent->show();

    averageFace->_color = QColor(Qt::green);

    FaceAligner faceAligner;

    QTime myTimer;
    myTimer.start();

    faceAligner.computeAlign(*face, *averageFace, 500, 100);
    qDebug() << "vypocet zarovnania: "<< myTimer.elapsed() << "ms";
    myTimer.restart();

    faceAligner.alignFaceFast(*face);
    faceAligner.alignFaceFast(*averageFace);
    qDebug() << "zarovnanie: "<< myTimer.elapsed() << "ms";

    parent->centralWidget()->repaint();

    /*
    QTime myTimer;
    myTimer.start();



    tTransformValues* values = FaceAligner::computeAlign(*face, *averageFace, 500, 10);
    int miliSec = myTimer.elapsed();
    qDebug() << "miliSec "<< miliSec;
    Common::delay(2);

    for(int i = values->rotation.count() -1; i >=0; i--) {
        Matrix r = values->rotation.at(i);
        cv::Point3d p = values->translate.at(i);
        r = r.t();
        p = -p;

        AverageFace::transform(face->pointsMat, r);
        AverageFace::translate(face->pointsMat,p);
        AverageFace::transform(averageFace->pointsMat, r);
        AverageFace::translate(averageFace->pointsMat,p);

        //window->repaint();
        parent->centralWidget()->repaint();
    }
    */
}


