#include "run.h"


Run::Run(QMainWindow* parent) {

    window = new GLWidget();
    this->parent = parent;

	_divideMethod = FaceDivider::method2;

	_compareMethod = Comparator::CorrelationDistance;

	_isArena = true;
	_isEigenface = true;
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
	 Mesh *face1 = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/04581d202.abs", true);

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

void Run::alignFace() {

	//Mesh *face = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/04287d53.abs", true);
	//Mesh *face = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/04440d101.abs", true);
	Mesh *face = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/04287d49.abs", true);
	Mesh *averageFace = new Mesh(Common::pathToAverageFace, false);

	/*
	Mesh *face11 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d558.abs", true));
	Mesh *averageFace00 = new Mesh(Mesh::fromOBJ("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm_4_4.obj",true));
	Mesh *face22 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d546.abs", true));
	Mesh *face33 = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/02463d552.abs", true));
	*/

	averageFace->_color = QColor(Qt::gray);

	window->addFace(face);
	window->addFace(averageFace);

	//face->rotate(0.1,0.1,0.1);
	parent->show();

	//Common::delay(1000);

	FaceAligner *faceAligner1 = new FaceAligner;

    QTime myTimer;
    myTimer.start();


	faceAligner1->align(*face,
						*averageFace,
						Common::alignerMaxIterations,
						Common::alignerConvergentTreshold,
						true);

	qDebug() << "=="
			 << face->name << "iter:" << faceAligner1->finalIterations
			 << ", distance:" << faceAligner1->finalDistance
			 <<", zarovnanie : "<< myTimer.elapsed() << "ms";


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
	Mesh *face = new Mesh(Mesh::fromABS("/Users/martin/Documents/[]sklad/frgc_data/Fall2003range/04203d436.abs", true));


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

	avg.process("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_06.obj");

}

void Run::normalizeAverageFace() {
	Mesh *averageFace = new Mesh(Mesh::fromOBJ(Common::pathToWarehouse+"/averageFace/averageFace_final.obj", true));

	//averageFace->centralize();

	//averageFace->rotate(0,0,0);


	qDebug() << "averageFace:";
	qDebug() << "pocet bodov:" << averageFace->pointsMat.rows;

	averageFace->translate(cv::Point3d(0,20,-10));
	averageFace->rotate(0,0.03,0);
	averageFace->translate(cv::Point3d(-2,0,+10));

	Mesh *gridMesh = new Mesh(Mesh::create2dGrid(Common::averageFaceTL, Common::averageFaceBR,4,4));
	Mesh *finalMesh = new Mesh(averageFace->getExtract2dGrid(*gridMesh));

	gridMesh->_color = QColor(Qt::green);

	qDebug() << "finalMesh:";
	qDebug() << "pocet bodov:" << finalMesh->pointsMat.rows;


	window->addFace(gridMesh);
	window->addFace(finalMesh);

	finalMesh->writeOBJ(Common::pathToAverageFace,'.');
}

void Run::createDepthMaps() {

	QTime myTimer;
	myTimer.start();

	Controller controller;

	QStringList facePaths;

	Mesh *modelFace = new Mesh(Common::pathToAverageFace, false);

	/*
	facePaths.append(Common::pathToFall2003+"04287d49.abs");
	facePaths.append(Common::pathToFall2003+"04287d51.abs");
	facePaths.append(Common::pathToFall2003+"04287d53.abs");
	facePaths.append(Common::pathToFall2003+"04395d200.abs");
	//facePaths.append(Common::pathToAverageFace);
	facePaths.append("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final.obj");
	*/
	Common::loadFilesPathFromDir(Common::pathToFall2003,facePaths,QStringList("*abs"));

	int indexStart = 0;
	int indexStop = 3000;

	for(int i=0; i< facePaths.size();i++) {
		if(i < indexStart) {
			continue;
		} else if(i == indexStop) {
			break;
		}

		myTimer.restart();

		Mesh *face = new Mesh(facePaths.at(i), true);
		cv::Mat depthmap;
		int iterations;
		double distance;
		controller.createDepthmap(*face, *modelFace, depthmap, distance, iterations);

		//compute max
		//double max;
		//cv::minMaxLoc(depthmap, NULL, &max);

		//qDebug() << max;

		//save as image
		/*
		cv::Mat dst;
		cv::normalize(depthMap,dst,0,255,cv::NORM_MINMAX, CV_8UC1);
		cv::imwrite(Common::pathToDepthmap.toStdString()+face->name.toStdString()+"_18.jpg", dst );
		cv::imshow(face->name.toStdString(),dst);
		*/

		//show depthmap

		/*
		cv::Mat dst;
		cv::normalize(depthmap,dst,0,255,cv::NORM_MINMAX, CV_8UC1);
		cv::imshow(face->name.toStdString(),dst);
		*/

		//save as Mat object

		Common::saveDepthmap(face->name+".xml", Common::pathToDepthmapF2003,depthmap, distance, iterations);
		//Common::saveDepthMap(face->name+".xml", Common::pathToWarehouse+"depthmap_tmp2/",depthmap, distance, iterations);


		qDebug("%4d: %10s %3d %6.1f %5d ms",i, face->name.toStdString().c_str(),iterations, distance,myTimer.elapsed());

		delete face;
	}

	delete modelFace;

	qDebug() << "done";

}

/*
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

	cv::imshow("mean",  Common::norm_0_255(mean.reshape(1, height)));

	// show eigenvectors
	qDebug() << "pocet eigenfaces: " << W.cols;
	 for(int i = 0; i < W.cols; i++) {
		 cv::Mat ev = W.col(i).clone();
		 // Reshape to original size & normalize to [0...255] for imshow.
		 cv::Mat grayscale = Common::norm_0_255(ev.reshape(1, height));
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



	cv::imshow("meanFromPca", Common::norm_0_255(meanFromPca.reshape(1, images[0].rows)));
	//cv::imwrite("/Users/martin/Documents/[]sklad/frgc_data/depthmap/mean_face.jpg", Common::norm_0_255(mean.reshape(1, images[0].rows)));

	cv::imshow("mean",meanFace);

	for(int i = 0; i < faces.count(); i++) {

		cv::Mat grayscale = Common::norm_0_255(eigenvectors.row(i).reshape(1, images[0].rows));
		// Show the image & apply a Jet colormap for better sensing.
		//cv::Mat cgrayscale;
		//cv::applyColorMap(grayscale, cgrayscale, cv::COLORMAP_JET);

		imshow(faces.at(i).toStdString(), grayscale);
		//imshow(faces.at(i).toStdString(), Common::norm_0_255(eigenvectors.row(i)).reshape(1, images[0].rows));
	}

	//compute distance
	qDebug() << "max distance: " << cv::norm(eigenvectors.row(0), eigenvectors.row(1), cv::NORM_INF);
	qDebug() << "manhattan distance: " << cv::norm(eigenvectors.row(0), eigenvectors.row(1), cv::NORM_L1);
	qDebug() << "euclidian distance: " << cv::norm(eigenvectors.row(0), eigenvectors.row(1), cv::NORM_L2);

	qDebug() << "L2:" << cv::norm(eigenvectors.row(0), cv::NORM_L2);
	qDebug() << "L2:" << cv::norm(eigenvectors.row(1), cv::NORM_L2);

}
*/

void Run::detectWrongDepthmaps() {
	/*
	 * z prvych 500 vylucenych 32+8 => 8%
	 *
	 * z prvych 100 vylucenych 87 => 8,7%
	 */

	//average landmarks
	Landmarks averageLandmarks(Common::lmPathToLmDir+Common::lmAvgLmLabel);

	cv::Mat averageFace;
	double distance;
	int iterations;
	QString path = Common::pathToAverageDepthmap;
	QFileInfo fileInfo(path);
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),averageFace,averageFace,distance,iterations);



	QStringList facePaths;
	Common::loadFilesPathFromDir(Common::pathToDepthmapF2003,facePaths);

	//facePaths.append(Common::pathToDepthmapF2003+"04265d265.xml");

	for(int i =0; i < facePaths.size(); i++) {
		QFileInfo fileInfo(facePaths.at(i));

		//QFile::rename(fileInfo.absoluteFilePath(), fileInfo.absolutePath()+"/"+fileInfo.baseName()+".xml");
		//continue;

		cv::Mat depthmap;
		Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),depthmap,averageFace,distance,iterations);

		if(iterations == Common::alignerMaxIterations) {
			qDebug() << "wrong depthmap: " << fileInfo.fileName() << iterations;

			//cv::imshow(fileInfo.baseName().toStdString(), Common::norm_0_255(depthmap));

			QFile::rename(fileInfo.absoluteFilePath(), fileInfo.absoluteFilePath()+Common::wrongDpSuffixLabel);
			continue;
		}

		if(distance > Common::alignerDistanceTresholdToContinue-2000) {
			qDebug() << "wrong depthmap: " << fileInfo.fileName() << distance;
			//cv::imshow(fileInfo.baseName().toStdString(), Common::norm_0_255(depthmap));

			QFile::rename(fileInfo.absoluteFilePath(), fileInfo.absoluteFilePath()+Common::wrongDpSuffixLabel);
			continue;
		}

		//check landmarks

		Landmarks landmarks;
		LandmarkDetector detector(depthmap);
		bool result = detector.detectAll(landmarks);
		if(result) {
			result = LandmarkDetector::checkLandmarks(landmarks, averageLandmarks);
			if(!result) {
				qDebug() << "wrong lm check:" << fileInfo.fileName();
				QFile::rename(fileInfo.absoluteFilePath(), fileInfo.absoluteFilePath()+Common::wrongLmSuffixLabel);
				continue;
			}
		} else {
			qDebug() << "wrong lm all:" << fileInfo.fileName();
			QFile::rename(fileInfo.absoluteFilePath(), fileInfo.absoluteFilePath()+Common::wrongLmSuffixLabel);
			continue;
		}

		//DepthMap::showAllLandmarks(depthmap, landmarks,fileInfo.baseName());


		//cv::Mat crop = depthmap(Common::faceCropArea);
		//cv::imshow(fileInfo.baseName().toStdString(), Common::norm_0_255(crop));

		//Common::delay(300);

		/*
		double min,max;
		cv::minMaxLoc(depthmap, &min, &max);
		//qDebug() << fileInfo.fileName() << min << max;
		if(min < Common::depthmapInitValue+10) {
			Common::delay(2000);
		}
		*/

	}

}


