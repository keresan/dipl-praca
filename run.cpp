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
	 Mesh *face1 = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true);

	 Mesh *gridMesh = new Mesh(Mesh::create2dGrid(cv::Point3d(-50,50,0), cv::Point3d(50,-50,0),2,2));

	 gridMesh->_color = QColor(Qt::green);

     window->setWindowTitle("test2_show");
     window->addFace(face1);
	 window->addFace(gridMesh);

}

void Run::test_crop() {
	Mesh *face1 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true));

	Mesh *cropMesh = new Mesh(face1->crop(cv::Point3d(-60,70,0), cv::Point3d(60,-70,0)));

	qDebug() << "cropMesh:";
	qDebug() << "pocet bodov:" << cropMesh->pointsMat.rows;
	qDebug() << "pocet trojuholnikov:" << cropMesh->triangles.count();

	window->addFace(cropMesh);

	delete face1;

}

void Run::test_alignFace() {

	Mesh *face1 = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true);
	Mesh *face2 = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d546.abs", true);
	Mesh *face3 = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d552.abs", true);
	Mesh *averageFace = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm_4_4.obj", false);

	/*
	Mesh *face11 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true));
	Mesh *averageFace00 = new Mesh(Mesh::fromOBJ("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm_4_4.obj",true));
	Mesh *face22 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d546.abs", true));
	Mesh *face33 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d552.abs", true));
	*/

	averageFace->_color = QColor(Qt::green);

	//qDebug() << "averageFace:" << averageFace->pointsMat.rows << "bodov";

	//window->addFace(face2);
	//window->addFace(averageFace2);


	//face->rotate(0.1,0.1,0.1);
	//parent->show();


	FaceAligner *faceAligner1 = new FaceAligner;
	//FaceAligner *faceAligner2 = new FaceAligner;
	//FaceAligner *faceAligner3 = new FaceAligner;

    QTime myTimer;
    myTimer.start();


	faceAligner1->findBestStartingPosition(*face2, *averageFace);
	qDebug() << "==" << face2->name << "pred-zarovnanie 2: "<< myTimer.elapsed() << "ms";

	//qDebug() << "best pos: " << bestPos.x << "," << bestPos.y;
   //Common::delay(3000);

	/**
	  * BINGOOOOO
	  * face1 a face2 maju spolu nieco spolocne !!!!!!
	  * ked sa prva tvar zmaze, je uz vsetko okej
	  * -vcera to tak bolo a dneska uz nie :D - asi si niekde prepisujem pamat, alebo nieco podobne
	  *
	  * tak dneska staci, ked sa deletne faceAligner2 a uz to ide v poriadku
	  *
	  * ak sa vsetky tvare vytvoria na heape cez new Mesh(..) tak sa pri druhom volani vypocet spomali. Ak ale vytvoria na stacku,
	  * tak je vypocet rychli.
	  *
	  */
	//delete face2;
	//delete faceAligner2;

	//myTimer.restart();

	//face->centralize();
	//newAverageFace->centralize();
	//int iterations = faceAligner.computeAlign(*face, *averageFace, 100, 100);
	//qDebug() << "vypocet zarovnania: "<< myTimer.elapsed() << "ms";
	//qDebug() << "pocet iteracii: " << iterations;
    myTimer.restart();

	faceAligner1->findBestStartingPosition(*face1, *averageFace);
	qDebug() << "==" << face1->name << "pred-zarovnanie 1: " << myTimer.elapsed() << "ms";
	myTimer.restart();

	faceAligner1->findBestStartingPosition(*face3, *averageFace);
	qDebug() << "==" << face3->name << "pred-zarovnanie 3: " << myTimer.elapsed() << "ms";

	//parent->repaint();
	//Common::delay(1000);

	//faceAligner.alignFaceFast(*face);
	//faceAligner.alignFaceFast(*averageFace);
   // qDebug() << "zarovnanie: "<< myTimer.elapsed() << "ms";

   // parent->centralWidget()->repaint();

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

void Run::test_alignFace2() {
	Mesh *face1 = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true);
	Mesh *face2 = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d546.abs", true);
	Mesh *face3 = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/04201d368.abs", true);
	Mesh *averageFace = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm_4_4.obj", false);
