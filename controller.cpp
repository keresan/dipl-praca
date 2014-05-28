#include "controller.h"

Controller::Controller(){

}

/**
 * @brief Compute average landmarks position
 * @param faces Vector of faces
 * @param labels Labels of faces
 * @param avgLandmarks Computed average landmarks
 */
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
	}

	//compute average value
	for(int i = 0; i < landmarkVector.size(); i++) {
		if(landmarkVector[i].second)
		landmarkVector[i].first.x = qRound(landmarkVector.at(i).first.x / (double)counter);
		landmarkVector[i].first.y = qRound(landmarkVector.at(i).first.y / (double)counter);
	}
	avgLandmarks = Landmarks(landmarkVector);
}

/**
 * @brief Create PCA subspaces from sets of faces
 * @param faces Vector of faces
 * @param avgLandmarks Average landmarks
 * @param eigenface Created PCA subspaces
 * @param method Method to divide
 */
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
 * @brief Detect landmarks of depthmap
 * @param depthmap Depthmap
 * @param landmarks Detected landmarks
 * @return True if process of detection finish successful, otherwise false
 */
bool Controller::detectLandmarks(cv::Mat const &depthmap, Landmarks &landmarks) {
	LandmarkDetector detector(depthmap);
	bool result = detector.detectAll(landmarks);
	return result;
}

/**
 * @brief Detect landmarks, divide depthmap and project areas to PCA subspaces.
 * @param depthmap Input depthmap
 * @param label Label of depthmap
 * @param averageLandmarks Average landmarks
 * @param result Set to true if process succesful
 * @param method Divide method
 * @param subspaces PCA subspaces to project
 * @param featuresVector Vector of features
 * @param showBackProjections If True, back projection is show
 */
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

	//divide
	tFaceAreas areas;
	FaceDivider faceDivider(depthmap, landmarks, averageLandmarks);

	faceDivider.setResizeParam(Common::faceWidth);
	faceDivider.divide(method,areas);


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

/**
 * @brief Detect landmarks and  divide depthmap
 * @param depthmap Input depthmap
 * @param label Label of depthmap
 * @param averageLandmarks  Average landmarks
 * @param result Set to true if process succesful
 * @param method Divide method
 * @param featuresVector Vector of face areas
 */
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
		EigenFace::toRowMatrix(src,projection,depthmap.type());
		featuresVector.append(projection);

		//reshape
		//int rows = areas.at(i).rows;
		//cv::Mat back = projection.reshape(1,rows);
		//cv::imshow("reshape"+label.toStdString()+std::to_string(i), Common::norm_0_255(back));
	}

	result = true;
}

/**
 * @brief Align face and create depthmap
 * @param face Input mesh
 * @param modelFace Average face model
 * @param depthmap Created depthmap
 * @param distance Distance between align face and average face model
 * @param iterations Count of iterations to align face
 */
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