void Run::loadDeptmap() {
	QStringList facePaths;

	cv::Mat averageFace;
	double distance;
	int iterations;
	QString path = Common::pathToAverageDepthmap;
	QFileInfo fileInfo(path);
	qDebug() << path;
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),averageFace,averageFace,distance,iterations);

	qDebug() << averageFace.rows << averageFace.cols;

	//get list of files

	//Common::loadFilesPathFromDir(Common::pathToDepthmapF2003,facePaths);

	//NaN
	//facePaths.append(Common::pathToDepthmapF2003+"04440d101.xml");


	/*
	facePaths.append(Common::pathToWarehouse+"depthmap_tmp2/"+"04287d49.xml");
	facePaths.append(Common::pathToWarehouse+"depthmap_tmp2/"+"04287d53.xml");
	facePaths.append(Common::pathToWarehouse+"depthmap_tmp2/"+"04287d51.xml");
	facePaths.append(Common::pathToWarehouse+"depthmap_tmp2/"+"04395d200.xml");
	*/
	//facePaths.append(Common::pathToWarehouse+"depthmap_tmp2/"+"averageFace_final.xml");

	//facePaths.append("04691d130.xml");
	//facePaths.append("04225d303.xml");
	//facePaths.append("04225d299.xml");
	//facePaths.append("02463d554");
	//facePaths.append("02463d556");

	facePaths.append(Common::pathToDepthmapF2003+"04219d419.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04273d246.xml");

	//04273d247

	for(int i =0; i < facePaths.size(); i++) {
		QFileInfo fileInfo(facePaths.at(i));

		qDebug() << facePaths.at(i);
		cv::Mat depthmap;
		Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),depthmap,averageFace,distance,iterations);
		//Common::loadDepthmap(fileInfo.fileName(),fileInfo.path(),depthmap,distance,iterations);


		qDebug() << fileInfo.absolutePath();
		cv::imwrite(Common::pathToWarehouse.toStdString()+"/depthmap_tmp/"+fileInfo.baseName().toStdString()+".jpg", Common::norm_0_255(depthmap));


		double min,max;
		cv::minMaxLoc(depthmap, &min, &max);
		qDebug() << fileInfo.fileName() << min << max;


		//cv::Mat crop = depthmap(Common::faceCropArea);

		qDebug() << "croped";

		cv::imshow(fileInfo.baseName().toStdString(), Common::norm_0_255(depthmap));

		//Common::delay(500);


		/*
		cv::Mat crop = depthMap(cv::Rect(40,50,240,200));

		cv::imshow(faceLabels.at(i).toStdString()+"_crop", Common::norm_0_255(crop));

		//draw smt
		cv::Mat normalizeMap = Common::norm_0_255(crop);
		cv::Mat colorMap;
		cvtColor(normalizeMap, colorMap, CV_GRAY2RGB);

		int thickness = -1;
		int lineType = 8;
		float diameter = 3.0;

		cv::circle(colorMap,cv::Point(50,50), diameter, cv::Scalar( 0, 0, 255 ), thickness, lineType );

		cv::imshow(faceLabels.at(i).toStdString()+"_color", colorMap);
		*/
		/*
		cv::Mat eye = depthMap(cv::Rect(20,50,100,60));
		cv::imshow(faceLabels.at(i).toStdString()+"eye", Common::norm_0_255(eye));

		double min, max;
		cv::minMaxLoc(depthMap, &min, &max);
		qDebug("%s: min= %.2f, max= %.2f",faceLabels.at(i).toStdString().c_str(),min,max);
		*/
	}
}