/*
	Mesh *face11 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true));
	Mesh *averageFace11 = new Mesh(Mesh::fromOBJ("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm_4_4.obj",true));
	Mesh *face22 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d546.abs", true));
	Mesh *averageFace22 = new Mesh(Mesh::fromOBJ("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm_4_4.obj",true));
*/
	//face2->_color = QColor(Qt::green);
	averageFace->_color = QColor(Qt::gray);
	window->addFace(face1);
	//window->addFace(face2);
	window->addFace(averageFace);

	//window->repaint();

	FaceAligner faceAligner1;

	parent->show();
	//parent->repaint();
	//window->show();
	Common::delay(1000);

	QTime myTimer;

	myTimer.start();
	faceAligner1.align(*face1, *averageFace,100,50);
	qDebug() << "==zarovnanie 1: "<< myTimer.elapsed() << "ms";

	myTimer.restart();
	faceAligner1.align(*face2, *averageFace,100,50);
	qDebug() << "==zarovnanie 2: "<< myTimer.elapsed() << "ms";

	window->repaint();

	myTimer.restart();
	faceAligner1.align(*face3, *averageFace,100,50);
	qDebug() << "==zarovnanie 3: "<< myTimer.elapsed() << "ms";


	window->repaint();

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

	DepthMap map(*face,cv::Point2d(-150,150),cv::Point2d(150,-150),600,600);

    qDebug() << "vytvorenie depth mapy: "<< myTimer.elapsed() << "ms";

	//save
	cv::Mat dst;

	//map.depthMap.convertTo(dst,CV_8UC1);

	cv::normalize(map.depthMap,dst,0,255,cv::NORM_MINMAX, CV_8UC1);
	cv::imwrite( "/Users/martin/Documents/[]sklad/frgc_data/depthmap/Gray_Image_05.jpg", dst );

	cv::namedWindow( "created depth map", cv::WINDOW_AUTOSIZE );// Create a window for display.
	cv::imshow( "Display window", dst );

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

	//averageFace->centralize();

	//averageFace->rotate(0,0,0);


	qDebug() << "averageFace:";
	qDebug() << "pocet bodov:" << averageFace->pointsMat.rows;

	averageFace->translate(cv::Point3d(0,20,-10));
	averageFace->rotate(0,0.03,0);
	averageFace->translate(cv::Point3d(-2,0,0));

	Mesh *gridMesh = new Mesh(Mesh::create2dGrid(Common::averageFaceTL, Common::averageFaceBR,4,4));
	Mesh *finalMesh = new Mesh(averageFace->getExtract2dGrid(*gridMesh));

	gridMesh->_color = QColor(Qt::green);

	qDebug() << "finalMesh:";
	qDebug() << "pocet bodov:" << finalMesh->pointsMat.rows;


	window->addFace(gridMesh);
	window->addFace(finalMesh);

	finalMesh->writeOBJ("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm_4_4.obj",'.');
}

void Run::createDepthMaps() {

	QTime myTimer;
	myTimer.start();

	//load model face
	Mesh *modelFace = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm_4_4.obj", false);

	QVector<QColor> listOfColors;

	QStringList faceLabels;

	QVector<cv::Mat> images;

	faceLabels.append("02463d546.abs"); listOfColors.append(QColor(Qt::white));
	faceLabels.append("02463d550.abs"); listOfColors.append(QColor(Qt::red));
	faceLabels.append("02463d552.abs"); listOfColors.append(QColor(Qt::green));
	faceLabels.append("02463d554.abs"); listOfColors.append(QColor(Qt::blue));
	faceLabels.append("02463d556.abs"); listOfColors.append(QColor(Qt::cyan));


	for(int i =0; i < faceLabels.count(); i++) {

		Mesh *face = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/"+faceLabels.at(i), true);
		qDebug() << "===loaded face: " << face->name;
		//normalize
		myTimer.restart();
		FaceAligner aligner;
		aligner.align(*face, *modelFace,100,30);

		//useknutie nepotrebnych casti - len kvoli zrychleniu vytvorenia depthmapy
		cv::Point2d tl(Common::depthMapTL.x-2, Common::depthMapTL.y+2);
		cv::Point2d br(Common::depthMapBR.x+2, Common::depthMapBR.y-2);
		face->cropMe(tl, br);

		qDebug() << "zarovnanie a orezanie tvare: "<< i << ":" << myTimer.elapsed() << "ms";
		myTimer.restart();

		//depth map
		DepthMap map(*face);

		//save as image
		cv::Mat dst;
		cv::normalize(map.depthMap,dst,0,255,cv::NORM_MINMAX, CV_8UC1);
		cv::imwrite( "/Users/martin/Documents/[]sklad/frgc_data/depthmap/"+face->name.toStdString()+"_3.jpg", dst );

		cv::imshow(face->name.toStdString(),dst);

		qDebug() << "vytvorenie depthmapy"<< i << ":" << myTimer.elapsed() << "ms";

		images.append(map.depthMap.clone());

		//save as Mat object

		QString name = faceLabels.at(i);
		name.chop(4);
		cv::FileStorage storage("/Users/martin/Documents/[]sklad/frgc_data/depthmap/"+name.toStdString()+"_dm.xml", cv::FileStorage::WRITE);
		storage << "depthmap" << map.depthMap;
		storage.release();


		double min, max;
		cv::minMaxLoc(map.depthMap, &min, &max);
		qDebug("%d: min= %.2f, max= %.2f",i,min,max);

		face->_color = listOfColors.at(i);
		window->addFace(face);
	}


	//EigenFace eigenface;
	//eigenface.pcaTransformation(images,faceLabels);
	//eigenface.pcaTransformation_eigen(images,faceLabels);


	qDebug() << "done";

}

