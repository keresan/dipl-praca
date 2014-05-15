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

	Mesh *modelFace = new Mesh("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final_norm_4_4.obj", false);

	/*
	facePaths.append(Common::pathToFall2003+"04287d49.abs");
	facePaths.append(Common::pathToFall2003+"04287d51.abs");
	facePaths.append(Common::pathToFall2003+"04287d53.abs");
	facePaths.append(Common::pathToFall2003+"04395d200.abs");
	//facePaths.append(Common::pathToAverageFace);
	facePaths.append("/Users/martin/Documents/[]sklad/frgc_data/averageFace/averageFace_final.obj");
	*/
	Common::loadFilesPathFromDir(Common::pathToFall2003,facePaths,QStringList("*abs"));

	int indexStart = 500;
	int indexStop = 1000;

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

		if(distance > Common::alignerDistanceTresholdToContinue) {
			qDebug() << "wrong depthmap: " << fileInfo.fileName();

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

	//get list of files

	Common::loadFilesPathFromDir(Common::pathToDepthmapF2003,facePaths);

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

	double distance;
	int iterations;

	for(int i =0; i < facePaths.count(); i++) {
		QFileInfo fileInfo(facePaths.at(i));

		cv::Mat depthmap;
		Common::loadDepthmap(fileInfo.fileName(),fileInfo.path(),depthmap,distance,iterations);


		/*
		double min,max;
		cv::minMaxLoc(depthmap, &min, &max);
		qDebug() << fileInfo.fileName() << min << max;
		*/
		if(distance > Common::alignerDistanceTresholdToContinue) {
			qDebug() << "wrong: " << fileInfo.fileName();

			QFile::rename(fileInfo.absoluteFilePath(), fileInfo.absoluteFilePath()+Common::wrongDpSuffixLabel);
			continue;
		}
		cv::Mat crop = depthmap(Common::faceCropArea);

		cv::imshow(fileInfo.baseName().toStdString(), Common::norm_0_255(crop));

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
	facePaths.append(Common::pathToDepthmapF2003+"04299d193.xml");
	//facePaths.append(Common::pathToDepthmapF2003+"04265d267.xml_wrong_lm");


	//Common::loadFilesPathFromDir(Common::pathToWarehouse+"depthmap_tmp2/",facePaths);

	//load average face
	cv::Mat averageFace;
	double distance;
	int iterations;
	QString path = Common::pathToAverageDepthmap;
	QFileInfo fileInfo(path);
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),averageFace,averageFace,distance,iterations);

	for(int i =0; i < facePaths.size(); i += 1) {
		cv::Mat depthmap;
		double distance;
		qDebug() << "actual file:" << facePaths.at(i);

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

		} else {
			qDebug() << facePaths.at(i) << "skipped";
		}

		Common::delay(500);

	}
}
void Run::init() {

	Controller controller;
	QVector<cv::Mat> faces;

	QStringList facePaths;
	QStringList labels;


	Common::loadFilesPathFromDir(Common::pathToDepthmapF2003,facePaths);

	//prepare file list
	while(facePaths.size() > 200) {
		facePaths.removeLast();
	}

	//load average face
	cv::Mat averageFace;
	double distance;
	int iterations;
	QString path = Common::pathToAverageDepthmap;
	QFileInfo fileInfo(path);
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(),averageFace,averageFace,distance,iterations);

	qDebug() << averageFace.rows << "x" << averageFace.cols;

	qDebug() << "facePaths.size():" << facePaths.size();
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

	qDebug() << "letc make avg landmarks";
	//load landmakrs
	Landmarks avgLandmarks(Common::lmPathToLmDir+Common::lmAvgLmLabel);

	//compute and save average landmarks
	//Landmarks avgLandmarks;
	//controller.averageLandmarks(faces, labels, avgLandmarks);
	//avgLandmarks.save(Common::lmAvgLmLabel);

	qDebug() << "letc make subspaces";

	//compute and save pca subspaces
	EigenFace eigenface;
	controller.createPcaSubspaces(faces,avgLandmarks,eigenface,FaceDivider::method1);
	eigenface.saveSubspaces(Common::eigenMethot1Label);

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
			//cv::imshow(faceLabels.at(i).toStdString() + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), Common::norm_0_255(crop));


			//divide face
			tFaceAreas areas;
			FaceDivider faceDivider(crop,landmarks,averageLandmakrks);
			faceDivider.divide(FaceDivider::method2,areas);

		} else {
			qDebug() << faceLabels.at(i) << "skipped";
		}
	}

}

void Run::processFace() {
	bool result;

	//load average landmarks
	Landmarks avgLandmarks(Common::lmAvgLmLabel);

	//load pca subspace
	EigenFace eigenface;
	eigenface.loadSubspaces(Common::eigenMethot1Label);

	cv::Mat depthmap;
	double distance;
	int iterations;

	//load average face
	cv::Mat averageFace;
	QString pathToAverage = Common::pathToWarehouse+"depthmap_tmp2/"+"averageFace_final.xml";
	QFileInfo fileInfo1(pathToAverage);
	Common::loadDepthmapProcess(fileInfo1.fileName(),fileInfo1.path(),averageFace,averageFace,distance,iterations);


	//load face
	QString path = Common::pathToDepthmapF2003+"04395d202.xml";

	QFileInfo fileInfo(path);
	Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.filePath(),depthmap,averageFace,distance,iterations);

	Controller controller;
	tFeatures featuresVector;
	controller.processFace(depthmap, "04397d348",avgLandmarks,result,FaceDivider::method1, eigenface, featuresVector);


}