void Run::showLandmarks() {
	QStringList facePaths;

	//average landmarks
	Landmarks averageLandmarks(Common::lmPathToLmDir+Common::lmAvgLmLabel);

	/*
	QDir dir;
	dir.setPath(Common::pathToDepthmapF2003);
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	dir.setNameFilters(QStringList()<<"*xml");
	faceLabels = dir.entryList();
	*/

	/*
	faceLabels.append("04202d454.xml");

	//faceLabels.append("04217d401.xml");// - chyba
	//faceLabels.append("04203d436.xml");
	faceLabels.append("02463d552.xml"); // predloha pre grafy
	//faceLabels.append("04397d348.xml"); - chyba, ale preskoci
	//faceLabels.append("04300d266.xml"); - chyba a spadne
	faceLabels.append("04222d397.xml");
	faceLabels.append("04287d49.xml");
	faceLabels.append("04287d51.xml");
	faceLabels.append("04287d53.xml");
	//faceLabels.append("04400d294.xml");

	//faceLabels.append("04202d438.xml");

	//faceLabels.append("04581d202.xml");
	//faceLabels.append("04225d303.xml");
	//faceLabels.append("02463d556.xml");
	*/
	//facePaths.append(Common::pathToDepthmapF2003+"04265d267.xml");
	//facePaths.append(Common::pathToDepthmapF2003+"04299d193.xml");
	//facePaths.append(Common::pathToDepthmapF2003+"04219d419.xml");
	//facePaths.append(Common::pathToDepthmapF2003+"04265d267.xml_wrong_lm");


	Common::loadFilesPathFromDir(Common::pathToDepthmapF2003,facePaths);

	//load average face
	cv::Mat averageFace;
	double distance;
	int iterations;
	QString path = Common::pathToAverageDepthmap;
	QFileInfo fileInfo(path);
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),averageFace,averageFace,distance,iterations);

	int indexStop = 20; //facePaths.size()

	for(int i =0; i < indexStop; i += 5) {
		cv::Mat depthmap;
		double distance;
		//qDebug() << "actual file:" << facePaths.at(i);

		QFileInfo fileInfo(facePaths.at(i));
		Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),depthmap,averageFace,distance,iterations);

		/*
		if(distance > Common::alignerDistanceTresholdToContinue) {
			qDebug() << faceLabels.at(i) << "skip";
			continue;
		}

		qDebug() << faceLabels.at(i)
				 << "dist:" << distance;

		*/

		LandmarkDetector detector(depthmap);
		Landmarks landmarks;
		bool result = detector.detectAll(landmarks);

		//draw landmarks
		if(result) {

			//result = LandmarkDetector::checkLandmarks(landmarks, averageLandmarks);
			if(!result) {
				qDebug() << facePaths.at(i) << "skipped for landmarks";
				continue;
			}

			//DepthMap::showAllLandmarks(crop,landmarks,"1 ");


			cv::Mat normalizeMap = Common::norm_0_255(depthmap);
			cv::Mat colorMap;
			cvtColor(normalizeMap, colorMap, CV_GRAY2RGB);

			int thickness = -1;
			int lineType = 8;
			float diameter = 1.0;

			cv::circle(colorMap,landmarks.pos(Landmarks::NoseTip), diameter, cv::Scalar( 0, 0, 255 ), thickness, lineType );
			cv::circle(colorMap,landmarks.pos(Landmarks::NoseRoot), diameter, cv::Scalar( 0, 0, 255 ), thickness, lineType );
			cv::circle(colorMap,landmarks.pos(Landmarks::NoseBottom), diameter, cv::Scalar( 0, 0, 255 ), thickness, lineType );
			cv::circle(colorMap,landmarks.pos(Landmarks::NoseCornerLeft), diameter, cv::Scalar( 0, 255, 0 ), thickness, lineType );
			cv::circle(colorMap,landmarks.pos(Landmarks::NoseCornerRight), diameter, cv::Scalar(255, 0, 0 ), thickness, lineType );
			//cv::circle(colorMap,landmarks.pos(Landmarks::EyeInnerCornerLeft), diameter, cv::Scalar( 0, 255, 0 ), thickness, lineType );
			//cv::circle(colorMap,landmarks.pos(Landmarks::EyeInnerCornerRight), diameter, cv::Scalar( 255, 0, 0 ), thickness, lineType );

			cv::imshow(fileInfo.fileName().toStdString() + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), colorMap);

			cv::imwrite(Common::pathToWarehouse.toStdString()+"depthmap_tmp/04219d419.jpg", Common::norm_0_255(depthmap));

		} else {
			qDebug() << facePaths.at(i) << "skipped";
		}

		Common::delay(400);

	}
}
void Run::init() {

	qDebug() << "#####  Eigenface Initialization";

	Controller controller;
	QVector<cv::Mat> faces;

	QStringList allFacePaths;
	QStringList facePaths;
	QStringList labels;

	Common::loadFilesPathFromDir(Common::pathToDepthmapF2003,allFacePaths);

	//prepare file list


	for(int i = 0; i < allFacePaths.size(); i+=1) {
		if(facePaths.size() < 300) {
			facePaths.append(allFacePaths[i]);
		}
	}

	//kontrola spravneho tvorenia vstupu pre pca
	/*
	for(int i = 0; i < allFacePaths.size(); i+=5) {
		if(facePaths.size() < 10) {
			facePaths.append(allFacePaths[i]);
		}
	}
	*/

	//load average face
	cv::Mat averageFace;
	double distance;
	int iterations;

	QString path = Common::pathToAverageDepthmap;
	QFileInfo fileInfo(path);
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),averageFace,averageFace,distance,iterations);

	//qDebug() << averageFace.rows << "x" << averageFace.cols;

	//qDebug() << "facePaths.size():" << facePaths.size();
	for(int i=0; i< facePaths.size();i++) {
		//qDebug() << i << ": actaul face: " << facePaths.at(i);

		cv::Mat depthmap;
		double distance;
		int iterations;

		QFileInfo fileInfo(facePaths.at(i));

		Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(), depthmap,averageFace, distance, iterations);

		labels.append(fileInfo.baseName());
		faces.append(depthmap);
		//cv::imshow(std::to_string(i),Common::norm_0_255(depthmap));

	}

	//create

	//load landmakrs
	//Landmarks avgLandmarks(Common::lmPathToLmDir+Common::lmAvgLmLabel);

	//compute and save average landmarks
	Landmarks avgLandmarks;
	controller.averageLandmarks(faces, labels, avgLandmarks);
	avgLandmarks.save(Common::lmAvgLmLabel);

	//compute and save pca subspaces

	EigenFace eigenface0;
	controller.createPcaSubspaces(faces,avgLandmarks,eigenface0,FaceDivider::method0);
	eigenface0.saveSubspaces(Common::eigenMethod0Label);

	//landmark divide
	EigenFace eigenface1;
	controller.createPcaSubspaces(faces,avgLandmarks,eigenface1,FaceDivider::method1);
	eigenface1.saveSubspaces(Common::eigenMethod1Label);

	EigenFace eigenface2;
	controller.createPcaSubspaces(faces,avgLandmarks,eigenface2,FaceDivider::method2);
	eigenface2.saveSubspaces(Common::eigenMethod2Label);

	EigenFace eigenface3;
	controller.createPcaSubspaces(faces,avgLandmarks,eigenface3,FaceDivider::method3);
	eigenface3.saveSubspaces(Common::eigenMethod3Label);

	//rigid divide
	EigenFace eigenface1r;
	controller.createPcaSubspaces(faces,avgLandmarks,eigenface1r,FaceDivider::method1rigid);
	eigenface1r.saveSubspaces(Common::eigenMethod1rLabel);

	EigenFace eigenface2r;
	controller.createPcaSubspaces(faces,avgLandmarks,eigenface2r,FaceDivider::method2rigid);
	eigenface2r.saveSubspaces(Common::eigenMethod2rLabel);

	EigenFace eigenface3r;
	controller.createPcaSubspaces(faces,avgLandmarks,eigenface3r,FaceDivider::method3rigid);
	eigenface3r.saveSubspaces(Common::eigenMethod3rLabel);

}

