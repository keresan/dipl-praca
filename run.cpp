#include "run.h"

Run::Run(QMainWindow* parent) {

    window = new GLWidget();
    this->parent = parent;
}

void Run::test_selectGrid() {

    //face 1
    Mesh face1 = Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true);

    qDebug() << "face1:";
    qDebug() << "pocet bodov:" << face1.pointsMat.rows;
    qDebug() << "pocet trojuholnikov:" << face1.triangles.count();

	Mesh *gridMesh = new Mesh(Mesh::create2dGrid(cv::Point3d(-50,50,0), cv::Point3d(50,-50,0),1,1));

    gridMesh->_color = QColor(Qt::green);

    Mesh *newMesh = new Mesh(face1.getExtract2dGrid(*gridMesh));

    qDebug() << "newMesh:";
    qDebug() << "pocet bodov:" << newMesh->pointsMat.rows;
    qDebug() << "pocet trojuholnikov:" << newMesh->triangles.count();

    //face1 = face1.crop(cv::Point3d(0,0,0),70,70,130,100);

    //newMesh.writeOBJ("/Users/martin/Documents/skola/diplomka/pomocne/02463d548.abs_grid_2.obj",'.');

    window->addFace(gridMesh);
    window->addFace(newMesh);



}

void Run::test_show() {
     Mesh *face1 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true));

	 Mesh *gridMesh = new Mesh(Mesh::create2dGrid(cv::Point3d(-50,50,0), cv::Point3d(50,-50,0),2,2));

	 gridMesh->_color = QColor(Qt::green);

     window->setWindowTitle("test2_show");
     window->addFace(face1);
	 window->addFace(gridMesh);

}

void Run::test_crop() {
	Mesh *face1 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true));

	Mesh *cropMesh = new Mesh(face1->crop(cv::Point3d(-50,50,0), cv::Point3d(50,-50,0)));

	qDebug() << "cropMesh:";
	qDebug() << "pocet bodov:" << cropMesh->pointsMat.rows;
	qDebug() << "pocet trojuholnikov:" << cropMesh->triangles.count();

	window->addFace(cropMesh);

}

void Run::test_alignFace() {

    Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));
	Mesh *averageFace = new Mesh(Mesh::fromOBJ("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm.obj",true));

	averageFace->_color = QColor(Qt::green);

	qDebug() << "averageFace:";
	qDebug() << "pocet bodov:" << averageFace->pointsMat.rows;
	qDebug() << "pocet trojuholnikov:" << averageFace->triangles.count();


    window->setWindowTitle("test rotate");
    window->addFace(face);
	window->addFace(averageFace);

	face->rotate(0.1,0.1,0.1);
	//window->repaint(); // ??
	parent->show();

	Common::delay(2000);

    FaceAligner faceAligner;

    QTime myTimer;
    myTimer.start();

	//face->centralize();
	//newAverageFace->centralize();
	int iterations = faceAligner.computeAlign(*face, *averageFace, 100, 100);
    qDebug() << "vypocet zarovnania: "<< myTimer.elapsed() << "ms";
	qDebug() << "pocet iteracii: " << iterations;
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

void Run::depthMapMapping() {
    Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));

	DepthMap map(*face);

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

			 printf("%dx%d -> ",r,c);
             cv::Point2f queryPoint;
             int row, col;
             if (map.mapIndeciesToPoint(r,c,queryPoint)) {
				 printf("[%5.2f;%5.2f] -> ",queryPoint.x, queryPoint.y);
                 if(map.mapPointToIndecies(queryPoint,row, col)) {
					 printf("%dx%d ->", row, col);
						cv::Point2f reQueryPoint;
						if(map.mapIndeciesToPoint(row,col,reQueryPoint)) {
							printf("[%5.2f;%5.2f]\n ",reQueryPoint.x, reQueryPoint.y);
						} else {
							printf("unknown\n");
						}

                 } else {
					 printf("unknown ->");
                 }
             } else {
                 printf("unkwnown -> ");
             }

         }

    }
	/*
	int row, col;
	cv::Point2f queryPoint;
	queryPoint = cv::Point2f(5,-5);
	map.mapPointToIndecies(queryPoint,row,col);
	qDebug("[%5.1f %5.1f] -> %d,%d",queryPoint.x, queryPoint.y, row,col);

	queryPoint = cv::Point2f(4,-3);
	map.mapPointToIndecies(queryPoint,row,col);
	qDebug("[%5.1f %5.1f] -> %d,%d",queryPoint.x, queryPoint.y, row,col);
	*/



}
void Run::test_depth_select() {
    Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));
	DepthMap map(*face);



    QVector<cv::Point2f> vector;

    qDebug() << "jedem";

	map.selectFromDepthMap(cv::Point2d(12.502,-48.859), cv::Point2d(13.040,-49.405), vector);

    qDebug() << "selected points: ";
    for(int i = 0; i < vector.count(); i++) {
        qDebug() << vector.at(i).x << vector.at(i).y;
    }


   // map.printPoints();

}

void Run::showDepthMap() {
	//Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d548.abs", true));
	Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true));



    //map.averageValueInTraignle(cv::Point2f(3,3),*face);

    qDebug() << "jedem:";
    QTime myTimer;
    myTimer.start();

	DepthMap map(*face);

    qDebug() << "vytvorenie depth mapy: "<< myTimer.elapsed() << "ms";

	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
	cv::imshow( "Display window", map.depthMap );

	window->addFace(face);

	//Common::printMatrix(map.depthMap);
}

void Run::createAverageFace() {

	//celkovo 75 tvari pouzitych

	QString pathToLandmarks("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range_marked/");
	QString pathToFaces("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/");
	QString pathToFirstFace("02463d550.abs_landmarks.txt");

	AverageFace avg(pathToLandmarks,pathToFaces,pathToFirstFace);

	avg.process("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_05.obj");



}

void Run::normalizeAverageFace() {
	Mesh *averageFace = new Mesh(Mesh::fromOBJ("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final.obj", true));

	averageFace->centralize();

	//averageFace->rotate(0,0,0);

	qDebug() << "averageFace:";
	qDebug() << "pocet bodov:" << averageFace->pointsMat.rows;
	qDebug() << "pocet trojuholnikov:" << averageFace->triangles.count();

	Mesh *croped = new Mesh(averageFace->crop(cv::Point3d(-60,70,0), cv::Point3d(60,-65,0)));

	qDebug() << "croped:";
	qDebug() << "pocet bodov:" << croped->pointsMat.rows;
	qDebug() << "pocet trojuholnikov:" << croped->triangles.count();


	Mesh *gridMesh = new Mesh(Mesh::create2dGrid(cv::Point3d(-60,70,0), cv::Point3d(60,-65,0),6,6));
	Mesh *finalMesh = new Mesh(croped->getExtract2dGrid(*gridMesh));

	qDebug() << "finalMesh:";
	qDebug() << "pocet bodov:" << finalMesh->pointsMat.rows;
	qDebug() << "pocet trojuholnikov:" << finalMesh->triangles.count();
	window->addFace(finalMesh);

	finalMesh->writeOBJ("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm.obj",'.');
}