void Run::eigenface() {


	std::vector<cv::Mat> images;
	std::vector<int> labels;


	images.push_back(cv::imread("/Users/martin/Documents/[]sklad/frgc_data/depthmap/02463d546.jpg", 0));
	labels.push_back(546);

	//images.push_back(cv::imread("/Users/martin/Documents/[]sklad/frgc_data/depthmap/02463d550.jpg", 0));
	//labels.push_back(550);

	//images.push_back(cv::imread("/Users/martin/Documents/[]sklad/frgc_data/depthmap/02463d552.jpg", 0));
	//labels.push_back(552);

	images.push_back(cv::imread("/Users/martin/Documents/[]sklad/frgc_data/depthmap/02463d554.jpg", 0));
	labels.push_back(554);

	//images.push_back(cv::imread("/Users/martin/Documents/[]sklad/frgc_data/depthmap/02463d556.jpg", 0));
	//labels.push_back(556);

	int height = images[0].rows;

	cv::Mat Sample = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	//images.pop_back();
	//labels.pop_back();

	int num_components = 10;
	double threshold = 10.0;

	cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
	model->train(images, labels);

	int predictedLabel = -1;
	double confidence = -1.0;

	//model->predict(testSample, predictedLabel, confidence);

	//qDebug("Predicted class = %d (score: %f) / Actual class = %d.", predictedLabel, confidence, testLabel);

	// Here is how to get the eigenvalues of this Eigenfaces model:
	cv::Mat eigenvalues = model->getMat("eigenvalues");

	// And we can do the same to display the Eigenvectors (read Eigenfaces):
	cv::Mat W = model->getMat("eigenvectors");

	// Get the sample mean from the training data
	cv::Mat mean = model->getMat("mean");

	cv::imshow("mean",  EigenFace::norm_0_255(mean.reshape(1, height)));

	// show eigenvectors
	qDebug() << "pocet eigenfaces: " << W.cols;
	 for(int i = 0; i < W.cols; i++) {
		 cv::Mat ev = W.col(i).clone();
		 // Reshape to original size & normalize to [0...255] for imshow.
		 cv::Mat grayscale = EigenFace::norm_0_255(ev.reshape(1, height));
		 // Show the image & apply a Jet colormap for better sensing.
		 cv::Mat cgrayscale;
		 cv::applyColorMap(grayscale, cgrayscale, cv::COLORMAP_JET);

		 cv::imshow(cv::format("eigenface %d",labels[i]),cgrayscale);

		// cv::waitKey(0);
	 }
}