void Run::divideFace() {
	QStringList faceLabels;

	/*
	QDir dir;
	dir.setPath(Common::pathToDepthmapF2003);
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	dir.setNameFilters(QStringList()<<"*xml");
	faceLabels = dir.entryList();
	*/

	//faceLabels.append("04202d454.xml");

	//faceLabels.append("04217d401.xml"); - chyba
	faceLabels.append("04203d436.xml");
	//faceLabels.append("04202d438.xml");

	//faceLabels.append("04581d202.xml");
	//faceLabels.append("04225d303.xml");
	//faceLabels.append("02463d556.xml");

	//for compute average position of each landmark
	Landmarks averageLandmakrks(Common::lmAvgLmLabel);

	int max =  faceLabels.size();

	for(int i = 0; i < max; i++) {
		cv::Mat depthMap;
		double distance;

		cv::FileStorage storage(Common::pathToDepthmapF2003.toStdString()+faceLabels.at(i).toStdString(), cv::FileStorage::READ);
		storage[Common::depthmapDepthmapLabel.toStdString()] >> depthMap;
		storage[Common::depthmapDistanceFromModelLabel.toStdString()] >> distance;

		cv::medianBlur(depthMap,depthMap,5);

		//LandmarkDetector detector(depthMap);
		cv::Mat crop = depthMap(Common::faceCropArea);
		//cv::Mat crop = depthMap;

		//cv::imshow(faceLabels.at(i).toStdString() + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), Common::norm_0_255(depthMap));

		LandmarkDetector detector(crop);
		Landmarks landmarks;


		bool result = detector.detectAll(landmarks);

		if(result) {
			cv::imshow(faceLabels.at(i).toStdString() + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), Common::norm_0_255(crop));
			//divide face
			tFaceAreas areas;
			FaceDivider faceDivider(crop,landmarks,averageLandmakrks);
			faceDivider.divide(FaceDivider::method3rigid,areas);

			for(int i =0; i < areas.size(); i++) {
				cv::Mat actualArea = areas[i];
				cv::imshow(std::to_string(i), Common::norm_0_255(actualArea));
			}
		} else {
			qDebug() << faceLabels.at(i) << "skipped";
		}
	}

}

void Run::processFace() {
	bool result;


	QString eigenMethodLabel = Common::eigenMethod3Label;
	FaceDivider::DivideMethod divideMethod = FaceDivider::method3;


	//load average landmarks
	Landmarks avgLandmarks(Common::lmAvgLmLabel);

	//load pca subspace
	EigenFace eigenface; //= eigenfaceMethod0;
	eigenface.loadSubspaces(eigenMethodLabel);

	cv::Mat depthmap;
	double distance;
	int iterations;

	//load average face
	cv::Mat averageFace;
	QString pathToAverage = Common::pathToAverageDepthmap;
	QFileInfo fileInfo1(pathToAverage);
	Common::loadDepthmapProcess(fileInfo1.fileName(),fileInfo1.path(),averageFace,averageFace,distance,iterations);


	//load face
	QString path = Common::pathToDepthmapF2003+"04395d202.xml";

	QFileInfo fileInfo(path);
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),depthmap,averageFace,distance,iterations);

	Controller controller;
	tFeatures featuresVector;
	QVector<cv::Mat> featuresVectorArena;
	//controller.processFace(depthmap, "04397d348",avgLandmarks,result,divideMethod, eigenface, featuresVector);

	controller.procesFaceArena(depthmap,"04397d348", avgLandmarks,result, divideMethod,featuresVectorArena);


}

