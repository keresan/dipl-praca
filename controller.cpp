#include "controller.h"

Controller::Controller(){

}

void Controller::averageLandmarks(QVector<cv::Mat> &faces, QStringList &labels, Landmarks &avgLandmarks) {

	VectorOfLandmarks landmarkVector = avgLandmarks.getLandmarks();
	int counter = 0;
	for(int i=0; i< faces.size();i++) {
		//qDebug() << i << ": actaul face: " << labels.at(i);

		LandmarkDetector detector(faces.at(i));
		Landmarks landmarks;

		bool result = detector.detectAll(landmarks);
		if(result) {
			counter++;
			QFileInfo fileInfo(labels.at(i));
			//cv::imshow(faceLabels.at(i).toStdString() + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), Common::norm_0_255(crop));
			//DepthMap::showAllLandmarks(faces.at(i), landmarks,fileInfo.baseName());

			//acumulate sum
			VectorOfLandmarks actualLandmarkVector = landmarks.getLandmarks();
			for(int i = 0; i < landmarkVector.size(); i++) {
				if( actualLandmarkVector.at(i).second) {
					landmarkVector[i].first += actualLandmarkVector.at(i).first;
					landmarkVector[i].second = true;
				}
			}
		} else {
			qDebug() << labels.at(i) << "skipped";
		}
		//Common::delay(1000);
	}

	//compute average value
	for(int i = 0; i < landmarkVector.size(); i++) {
		if(landmarkVector[i].second)
		landmarkVector[i].first.x = qRound(landmarkVector.at(i).first.x / (double)counter);
		landmarkVector[i].first.y = qRound(landmarkVector.at(i).first.y / (double)counter);
	}
	avgLandmarks = Landmarks(landmarkVector);
}

void Controller::createPcaSubspaces(QVector<cv::Mat> &faces, Landmarks &avgLandmarks, EigenFace &eigenface, FaceDivider::DivideMethod method) {

	VectorOfDivideFaces areasVector;

	for(int i=0; i< faces.size();i++) {

		//detect landmarks
		LandmarkDetector detector(faces.at(i));
		Landmarks landmarks;

		bool result = detector.detectAll(landmarks);
		if(result) {
			//divide face to areas
			tFaceAreas areas;
			FaceDivider faceDivider(faces[i],landmarks,avgLandmarks);
			faceDivider.setResizeParam(Common::faceWidth);

			faceDivider.divide(method, areas);

			areasVector.append(areas);
		} else {
			//qDebug() << labels.at(i) << "skipped";
			;
		}

	}
	//create subspaces
	eigenface.train(areasVector);

}

/**
 * @brief Controller::detectLandmarks - detect landmarks of particular depthmap
 * @param depthmap
 * @param landmarks
 * @return true - process of detection finish successful, else return false
 */
bool Controller::detectLandmarks(cv::Mat const &depthmap, Landmarks &landmarks) {
	LandmarkDetector detector(depthmap);
	bool result = detector.detectAll(landmarks);
	return result;
}

void Controller::processFace(cv::Mat &depthmap,
							 QString label,
							 Landmarks &averageLandmarks,
							 bool &result,
							 FaceDivider::DivideMethod method,
							 EigenFace &subspaces,
							 tFeatures &featuresVector,
							 bool showBackProjections) {


	if (depthmap.rows == 0 || depthmap.cols == 0) {
		throw std::runtime_error("processFace(): empty depthmap");
	}

	double min,max;
	cv::minMaxLoc(depthmap, &min, &max);

	if(min < Common::depthmapInitValue+2) {
		throw std::runtime_error("unknown values in depthmap");
	}

	//qDebug() << label << "min max:" << min << max;

	//detect landmarks
	LandmarkDetector detector(depthmap);
	Landmarks landmarks;

	bool res = detector.detectAll(landmarks);
	if(!res) {
		result = false;
		return;
	}

	//qDebug() << "noseTip value: " << depthmap.at<float>(landmarks.pos(Landmarks::NoseTip).y, landmarks.pos(Landmarks::NoseTip).x);


		//qDebug() << "depthmap: " << depthmap.rows << "x" << depthmap.cols;


	//divide
	tFaceAreas areas;
	FaceDivider faceDivider(depthmap, landmarks, averageLandmarks);

	faceDivider.setResizeParam(Common::faceWidth);
	faceDivider.divide(method,areas);


	/*
	for(int i =0; i < areas.size(); i++) {
		qDebug() << i << areas[i].rows << "x" << areas[i].cols;
		//cv::imshow(std::to_string(i), Common::norm_0_255(areas.at(i)));
	}
	*/


	//project to pca subspace
	subspaces.project(areas,featuresVector);


	//print feaures vector
	//qDebug() << label;
	for(int c = 0; c < featuresVector.cols; c++) {
		//qDebug("%3d: %10.2f",c, featuresVector.at<float>(0,c));
	}

	//show back projection of face areas

	if(showBackProjections) {
		DepthMap::showAllLandmarks(depthmap,landmarks,label);


		QVector<cv::Mat> back;
		subspaces.backProject(featuresVector, back);

		for(int i = 0; i < back.size(); i++) {
			//recontruct
			cv::Mat grayscale = Common::norm_0_255(back.at(i).reshape(1, areas.at(i).rows));
			cv::imshow(label.toStdString()+" "+std::to_string(i), grayscale);
		}
	}
	result = true;
}

void Controller::procesFaceArena(cv::Mat &depthmap,
								 QString label,
								 Landmarks &averageLandmarks,
								 bool &result,
								 FaceDivider::DivideMethod method,
								 QVector<cv::Mat>  &featuresVector) {

	if (depthmap.rows == 0 || depthmap.cols == 0) {
		throw std::runtime_error("processFace(): empty depthmap");
	}
	//detect landmarks
	LandmarkDetector detector(depthmap);
	Landmarks landmarks;

	bool res = detector.detectAll(landmarks);
	if(!res) {
		result = false;
		return;
	}

	//divide
	tFaceAreas areas;
	FaceDivider faceDivider(depthmap, landmarks, averageLandmarks);

	faceDivider.setResizeParam(Common::faceWidth);
	faceDivider.divide(method,areas);

	//create feature vector
	for(int i=0; i < areas.size(); i++) {
		cv::Mat projection;
		cv::Mat src = areas.at(i);
		//qDebug() << "area " << i << ":" << areas.at(i).rows << "x" <<  areas.at(i).cols;

		EigenFace::toRowMatrix(src,projection,depthmap.type());

		//qDebug() << "projection " << i << ":" << projection.rows << "x" <<  projection.cols;

		featuresVector.append(projection);

		//reshape
		//int rows = areas.at(i).rows;
		//cv::Mat back = projection.reshape(1,rows);
		//cv::imshow("reshape"+label.toStdString()+std::to_string(i), Common::norm_0_255(back));
	}

	result = true;
}


void Controller::createDepthmap(Mesh &face, Mesh &modelFace, cv::Mat &depthmap, double &distance, int &iterations) {

	//normalize
	FaceAligner aligner;
	aligner.align(face,
				  modelFace,
				  Common::alignerMaxIterations,
				  Common::alignerConvergentTreshold );

	distance = aligner.finalDistance;
	iterations = aligner.finalIterations;

	//crop unneccessary points - only for speed up
	cv::Point2d tl(Common::depthMapTL.x-5, Common::depthMapTL.y+5);
	cv::Point2d br(Common::depthMapBR.x+5, Common::depthMapBR.y-5);
	face.cropMe(tl, br);

	//depth map with default arguments
	DepthMap map(face);
	depthmap = map.depthMap;
}