void Run::eigenface_pca() {

	QVector<cv::Mat> images;
	//QVector<cv::Mat> meanFace;

	QStringList faces;
	faces.append("02463d546");
	faces.append("02463d554");

	//faces.append("02463d550");
	//faces.append("02463d552");
	//faces.append("02463d556");

	for(int i =0; i < faces.count(); i++) {
		cv::Mat image = cv::imread("/Users/martin/Documents/[]sklad/frgc_data/depthmap/"+faces.at(i).toStdString()+".jpg",cv::IMREAD_GRAYSCALE);
		images.append(image);
		double min, max;
		cv::minMaxLoc(image, &min, &max);
		qDebug("%d: min= %.2f, max= %.2f",i,min,max);
	}

	cv::Mat data = EigenFace::toRowMatrix(images,CV_32FC1);
	int num_components = 10;

	//average
	cv::Mat meanFace;
	cv::addWeighted(images[0], 0.5, images[1],0.5, 0.0, meanFace);

	QVector<cv::Mat> meanArray;
	meanArray.append(meanFace);
	cv::Mat meanVector = EigenFace::toRowMatrix(meanArray, CV_32FC1);


	//meanFace.append(cv::imread("/Users/martin/Documents/[]sklad/frgc_data/depthmap/mean_face.jpg",cv::IMREAD_GRAYSCALE));
	//cv::Mat mean = EigenFace::toRowMatrix(meanFace, CV_32FC1);

	//cv::imshow("mean",mean);


	//cv::PCA pca(data, meanVector, CV_PCA_DATA_AS_ROW, num_components);
	cv::PCA pca(data, cv::noArray(), CV_PCA_DATA_AS_ROW, num_components);


	cv::Mat meanFromPca = pca.mean.clone();
	//cv::Mat eigenvalues = pca.eigenvalues.clone();
	cv::Mat eigenvectors = pca.eigenvectors.clone();



	cv::imshow("meanFromPca", EigenFace::norm_0_255(meanFromPca.reshape(1, images[0].rows)));
	//cv::imwrite("/Users/martin/Documents/[]sklad/frgc_data/depthmap/mean_face.jpg", EigenFace::norm_0_255(mean.reshape(1, images[0].rows)));

	cv::imshow("mean",meanFace);

	for(int i = 0; i < faces.count(); i++) {

		cv::Mat grayscale = EigenFace::norm_0_255(eigenvectors.row(i).reshape(1, images[0].rows));
		// Show the image & apply a Jet colormap for better sensing.
		//cv::Mat cgrayscale;
		//cv::applyColorMap(grayscale, cgrayscale, cv::COLORMAP_JET);

		imshow(faces.at(i).toStdString(), grayscale);
		//imshow(faces.at(i).toStdString(), EigenFace::norm_0_255(eigenvectors.row(i)).reshape(1, images[0].rows));
	}

	//compute distance
	qDebug() << "max distance: " << cv::norm(eigenvectors.row(0), eigenvectors.row(1), cv::NORM_INF);
	qDebug() << "manhattan distance: " << cv::norm(eigenvectors.row(0), eigenvectors.row(1), cv::NORM_L1);
	qDebug() << "euclidian distance: " << cv::norm(eigenvectors.row(0), eigenvectors.row(1), cv::NORM_L2);

	qDebug() << "L2:" << cv::norm(eigenvectors.row(0), cv::NORM_L2);
	qDebug() << "L2:" << cv::norm(eigenvectors.row(1), cv::NORM_L2);

}

void Run::loadImages() {
	QStringList faceLabels;

	faceLabels.append("02463d546");
	faceLabels.append("02463d550");
	faceLabels.append("02463d552");
	faceLabels.append("02463d554");
	faceLabels.append("02463d556");

	for(int i =0; i < faceLabels.count(); i++) {

		cv::Mat depthMap;

		cv::FileStorage storage("/Users/martin/Documents/[]sklad/frgc_data/depthmap/"+faceLabels.at(i).toStdString()+"_dm.xml", cv::FileStorage::READ);
		storage["depthmap"] >> depthMap;

		cv::imshow(faceLabels.at(i).toStdString(), EigenFace::norm_0_255(depthMap));

		cv::Mat crop = depthMap(cv::Rect(20,50,240,200));
		cv::imshow(faceLabels.at(i).toStdString()+"_crop", EigenFace::norm_0_255(crop));

		cv::Mat eye = depthMap(cv::Rect(20,50,100,60));
		cv::imshow(faceLabels.at(i).toStdString()+"eye", EigenFace::norm_0_255(eye));

		double min, max;
		cv::minMaxLoc(depthMap, &min, &max);
		qDebug("%s: min= %.2f, max= %.2f",faceLabels.at(i).toStdString().c_str(),min,max);
	}


}