void Run::compareFacesInit() {
	qDebug() << "#####  Compare Faces Initialization";
	bool result = false;


	FaceDivider::DivideMethod divideMethod = _divideMethod;
	Comparator::CompareMethod compareMethod = _compareMethod;

	QString saveGenuineFile;
	QString saveImposterFile;
	QString eigenMethodLabel;
	QString errMethodLabel;
	QString errArenaMethodLabel;
	QString scnArenaMethodLabel;
	QString scnMethodLabel;

	switch(divideMethod) {
		case FaceDivider::method0:
			eigenMethodLabel = Common::eigenMethod0Label;
			saveGenuineFile = Common::cmpResultGenuineMethod0Label;
			saveImposterFile = Common::cmpResultImposterMethod0Label;
			errMethodLabel = Common::statEerMethod0Label;
			errArenaMethodLabel = Common::statEerArenaMethod0Label;
			scnArenaMethodLabel = Common::scnStatArenaMethod0Label;
			scnMethodLabel = Common::scnStatMethod0Label;
			break;
		case FaceDivider::method1:
			eigenMethodLabel = Common::eigenMethod1Label;
			saveGenuineFile = Common::cmpResultGenuineMethod1Label;
			saveImposterFile = Common::cmpResultImposterMethod1Label;
			errMethodLabel = Common::statEerMethod1Label;
			errArenaMethodLabel = Common::statEerArenaMethod1Label;
			scnArenaMethodLabel = Common::scnStatArenaMethod1Label;
			scnMethodLabel = Common::scnStatMethod1Label;
			break;
		case FaceDivider::method2:
			eigenMethodLabel = Common::eigenMethod2Label;
			saveGenuineFile = Common::cmpResultGenuineMethod2Label;
			saveImposterFile = Common::cmpResultImposterMethod2Label;
			errMethodLabel = Common::statEerMethod2Label;
			errArenaMethodLabel = Common::statEerArenaMethod2Label;
			scnArenaMethodLabel = Common::scnStatArenaMethod2Label;
			scnMethodLabel = Common::scnStatMethod2Label;
			break;
		case FaceDivider::method3:
			eigenMethodLabel = Common::eigenMethod3Label;
			saveGenuineFile = Common::cmpResultGenuineMethod3Label;
			saveImposterFile = Common::cmpResultImposterMethod3Label;
			errMethodLabel = Common::statEerMethod3Label;
			errArenaMethodLabel = Common::statEerArenaMethod3Label;
			scnArenaMethodLabel = Common::scnStatArenaMethod3Label;
			scnMethodLabel = Common::scnStatMethod3Label;
			break;
		case FaceDivider::method1rigid:
			eigenMethodLabel = Common::eigenMethod1rLabel;
			saveGenuineFile = Common::cmpResultGenuineMethod1rLabel;
			saveImposterFile = Common::cmpResultImposterMethod1rLabel;
			errMethodLabel = Common::statEerMethod1rLabel;
			errArenaMethodLabel = Common::statEerArenaMethod1rLabel;
			scnArenaMethodLabel = Common::scnStatArenaMethod1rLabel;
			scnMethodLabel = Common::scnStatMethod1rLabel;
			break;
		case FaceDivider::method2rigid:
			eigenMethodLabel = Common::eigenMethod2rLabel;
			saveGenuineFile = Common::cmpResultGenuineMethod2rLabel;
			saveImposterFile = Common::cmpResultImposterMethod2rLabel;
			errMethodLabel = Common::statEerMethod2rLabel;
			errArenaMethodLabel = Common::statEerArenaMethod2rLabel;
			scnArenaMethodLabel = Common::scnStatArenaMethod2rLabel;
			scnMethodLabel = Common::scnStatMethod2rLabel;
			break;
		case FaceDivider::method3rigid:
			eigenMethodLabel = Common::eigenMethod3rLabel;
			saveGenuineFile = Common::cmpResultGenuineMethod3rLabel;
			saveImposterFile = Common::cmpResultImposterMethod3rLabel;
			errMethodLabel = Common::statEerMethod3rLabel;
			errArenaMethodLabel = Common::statEerArenaMethod3rLabel;
			scnArenaMethodLabel = Common::scnStatArenaMethod3rLabel;
			scnMethodLabel = Common::scnStatMethod3rLabel;
			break;
		default:
			throw std::runtime_error("compareFacesInit(): unknown divide method");
	}

	//load average landmarks
	Landmarks avgLandmarks(Common::pathToAverageLm);

	//load pca subspace
	EigenFace eigenface;
	eigenface.loadSubspaces(eigenMethodLabel);

	//load average depthmap
	cv::Mat averageFace;
	double distance;
	int iterations;
	QString path = Common::pathToAverageDepthmap;
	QFileInfo fileInfo(path);
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),averageFace,averageFace,distance,iterations);

	cv::Mat depthmap;

	QStringList facePaths;
	QStringList labels;
	Controller controller;
	QVector<tFeatures> featuresVector;
	QVector<QVector<cv::Mat> >featuresVectorArena;

	Common::loadFilesPathFromDir(Common::pathToDepthmapF2003,facePaths);

	int indexStart = 400;
	int indexStop = 700; //700


	for(int i =0; i < facePaths.size(); i++) {
		tFeatures features;
		QVector<cv::Mat> featuresArena;

		if(i < indexStart && facePaths.size() > indexStart) {
			continue;
		} else if(i > indexStop) {
			break;
		}

		//load depthmap
		QFileInfo fileInfo(facePaths.at(i));
		Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(), depthmap,averageFace,distance,iterations);

		// eigenface algorithm
		if(_isEigenface) {
			controller.processFace(depthmap,
								   fileInfo.baseName(),
								   avgLandmarks,
								   result,
								   divideMethod,
								   eigenface,
								   features,
								   false);
			if(!result) {
				qDebug() << "ERROR:" << facePaths.at(i);
			}

			featuresVector.append(features);
		}

		// arena algorithm
		if(_isArena) {
			controller.procesFaceArena(depthmap,
									   fileInfo.baseName(),
									   avgLandmarks,
									   result,
									   divideMethod,
									   featuresArena);
			if(!result) {
				qDebug() << "ERROR ARENA:" << facePaths.at(i);
			}
			featuresVectorArena.append(featuresArena);
		}
		labels.append(fileInfo.baseName());
	}
	qDebug() << "featuresVector.size() a.k.a pocet vstupnych tvari:" << featuresVector.size();
	qDebug() << "featuresVectorArena.size() a.k.a pocet vstupnych tvari:" << featuresVectorArena.size();

	QVector<QVector<float> > impResult, genResult;
	QVector<QVector<float> > impResultArena, genResultArena;

	int vectorSize;
	if(_isEigenface) {
		vectorSize = featuresVector.size();
	} else {
		vectorSize = featuresVectorArena.size();
	}

	//compare each with each
	for(int i = 0; i < vectorSize ; i++) {
		for(int j = i+1; j < vectorSize; j++) {
			//compare features
			bool isGenuine = Common::isGenuinePerson(labels.at(i), labels.at(j));

			QVector<float> distances;
			QVector<float> distancesArena;

			// eigenface compare
			if(_isEigenface) {
				Comparator::compare(featuresVector[i],featuresVector[j],distances, compareMethod);

				//add result
				if(isGenuine) {
					genResult.append(distances);
				} else {
					impResult.append(distances);
				}
			}

			//arena::compare
			if(_isArena) {
				Comparator::compare(featuresVectorArena[i],featuresVectorArena[j],distancesArena, compareMethod);
				//add result
				if(isGenuine) {
					genResultArena.append(distancesArena);
				} else {
					impResultArena.append(distancesArena);
				}
			}
		}
	}

	if(_isEigenface) {
		//convert to matrix
		cv::Mat matToSaveGenuine, matToSaveImposter;
		Common::convertToMatrix(genResult, matToSaveGenuine);
		Common::convertToMatrix(impResult, matToSaveImposter);

		qDebug() << "matToSaveGenuine" << matToSaveGenuine.rows << "x" << matToSaveGenuine.cols;
		qDebug() << "matToSaveImposter" << matToSaveImposter.rows << "x" << matToSaveImposter.cols;

		//compute equal error rate
		Stats statistics(matToSaveImposter,matToSaveGenuine);
		statistics.computeEer();
		statistics.saveEer(errMethodLabel);

		//save stats for score normalizer
		ScoreNormalizer normalizer(ScoreNormalizer::zScore,matToSaveGenuine,matToSaveImposter);
		normalizer.save(scnMethodLabel);
	}

	if(_isArena) {

		//convert to matrix
		cv::Mat matToSaveGenuineArena, matToSaveImposterArena;
		Common::convertToMatrix(genResultArena, matToSaveGenuineArena);
		Common::convertToMatrix(impResultArena, matToSaveImposterArena);


		qDebug() << "matToSaveGenuineArena" << matToSaveGenuineArena.rows << "x" << matToSaveGenuineArena.cols;
		qDebug() << "matToSaveImposterArena" << matToSaveImposterArena.rows << "x" << matToSaveImposterArena.cols;

		//compute equal error rate for arena
		Stats statisticsArena(matToSaveImposterArena,matToSaveGenuineArena);
		statisticsArena.computeEer();
		statisticsArena.saveEer(errArenaMethodLabel);

		//save stats for score normalizer
		ScoreNormalizer normalizer(ScoreNormalizer::zScore,matToSaveGenuineArena,matToSaveImposterArena);
		normalizer.save(scnArenaMethodLabel);
	}

	//save result - naco sa to uklada pri inicializacii ?? ???
	//Common::saveCmpResult(matToSaveImposter, saveImposterFile);
	//Common::saveCmpResult(matToSaveGenuine, saveGenuineFile );

}

