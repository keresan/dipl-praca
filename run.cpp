#include "run.h"

Run::Run() {

    window = new GLWidget();
}

void Run::test1() {

    //face 1
    Mesh face1 = Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true);

    Mesh gridMesh = Mesh::create2dGrid(cv::Point3d(-70,130,0), cv::Point3d(70,100,0),2,2);

    //celee napicu !!! musi vracat odkaz, nie celu mesh ! ale zase potom vznika problem, kto novu maticu deletne.

    Mesh *newMesh = new Mesh();
    face1.getExtract2dGrid(gridMesh,*newMesh);
    //face1 = face1.crop(cv::Point3d(0,0,0),70,70,130,100);

    //newMesh.writeOBJ("/Users/martin/Documents/skola/diplomka/pomocne/02463d548.abs_grid_2.obj",'.');


    window->setWindowTitle("pokus1");

    qDebug() << window->sizeHint();
    //window.addFace(&gridMesh);

    window->addFace(newMesh);


}