void Run::compareFaces() {

	bool result;

	//load average landmarks
	Landmarks avgLandmarks(Common::pathToAverageLm);

	//load pca subspace
	EigenFace eigenface;
	eigenface.loadSubspaces(Common::eigenMethot1Label);

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


	/*
	facePaths.append(Common::pathToDepthmapF2003+"04287d49.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04287d53.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04287d51.xml");
	facePaths.append(Common::pathToDepthmapF2003+"04395d200.xml");
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

	int indexStart = 200;
	int indexStop = 600;

	for(int i =0; i < facePaths.size(); i++) {
		tFeatures features;

		if(i < indexStart) {
			continue;
		} else if(i > indexStop) {
			break;
		}

		//QString path = Common::pathToDepthmapF2003+facePaths.at(i)+".xml";
		//QString path =Common::pathToWarehouse+"depthmap_tmp2/"+facePaths.at(i)+".xml";


		QFileInfo fileInfo(facePaths.at(i));

		//qDebug() << fileInfo.absoluteFilePath();

		Common::loadDepthmapProcess(fileInfo.fileName(),fileInfo.path(), depthmap,averageFace,distance,iterations);

		//qDebug() << facePaths.at(i) << depthmap.rows << "x" << depthmap.cols;

		controller.processFace(depthmap,
							   fileInfo.baseName(),
							   avgLandmarks,
							   result,
							   FaceDivider::method1,
							   eigenface,
							   features);
		if(result) {
			qDebug() << "ERROR:" << facePaths.at(i);
		}

		//qDebug() << "features.rows: " << features.rows;

		featuresVector.append(features);
		labels.append(fileInfo.baseName());
	}
	qDebug() << "featuresVector.size() a.k.a pocet vstupnych tvari:" << featuresVector.size();
	//qDebug() << "labels.size():" << labels.size();


	QVector<QVector<float> > diffResult, identicResult;
	//cv::Mat	identicResult  = cv::Mat::ones(0, 6, CV_64F);

	int counter = 0;
	//compare each with each
	for(int i = 0; i < featuresVector.size() ; i++) {
		for(int j = i+1; j < featuresVector.size(); j++) {
			//compare features
			bool isIdentical = Common::isIdenticalPerson(labels.at(i), labels.at(j));
			qDebug() << "===" << labels.at(i) << "vs" << labels.at(j) << ":" << isIdentical;
			QVector<float> distances;

			Comparator::compare(featuresVector[i],featuresVector[j],distances, Comparator::EuclidianDistance);


			for(int k = 0; k < distances.size(); k++ ) {
				qDebug() << k << ":" << distances.at(k);
			}

			//add result
			if(isIdentical) {
				identicResult.append(distances);
			} else {
				diffResult.append(distances);
			}

			counter++;
		}
	}
	qDebug() << "pocet porovnani: " << counter;



	//convert to matrix
	cv::Mat matToSaveIdentic(identicResult.size(), identicResult.first().size(), CV_32F);
	for(int i=0; i<matToSaveIdentic.rows; i++)
		 for(int j=0; j<matToSaveIdentic.cols; j++)
			  matToSaveIdentic.at<float>(i, j) = identicResult.at(i).at(j);

	cv::Mat matToSaveDiff(diffResult.size(), diffResult.first().size(), CV_32F );
	for(int i=0; i<matToSaveDiff.rows; i++)
		 for(int j=0; j<matToSaveDiff.cols; j++)
			  matToSaveDiff.at<float>(i, j) = diffResult.at(i).at(j);

	qDebug() << "matToSaveIdentic" << matToSaveIdentic.rows << "x" << matToSaveIdentic.cols;
	qDebug() << "matToSaveDiff" << matToSaveDiff.rows << "x" << matToSaveDiff.cols;

	//save it
	Common::saveCmpResult(Common::cmpResultDiffMethod1Label,Common::pathToComResultDir,matToSaveDiff);
	Common::saveCmpResult(Common::cmpResultIdenticMethod1Label,Common::pathToComResultDir,matToSaveIdentic);


}


void Run::histogram() {

	cv::Mat identicResult, diffResult;

	Common::loadCmpResult(Common::cmpResultDiffMethod1Label,Common::pathToComResultDir,diffResult);
	Common::loadCmpResult(Common::cmpResultIdenticMethod1Label,Common::pathToComResultDir,identicResult);

	cv::Mat meanMinMax;

	qDebug() << "diffResult: " << diffResult.rows << "x" << diffResult.cols;

	Stats::computeMinMaxAvg(diffResult, meanMinMax);


	qDebug() << "meanMinMax: " << meanMinMax.rows << "x" << meanMinMax.cols;

	for(int c = 0; c < meanMinMax.cols; c++) {
		float mean = meanMinMax.at<float>(0,c);
		float min = meanMinMax.at<float>(1,c);
		float max = meanMinMax.at<float>(2,c);

		qDebug("%d: %6.2f %6.2f %6.2f",c,mean,min,max);
	}

	cv::Mat inputImposer = diffResult.col(0);
	cv::Mat inputGenuine = identicResult.col(0);
	cv::MatND histImposer, histGenuine;
	int channels[] = {0};
	int hbins = 100;
	int sbins = 50;
	//int histSize[] = {hbins, sbins};
	int histSize = 100;


	float hranges[] = {200,4500};
	float sranges[] = {0,0};
	const float* ranges[] = {hranges, sranges};


	cv::calcHist(&inputImposer,1,channels,cv::Mat(),histImposer,1,&histSize,ranges,true,false);
	cv::calcHist(&inputGenuine,1,channels,cv::Mat(),histGenuine,1,&histSize,ranges,true,false);


	for(int i = 0; i < histGenuine.rows; i++) {
		qDebug("%.0f\t%.0f", histGenuine.at<float>(i,0), histImposer.at<float>(i,0));
		;
	}


}