void Run::compareFaces() {

	qDebug() << "#####  Compare Faces";

	bool result = false;

	FaceDivider::DivideMethod divideMethod = _divideMethod;
	Comparator::CompareMethod compareMethod = _compareMethod;
	ScoreFusioner::FusionMethod fusionMethod = ScoreFusioner::WeightedSum;

	QString saveGenuineFile;
	QString saveImposterFile;
	QString saveGenuineArenaFile;
	QString saveImposterArenaFile;
	QString saveGenuineBothFile;
	QString saveImposterBothFile;

	QString eigenMethodLabel;
	QString loadScnArenaFile;
	QString loadScnFile;
	QString errMethodLabel;
	QString errArenaMethodLabel;


	switch(divideMethod) {
		case FaceDivider::method0:
			eigenMethodLabel = Common::eigenMethod0Label;
			saveGenuineFile = Common::cmpResultGenuineMethod0Label;
			saveImposterFile = Common::cmpResultImposterMethod0Label;
			loadScnArenaFile  = Common::scnStatArenaMethod0Label;
			loadScnFile = Common::scnStatMethod0Label;
			errMethodLabel = Common::statEerMethod0Label;
			errArenaMethodLabel = Common::statEerArenaMethod0Label;
			saveGenuineArenaFile = Common::cmpResultArenaGenuineMethod0Label;
			saveImposterArenaFile = Common::cmpResultArenaImposterMethod0Label;
			saveGenuineBothFile = Common::cmpResultBothGenuineMethod0Label;
			saveImposterBothFile = Common::cmpResultBothImposterMethod0Label;
			break;
		case FaceDivider::method1:
			eigenMethodLabel = Common::eigenMethod1Label;
			saveGenuineFile = Common::cmpResultGenuineMethod1Label;
			saveImposterFile = Common::cmpResultImposterMethod1Label;
			loadScnArenaFile  = Common::scnStatArenaMethod1Label;
			loadScnFile = Common::scnStatMethod1Label;
			errMethodLabel = Common::statEerMethod1Label;
			errArenaMethodLabel = Common::statEerArenaMethod1Label;
			saveGenuineArenaFile = Common::cmpResultArenaGenuineMethod1Label;
			saveImposterArenaFile = Common::cmpResultArenaImposterMethod1Label;
			saveGenuineBothFile = Common::cmpResultBothGenuineMethod1Label;
			saveImposterBothFile = Common::cmpResultBothImposterMethod1Label;
			break;
		case FaceDivider::method2:
			eigenMethodLabel = Common::eigenMethod2Label;
			saveGenuineFile = Common::cmpResultGenuineMethod2Label;
			saveImposterFile = Common::cmpResultImposterMethod2Label;
			loadScnArenaFile  = Common::scnStatArenaMethod2Label;
			loadScnFile = Common::scnStatMethod2Label;
			errMethodLabel = Common::statEerMethod2Label;
			errArenaMethodLabel = Common::statEerArenaMethod2Label;
			saveGenuineArenaFile = Common::cmpResultArenaGenuineMethod2Label;
			saveImposterArenaFile = Common::cmpResultArenaImposterMethod2Label;
			saveGenuineBothFile = Common::cmpResultBothGenuineMethod2Label;
			saveImposterBothFile = Common::cmpResultBothImposterMethod2Label;
			break;
		case FaceDivider::method3:
			eigenMethodLabel = Common::eigenMethod3Label;
			saveGenuineFile = Common::cmpResultGenuineMethod3Label;
			saveImposterFile = Common::cmpResultImposterMethod3Label;
			loadScnArenaFile  = Common::scnStatArenaMethod3Label;
			loadScnFile = Common::scnStatMethod3Label;
			errMethodLabel = Common::statEerMethod3Label;
			errArenaMethodLabel = Common::statEerArenaMethod3Label;
			saveGenuineArenaFile = Common::cmpResultArenaGenuineMethod3Label;
			saveImposterArenaFile = Common::cmpResultArenaImposterMethod3Label;
			saveGenuineBothFile = Common::cmpResultBothGenuineMethod3Label;
			saveImposterBothFile = Common::cmpResultBothImposterMethod3Label;
			break;
		case FaceDivider::method1rigid:
			eigenMethodLabel = Common::eigenMethod1rLabel;
			saveGenuineFile = Common::cmpResultGenuineMethod1rLabel;
			saveImposterFile = Common::cmpResultImposterMethod1rLabel;
			loadScnArenaFile  = Common::scnStatArenaMethod1rLabel;
			loadScnFile = Common::scnStatMethod1rLabel;
			errMethodLabel = Common::statEerMethod1rLabel;
			errArenaMethodLabel = Common::statEerArenaMethod1rLabel;
			saveGenuineArenaFile = Common::cmpResultArenaGenuineMethod1rLabel;
			saveImposterArenaFile = Common::cmpResultArenaImposterMethod1rLabel;
			saveGenuineBothFile = Common::cmpResultBothGenuineMethod1rLabel;
			saveImposterBothFile = Common::cmpResultBothImposterMethod1rLabel;
			break;
		case FaceDivider::method2rigid:
			eigenMethodLabel = Common::eigenMethod2rLabel;
			saveGenuineFile = Common::cmpResultGenuineMethod2rLabel;
			saveImposterFile = Common::cmpResultImposterMethod2rLabel;
			loadScnArenaFile  = Common::scnStatArenaMethod2rLabel;
			loadScnFile = Common::scnStatMethod2rLabel;
			errMethodLabel = Common::statEerMethod2rLabel;
			errArenaMethodLabel = Common::statEerArenaMethod2rLabel;
			saveGenuineArenaFile = Common::cmpResultArenaGenuineMethod2rLabel;
			saveImposterArenaFile = Common::cmpResultArenaImposterMethod2rLabel;
			saveGenuineBothFile = Common::cmpResultBothGenuineMethod2rLabel;
			saveImposterBothFile = Common::cmpResultBothImposterMethod2rLabel;
			break;
		case FaceDivider::method3rigid:
			eigenMethodLabel = Common::eigenMethod3rLabel;
			saveGenuineFile = Common::cmpResultGenuineMethod3rLabel;
			saveImposterFile = Common::cmpResultImposterMethod3rLabel;
			loadScnArenaFile  = Common::scnStatArenaMethod3rLabel;
			loadScnFile = Common::scnStatMethod3rLabel;
			errMethodLabel = Common::statEerMethod3rLabel;
			errArenaMethodLabel = Common::statEerArenaMethod3rLabel;
			saveGenuineArenaFile = Common::cmpResultArenaGenuineMethod3rLabel;
			saveImposterArenaFile = Common::cmpResultArenaImposterMethod3rLabel;
			saveGenuineBothFile = Common::cmpResultBothGenuineMethod3rLabel;
			saveImposterBothFile = Common::cmpResultBothImposterMethod3rLabel;
			break;
		default:
			throw std::runtime_error("compareFaces(): unknown divide method");
	}

	//load average landmarks
	Landmarks avgLandmarks(Common::pathToAverageLm);

	//load pca subspace
	EigenFace eigenface;// = eigenfaceMethod0;
	eigenface.loadSubspaces(eigenMethodLabel);

	//load average depthmap
	cv::Mat averageFace;
	double distance;
	int iterations;
	QString path = Common::pathToAverageDepthmap;
	QFileInfo fileInfo(path);
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),averageFace,averageFace,distance,iterations);

	cv::Mat depthmap;

	QStringList facePaths;
	QStringList labels;


	//load normalizer

	/*
	ScoreNormalizer normArena, normEigenface;
	if(_isEigenface) {
		normEigenface = ScoreNormalizer(ScoreNormalizer::zScore,loadScnArenaFile);
	}
	if(_isArena) {
		normArena= ScoreNormalizer(ScoreNormalizer::zScore,loadScnFile);
	}

	*/

	//ScoreNormalizer normImposter(ScoreNormalizer::zScore,Common::cmpResultImposterMethod1Label);

	/*
	facePaths.append(Common::pathToDepthmapF2003+"04202d438.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04287d53.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04435d354.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04395d200.xml");
	*/

	/*
	facePaths.append(Common::pathToDepthmapF2003+"04202d438.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04435d340.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04217d405.xml");
	*/

	//facePaths.append(Common::pathToDepthmapF2003+"02463d546.xml_init");
	//facePaths.append(Common::pathToDepthmapF2003+"04201d370.xml_init");
	//facePaths.append(Common::pathToDepthmapF2003+"04447d131.xml");

	//facePaths.append(Common::pathToDepthmapF2003+"04430d271.xml");
	//facePaths.append(Common::pathToDepthmapF2003+"04435d344.xml");

	/*
=== "04575d300" vs "04579d260" : false 0.107441
=== "04575d300" vs "04588d137" : false 0.152751
	 */


	/*
	facePaths.append(Common::pathToDepthmapF2003+"04575d302.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04575d300.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04579d260.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04588d137.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04618d164.xml");
	*/

	/*
=== "04429d347" vs "04435d344" : false 139.923
=== "04557d335" vs "04579d260" : false 136.917
=== "04557d337" vs "04579d260" : false 139.57
	 */

	/*
	facePaths.append(Common::pathToDepthmapF2003+"04429d347.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04435d344.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04557d335.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04579d260.xml");
	*/

	//facePaths.append("averageFace_final_norm_4_4");
	//facePaths.append("averageFace_final");

	/*
	facePaths.append("04395d200");
	facePaths.append("04395d202");
	facePaths.append("04388d293");
	facePaths.append("04388d297");
	facePaths.append("04379d286");
	*/

	Common::loadFilesPathFromDir(Common::pathToDepthmapF2003,facePaths);

	Controller controller;
	QVector<tFeatures> featuresVector;
	QVector<QVector<cv::Mat> >featuresVectorArena;

	int indexStart = 400;
	int indexStop = 1000; //1000

	for(int i =0; i < facePaths.size(); i++) {
		tFeatures features;
		QVector<cv::Mat> featuresArena;

		if(i < indexStart && facePaths.size() > indexStart) {
			continue;
		} else if(i > indexStop) {
			break;
		}

		//load face depthmap
		QFileInfo fileInfo(facePaths.at(i));
		Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(), depthmap,averageFace,distance,iterations);

		//qDebug() << facePaths.at(i) << depthmap.rows << "x" << depthmap.cols;

		// eigenface algorithm
		if(_isEigenface) {
			controller.processFace(depthmap,
								   fileInfo.baseName(),
								   avgLandmarks,
								   result,
								   divideMethod,
								   eigenface,
								   features,
								   false);
			if(!result) {
				qDebug() << "ERROR:" << facePaths.at(i);
			}
			featuresVector.append(features);
		}

		// arena algorithm
		if(_isArena) {
			controller.procesFaceArena(depthmap,
									   fileInfo.baseName(),
									   avgLandmarks,
									   result,
									   divideMethod,
									   featuresArena);
			if(!result) {
				qDebug() << "ERROR ARENA:" << facePaths.at(i);
			}
			featuresVectorArena.append(featuresArena);
		}
		labels.append(fileInfo.baseName());
	}
	qDebug() << "featuresVector.size() a.k.a pocet vstupnych tvari:" << featuresVector.size();
	qDebug() << "featuresVectorArena.size() a.k.a pocet vstupnych tvari:" << featuresVectorArena.size();


	//initialize fusioner for arena and eigenface
	ScoreFusioner fusionerEigenFace, fusionerArena, fusionerBoth;

	QVector<float>  weightsBoth,weightsArena,weightsEigenface;

	if(_isEigenface) {
		Stats::loadEer(weightsEigenface, errMethodLabel);
		if(fusionMethod == ScoreFusioner::WeightedSum) {
			fusionerEigenFace.setWeightsAsComplement(weightsEigenface);
		}
	}

	if(_isArena) {
		Stats::loadEer(weightsArena, errArenaMethodLabel);
		if(fusionMethod == ScoreFusioner::WeightedSum) {
			fusionerArena.setWeightsAsComplement(weightsArena);
		}

	}
	if(_isArena && _isEigenface) {
		weightsBoth = weightsEigenface << weightsArena;
		if(fusionMethod == ScoreFusioner::WeightedSum) {
			fusionerBoth.setWeightsAsComplement(weightsBoth);
		}
	}

	int counter = 0;
	int filterCounter = 0;

	QVector<QVector<float> > impResult, genResult;
	QVector<QVector<float> > impResultArena, genResultArena;
	QVector<QVector<float> > impResultBoth, genResultBoth;

	int vectorSize;
	if(_isEigenface) {
		vectorSize = featuresVector.size();
	} else {
		vectorSize = featuresVectorArena.size();
	}

	//compare each with each
	for(int i = 0; i < vectorSize; i++) {
		for(int j = i+1; j < vectorSize; j++) {
			//compare features
			bool isGenuine = Common::isGenuinePerson(labels.at(i), labels.at(j));

			QVector<float> distances;
			//QVector<float> distancesNormalize;
			QVector<float> distancesArena;
			//QVector<float> distancesArenaNormalize;
			QVector<float> distanceBoth;



			// eigenface compare
			if(_isEigenface) {
				Comparator::compare(featuresVector[i],featuresVector[j],distances, compareMethod);
				//normalize result
				//normEigenface.normalize(distances, distancesNormalize);


				//score fusion
				float finalScore = fusionerEigenFace.fusion(distances,fusionMethod);

				distanceBoth = distanceBoth << distances;

				distances.append(finalScore);

				//add result
				if(isGenuine) {
					genResult.append(distances);
				} else {
					impResult.append(distances);
				}

			}

			//arena compare
			if(_isArena) {
				Comparator::compare(featuresVectorArena[i],featuresVectorArena[j],distancesArena, compareMethod);

				//normalize result
				//normArena.normalize(distancesArena, distancesArenaNormalize);

				//score fusion
				float finalScore = fusionerArena.fusion(distancesArena,fusionMethod);\

				distanceBoth = distanceBoth << distancesArena;

				distancesArena.append(finalScore);

				//add result
				if(isGenuine) {
					genResultArena.append(distancesArena);
				} else {
					impResultArena.append(distancesArena);
				}
			}


			// fusion arena and eigenface result
			if(_isArena && _isEigenface) {
				float finalScore = fusionerBoth.fusion(distanceBoth,fusionMethod);

				//distanceBoth.append(bothFusion);

				QVector<float> finalVector;
				finalVector.append(finalScore);

				//add result
				if(isGenuine) {
					genResultBoth.append(finalVector);
				} else {
					impResultBoth.append(finalVector);
				}

			}

			/*
			//qDebug() << "===" << labels.at(i) << "vs" << labels.at(j) << ":" << isGenuine << distances[0];
			for(int k = 0; k < distances.size(); k++ ) {
				//qDebug() << k << ":" << distances.at(k) << "->" << normDistances.at(k);
				;
			}
			*/

			counter++;
		}
	}
	qDebug() << "filter counter: " << filterCounter;
	qDebug() << "pocet porovnani: " << counter;

	if(_isEigenface) {
		//convert to matrix
		cv::Mat matToSaveGenuine, matToSaveImposter;
		Common::convertToMatrix(genResult, matToSaveGenuine);
		Common::convertToMatrix(impResult, matToSaveImposter);

		qDebug() << "matToSaveGenuine" << matToSaveGenuine.rows << "x" << matToSaveGenuine.cols;
		qDebug() << "matToSaveImposter" << matToSaveImposter.rows << "x" << matToSaveImposter.cols;

		//save it
		Common::saveCmpResult(matToSaveImposter, saveImposterFile);
		Common::saveCmpResult(matToSaveGenuine, saveGenuineFile);

	}
	if(_isArena) {
		//convert to matrix
		cv::Mat matToSaveGenuineBoth, matToSaveImposterBoth;
		Common::convertToMatrix(genResultBoth, matToSaveGenuineBoth);
		Common::convertToMatrix(impResultBoth, matToSaveImposterBoth);

		qDebug() << "matToSaveGenuineBoth" << matToSaveGenuineBoth.rows << "x" << matToSaveGenuineBoth.cols;
		qDebug() << "matToSaveImposterBoth" << matToSaveImposterBoth.rows << "x" << matToSaveImposterBoth.cols;

		//save it
		Common::saveCmpResult(matToSaveImposterBoth, saveImposterArenaFile);
		Common::saveCmpResult(matToSaveGenuineBoth, saveGenuineArenaFile);

		}

	if(_isArena && _isEigenface) {
		//convert to matrix
		cv::Mat matToSaveGenuineBoth, matToSaveImposterBoth;
		Common::convertToMatrix(genResultBoth, matToSaveGenuineBoth);
		Common::convertToMatrix(impResultBoth, matToSaveImposterBoth);

		qDebug() << "matToSaveGenuineBoth" << matToSaveGenuineBoth.rows << "x" << matToSaveGenuineBoth.cols;
		qDebug() << "matToSaveImposterBoth" << matToSaveImposterBoth.rows << "x" << matToSaveImposterBoth.cols;

		//save it
		Common::saveCmpResult(matToSaveImposterBoth, saveImposterBothFile);
		Common::saveCmpResult(matToSaveGenuineBoth, saveGenuineBothFile);

		}

}


