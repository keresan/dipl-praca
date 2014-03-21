#include "run.h"

Run::Run(QMainWindow* parent) {

    window = new GLWidget();
    this->parent = parent;
}

void Run::test1() {

    //face 1
    Mesh face1 = Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true);

    qDebug() << "face1:";
    qDebug() << "pocet bodov:" << face1.pointsMat.rows;
    qDebug() << "pocet trojuholnikov:" << face1.triangles.count();

    Mesh *gridMesh = new Mesh(Mesh::create2dGrid(cv::Point3d(-100,150,0), cv::Point3d(100,150,0),2,2));

    gridMesh->_color = QColor(Qt::green);

    Mesh *newMesh = new Mesh(face1.getExtract2dGrid(*gridMesh));

    qDebug() << "newMesh:";
    qDebug() << "pocet bodov:" << newMesh->pointsMat.rows;
    qDebug() << "pocet trojuholnikov:" << newMesh->triangles.count();

    //face1 = face1.crop(cv::Point3d(0,0,0),70,70,130,100);

    //newMesh.writeOBJ("/Users/martin/Documents/skola/diplomka/pomocne/02463d548.abs_grid_2.obj",'.');

    window->setWindowTitle("test1");
    window->addFace(gridMesh);
    window->addFace(newMesh);



}

void Run::test_show() {
     Mesh *face1 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true));


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

    face->rotate(0,0,0.0);
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

void Run::test_depth() {
    QTime myTimer;
    myTimer.start();
    Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));
    qDebug() << "nacitanie tvare: "<< myTimer.elapsed() << "ms";
    myTimer.restart();

    Mesh *gridMesh = new Mesh(Mesh::create2dGrid(cv::Point3d(-100,150,-50), cv::Point3d(100,150,-50),5,5));
    gridMesh->_color =  QColor(Qt::green);

    qDebug() << "vytvorenie gridu: "<< myTimer.elapsed() << "ms";
    myTimer.restart();

    Mesh *newMesh = new Mesh();
    face->getExtract2dGrid_2(*gridMesh,*newMesh);
    qDebug() << "extrahovanie tvare: "<< myTimer.elapsed() << "ms";
    qDebug() << "pocet bodov: " << newMesh->pointsMat.rows;
    myTimer.restart();

    //window->addFace(face);
    //window->addFace(gridMesh);
    window->addFace(newMesh);
}

void Run::test_depth2() {
    Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));
    DepthMap map;

    /*
    QTime myTimer;
    myTimer.start();
    map.createDepthMap(*face);
    qDebug() << "vytvorenie depth mapy: "<< myTimer.elapsed() << "ms";
    */

    //Common::printMatrix(map.depthMap);
    map.printPoints();


    for(int r = 0; r < map.depthMap.rows ; r++) {
         for(int c = 0; c < map.depthMap.cols; c++) {

             printf("%d,%d -> ",r,c);
             cv::Point2f queryPoint;
             int row, col;
             if (map.mapIndeciesToPoint(r,c,queryPoint)) {
                 printf("[%5.1f %5.1f] -> ",queryPoint.x, queryPoint.y);
                 if(map.mapPointToIndecies(queryPoint,row, col)) {
                     printf("%d,%d\n", row, col);
                 } else {
                     printf("unknown\n");
                 }
             } else {
                 printf("unkwnown -> ");
             }

         }

    }


   // map.averageValueInTraignle(cv::Point2f(0,0),*face);

}
void Run::test_depth_select() {
    Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));
    DepthMap map;



    QVector<cv::Point2f> vector;

    qDebug() << "jedem";

    map.selectFromDepthMap(cv::Point2d(-38.043,38.813), cv::Point2d(-35.781,43.810), vector);

    qDebug() << "selected points: ";
    for(int i = 0; i < vector.count(); i++) {
        qDebug() << vector.at(i).x << vector.at(i).y;
    }


   // map.printPoints();

}

void Run::test_depth3() {
    Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));
    DepthMap map;

    //map.averageValueInTraignle(cv::Point2f(3,3),*face);

    qDebug() << "jedem:";
    QTime myTimer;
    myTimer.start();

    map.test1(*face);
    qDebug() << "vytvorenie depth mapy: "<< myTimer.elapsed() << "ms";
}