void Run::histogram() {

	FaceDivider::DivideMethod divideMethod = _divideMethod;
	QString loadGenuineFile;
	QString loadImposterFile;
	QString loadGenuineFileArena;
	QString loadImposterFileArena;
	QString loadGenuineFileBoth;
	QString loadImposterFileBoth;


	switch(divideMethod) {
		case FaceDivider::method0:
			loadGenuineFile = Common::cmpResultGenuineMethod0Label;
			loadImposterFile = Common::cmpResultImposterMethod0Label;
			loadGenuineFileArena = Common::cmpResultArenaGenuineMethod0Label;
			loadImposterFileArena = Common::cmpResultArenaImposterMethod0Label;
			loadGenuineFileBoth = Common::cmpResultBothGenuineMethod0Label;
			loadImposterFileBoth = Common::cmpResultBothImposterMethod0Label;
			break;
		case FaceDivider::method1:
			loadGenuineFile = Common::cmpResultGenuineMethod1Label;
			loadImposterFile = Common::cmpResultImposterMethod1Label;
			loadGenuineFileArena = Common::cmpResultArenaGenuineMethod1Label;
			loadImposterFileArena = Common::cmpResultArenaImposterMethod1Label;
			loadGenuineFileBoth = Common::cmpResultBothGenuineMethod1Label;
			loadImposterFileBoth = Common::cmpResultBothImposterMethod1Label;
			break;
		case FaceDivider::method2:
			loadGenuineFile = Common::cmpResultGenuineMethod2Label;
			loadImposterFile = Common::cmpResultImposterMethod2Label;
			loadGenuineFileArena = Common::cmpResultArenaGenuineMethod2Label;
			loadImposterFileArena = Common::cmpResultArenaImposterMethod2Label;
			loadGenuineFileBoth = Common::cmpResultBothGenuineMethod2Label;
			loadImposterFileBoth = Common::cmpResultBothImposterMethod2Label;
			break;
		case FaceDivider::method3:
			loadGenuineFile = Common::cmpResultGenuineMethod3Label;
			loadImposterFile = Common::cmpResultImposterMethod3Label;
			loadGenuineFileArena = Common::cmpResultArenaGenuineMethod3Label;
			loadImposterFileArena = Common::cmpResultArenaImposterMethod3Label;
			loadGenuineFileBoth = Common::cmpResultBothGenuineMethod3Label;
			loadImposterFileBoth = Common::cmpResultBothImposterMethod3Label;
			break;
		case FaceDivider::method1rigid:
			loadGenuineFile = Common::cmpResultGenuineMethod1rLabel;
			loadImposterFile = Common::cmpResultImposterMethod1rLabel;
			loadGenuineFileArena = Common::cmpResultArenaGenuineMethod1rLabel;
			loadImposterFileArena = Common::cmpResultArenaImposterMethod1rLabel;
			loadGenuineFileBoth = Common::cmpResultBothGenuineMethod1rLabel;
			loadImposterFileBoth = Common::cmpResultBothImposterMethod1rLabel;
			break;
		case FaceDivider::method2rigid:
			loadGenuineFile = Common::cmpResultGenuineMethod2rLabel;
			loadImposterFile = Common::cmpResultImposterMethod2rLabel;
			loadGenuineFileArena = Common::cmpResultArenaGenuineMethod2rLabel;
			loadImposterFileArena = Common::cmpResultArenaImposterMethod2rLabel;
			loadGenuineFileBoth = Common::cmpResultBothGenuineMethod2rLabel;
			loadImposterFileBoth = Common::cmpResultBothImposterMethod2rLabel;
			break;
		case FaceDivider::method3rigid:
			loadGenuineFile = Common::cmpResultGenuineMethod3rLabel;
			loadImposterFile = Common::cmpResultImposterMethod3rLabel;
			loadGenuineFileArena = Common::cmpResultArenaGenuineMethod3rLabel;
			loadImposterFileArena = Common::cmpResultArenaImposterMethod3rLabel;
			loadGenuineFileBoth = Common::cmpResultBothGenuineMethod3rLabel;
			loadImposterFileBoth = Common::cmpResultBothImposterMethod3rLabel;
			break;
		default:
			throw std::runtime_error("compareFaces(): unknown divide method");;
	}

	cv::Mat genResult, impResult;

	if(_isEigenface && _isArena) {
		qDebug() << "eigenface & arena:";
		Common::loadCmpResult(impResult, loadImposterFileBoth);
		Common::loadCmpResult(genResult, loadGenuineFileBoth);
	} else if(_isEigenface) {
		qDebug() << "eigenface:";
		Common::loadCmpResult(impResult, loadImposterFile);
		Common::loadCmpResult(genResult, loadGenuineFile);
	} else if(_isArena) {
		qDebug() << "arena:";
		Common::loadCmpResult(impResult, loadImposterFileArena);
		Common::loadCmpResult(genResult, loadGenuineFileArena);
	}

	Stats stats(impResult,genResult);

	stats.computeEer();

	for(int i = 0; i < stats.statValuesVector.size(); i++) {

		//qDebug() << "==" << i << ":";
		//qDebug() << "min:" << stats.statValuesVector.at(i).min;
		//qDebug() << "max:" << stats.statValuesVector.at(i).max;
		//qDebug() << "err:" << stats.statValuesVector.at(i).eer;

		QString err;
		Stats::convertDotInNumber(stats.statValuesVector.at(i).eer,err);
		//qDebug("%s",err.toStdString().c_str());
	}

	//stats.computeHist(0,0,1.4);
	//stats.printHist(0);
	//stats.computeGraphFmrFnmr(0,0,1.3,0.01);

}

void Run::test_eigen() {
	EigenFace eigenface;

	eigenface.test();
}

