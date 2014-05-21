#include "landmarkdetector.h"

#include <QTime>

LandmarkDetector::LandmarkDetector(cv::Mat depthMap){
	_depthMap = depthMap;
}

/**
 * @brief LandmarkDetector::detectAll
 * @param landmarks
 * @return  True if no exeption during detection appears
 */
bool LandmarkDetector::detectAll(Landmarks &landmarks) {

	try {
		detectNoseTip();
		detectNoseCorners();
		detectNoseRoot();
		detectNoseBottom();
		//detectInnerEyeCorners();
	} catch(cv::Exception &e) {
		//const char* err_msg = e.what();
		//qDebug() << "cannot detect landmarks on" << faceLabels.at(i);
		return false;
	}

	landmarks.set(Landmarks::NoseTip,noseTip);
	landmarks.set(Landmarks::NoseCornerLeft, noseCornerLeft);
	landmarks.set(Landmarks::NoseCornerRight, noseCornerRight);
	landmarks.set(Landmarks::NoseRoot, noseRoot);
	landmarks.set(Landmarks::NoseBottom, noseBottom);
	//landmarks.set(Landmarks::EyeInnerCornerLeft, eyeInnerCornerLeft );
	//landmarks.set(Landmarks::EyeInnerCornerRight, eyeInnerCornerRight);

	return true;
}


void LandmarkDetector::detectNoseTip() {

	cv::Mat noseTipArea = _depthMap(cv::Range(Common::detectNoseTipAreaStartY,Common::detectNoseTipAreaStartY+Common::detectNoseTipAreaHeight),
									cv::Range::all());

	std::vector<float> profileCurveY(noseTipArea.rows);
	std::vector<float> profileCurveX(_depthMap.cols);

	//compute Y coord
	for(unsigned long i = 0; i < profileCurveY.size(); i++) {
		double max;
		cv::minMaxLoc(noseTipArea.row(i), NULL, &max);
		profileCurveY[i] = max;
	}

	//smooth
	cv::medianBlur(profileCurveY, profileCurveY,5);

	int indexMax[2];
	cv::minMaxIdx(profileCurveY,NULL,NULL,NULL,indexMax);
	noseTip.y = indexMax[1] + Common::detectNoseTipAreaStartY;

	//compute X coord

	//copy row with max value to vector
	for(int i = 0; i < _depthMap.cols; i++) {
		profileCurveX[i] = _depthMap.at<float>(noseTip.y,i);
	}

	cv::medianBlur(profileCurveX, profileCurveX,5);


	cv::minMaxIdx(profileCurveX,NULL,NULL,NULL,indexMax);
	noseTip.x = indexMax[1];


	cv::Mat normalizeMap = Common::norm_0_255(_depthMap);
	cv::Mat colorMap;
	cvtColor(normalizeMap, colorMap, CV_GRAY2RGB);
	int thickness = -1;
	int lineType = 8;
	float diameter = 0.5;

	/*
	int deltaY = Common::detectNoseTipAreaStartY;

	for(int i = 0; i < profileCurveX.size()-1; i++) {
		cv::line(colorMap,
				 cv::Point(i, qRound(profileCurveX[i])),
				 cv::Point(i+1,qRound(profileCurveX[i+1])),
				 cv::Scalar( 0, 255, 255 ),
				 1,
				 lineType
				);
	}
	for(int i = 0; i < profileCurveY.size()-1; i++) {

		cv::line(colorMap,
				 cv::Point(qRound(profileCurveY[i]),i+deltaY),
				 cv::Point(qRound(profileCurveY[i+1]),i+1+deltaY),
				 cv::Scalar( 0, 255, 0 ),
				 1,
				 lineType
				);
	}


	cv::circle(colorMap,noseTip, 2, cv::Scalar( 0, 0, 255 ), thickness, lineType );

	cv::imshow("nose tip "+ QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), colorMap);

	*/

}

void LandmarkDetector::detectInnerEyeCorners() {


	cv::Mat eyeArea = _depthMap(cv::Range(noseRoot.y-Common::detectEyeAreaHalfHeight, noseRoot.y+Common::detectEyeAreaHalfHeight),
									cv::Range(noseRoot.x-Common::detectEyeAreaWidth, noseRoot.x+Common::detectEyeAreaWidth) );




	std::vector<float> profileCurveX(eyeArea.cols);
	std::vector<float> profileCurveXgradient(eyeArea.cols);

	for(unsigned long i = 0; i < profileCurveX.size(); i++) {
		double min;
		cv::minMaxLoc(eyeArea.col(i), &min, NULL);
		profileCurveX[i] = min;
	}

	double max;
	cv::minMaxLoc(profileCurveX, NULL, &max);
	qDebug() << max;

	//smooth
	cv::medianBlur(profileCurveX, profileCurveX,5);


	//gradient
	cv::Sobel(profileCurveX, profileCurveXgradient,CV_32F,1,0,3,1,0,cv::BORDER_DEFAULT);

	cv::Mat normalizeMap = Common::norm_0_255(_depthMap);
	cv::Mat colorMap;
	cvtColor(normalizeMap, colorMap, CV_GRAY2RGB);
	int thickness = -1;
	int lineType = 8;
	float diameter = 2;

	/*
	int deltaX = noseRoot.x-Common::detectEyeAreaWidth;
	int deltaY = 40;

	cv::line(colorMap,
			 cv::Point(noseRoot.x-Common::detectEyeAreaWidth,noseRoot.y-Common::detectEyeAreaHalfHeight),
			 cv::Point(noseRoot.x-Common::detectEyeAreaWidth, noseRoot.y+Common::detectEyeAreaHalfHeight),
			 cv::Scalar( 255, 255, 255), //blue B G R
			 1,
			 lineType
			);
	cv::line(colorMap,
			 cv::Point(noseRoot.x+Common::detectEyeAreaWidth,noseRoot.y-Common::detectEyeAreaHalfHeight),
			 cv::Point(noseRoot.x+Common::detectEyeAreaWidth, noseRoot.y+Common::detectEyeAreaHalfHeight),
			 cv::Scalar( 255, 255, 255), //blue B G R
			 1,
			 lineType
			);


	cv::line(colorMap,
			 cv::Point(deltaX,deltaY),
			 cv::Point(noseTip.y, deltaY),
			 cv::Scalar( 0, 255, 0), //blue B G R
			 1,
			 lineType
			);

	for(int i = 0; i < profileCurveX.size()-1; i++) {
		cv::line(colorMap,
				 cv::Point(i+deltaX, qRound(profileCurveXgradient[i]*1)+deltaY),
				 cv::Point(i+1+deltaX, qRound(profileCurveXgradient[i+1]*1)+deltaY),
				 cv::Scalar( 255, 0, 0), //blue B G R
				 1,
				 lineType
				);


		cv::line(colorMap,
				 cv::Point(i+deltaX, qRound(profileCurveX[i])),
				 cv::Point(i+1+deltaX, qRound(profileCurveX[i+1])),
				 cv::Scalar( 0, 0, 255), //red B G R
				 1,
				 lineType
				);
	}
	cv::circle(colorMap,noseRoot, diameter, cv::Scalar( 0, 0, 255 ), thickness, lineType );
	cv::imshow("eye corners" + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), colorMap);

	*/
	/*
	cv::Mat rightEyeArea = _depthMap(cv::Range(noseRoot.y-Common::detectEyeAreaHalfHeight, noseRoot.y+Common::detectEyeAreaHalfHeight),
									cv::Range(noseRoot.x, noseRoot.x+Common::detectEyeAreaWidth) );

	cv::Mat leftEyeArea = _depthMap(cv::Range(noseRoot.y-Common::detectEyeAreaHalfHeight, noseRoot.y+Common::detectEyeAreaHalfHeight),
									cv::Range(noseRoot.x-Common::detectEyeAreaWidth, noseRoot.x) );




	int indexMin[2];
	//left eye inner corner
	cv::minMaxIdx(leftEyeArea,NULL,NULL,indexMin,NULL);
	eyeInnerCornerLeft.x = indexMin[1] + noseRoot.x - Common::detectEyeAreaWidth;
	eyeInnerCornerLeft.y = indexMin[0] + noseRoot.y - Common::detectEyeAreaHalfHeight;

	//right eye inner corner
	cv::minMaxIdx(rightEyeArea,NULL,NULL,indexMin,NULL);
	eyeInnerCornerRight.x = indexMin[1] + noseRoot.x;
	eyeInnerCornerRight.y = indexMin[0] + noseRoot.y - Common::detectEyeAreaHalfHeight;
	*/

	/*
	cv::Mat normalizeMap = Common::norm_0_255(_depthMap);
	cv::Mat colorMap;
	cvtColor(normalizeMap, colorMap, CV_GRAY2RGB);
	int thickness = -1;
	int lineType = 8;
	float diameter = 1;

	//center line
	cv::line(colorMap,
			 cv::Point(noseRoot.x, noseRoot.y-Common::detectEyeAreaHalfHeight),
			 cv::Point(noseRoot.x, noseRoot.y+Common::detectEyeAreaHalfHeight),
			 cv::Scalar( 255,255,255 )); //white

	//right line
	cv::line(colorMap,
			 cv::Point(noseRoot.x+Common::detectEyeAreaWidth, noseRoot.y-Common::detectEyeAreaHalfHeight),
			 cv::Point(noseRoot.x+Common::detectEyeAreaWidth, noseRoot.y+Common::detectEyeAreaHalfHeight),
			 cv::Scalar( 255,255,255 )); //white

	//left line
	cv::line(colorMap,
			 cv::Point(noseRoot.x-Common::detectEyeAreaWidth, noseRoot.y-Common::detectEyeAreaHalfHeight),
			 cv::Point(noseRoot.x-Common::detectEyeAreaWidth, noseRoot.y+Common::detectEyeAreaHalfHeight),
			 cv::Scalar( 255,255,255 )); //white


	//cv::imshow("left eye area "+QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), Common::norm_0_255(leftEyeArea));


	cv::circle(colorMap,eyeInnerCornerLeft, diameter, cv::Scalar( 0, 0, 255 ), thickness, lineType );
	cv::circle(colorMap,eyeInnerCornerRight, diameter, cv::Scalar( 255, 0, 0 ), thickness, lineType );

	cv::imshow("eye inner corner" + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), colorMap);
	*/

}

/**
 * @brief LandmarkDetector::detecNasalBridge
 * @return
 * urobit verikalnu profilovu krivku od spicky nosa +- 30px v horizontalnej ose - ziskame tym profil nosa
 * nosal bridge musi byt konvexny
 */

void LandmarkDetector::detectNoseRoot() {


	cv::Mat noseArea = _depthMap(cv::Range(0, noseTip.y+Common::detectNoseRootAreaHalfWidth),
								 cv::Range(noseTip.x-Common::detectNoseRootAreaHalfWidth, noseTip.x+Common::detectNoseRootAreaHalfWidth) );


	//cv::line(colorMap,cv::Point(_noseTip.x-30,0),cv::Point(_noseTip.x-30,_noseTip.y+10), cv::Scalar( 255, 255, 255 ), 1, lineType);
	//cv::line(colorMap,cv::Point(_noseTip.x+30,0),cv::Point(_noseTip.x+30,_noseTip.y+10), cv::Scalar( 255, 255, 255 ), 1, lineType);

	std::vector<float> profileCurveY(noseArea.rows);
	std::vector<float> profileCurveYgradient(noseArea.rows);
	std::vector<float> profileCurveX(noseArea.cols);

	for(unsigned long i = 0; i < profileCurveY.size(); i++) {
		double max;
		cv::minMaxLoc(noseArea.row(i), NULL, &max);
		profileCurveY[i] = max;
	}

	//smooth
	cv::medianBlur(profileCurveY, profileCurveY,5);

	//profileCurveY[51] = 201;
	//profileCurveY[52] = 213;
	//profileCurveY[53] = 213;
	//profileCurveY[54] = 213;
	//median bluer s ksize 5 je schopny napravit az 2 chybne pixeli po sebe


	//gradient
	cv::Sobel(profileCurveY, profileCurveYgradient,CV_32F,1,0,3,1,0,cv::BORDER_DEFAULT);

	//detect first negative gradient from nose tip
	for(int i = noseTip.y-Common::detectNoseRootMinDistanceFromTip; i > 0; i--) {
		if(profileCurveYgradient[i] < 0) {
			noseRoot.y = i;
			break;
		}
	}

	//X coord

	//copy
	for(int i = 0; i < noseArea.cols; i++) {
		profileCurveX[i] = noseArea.at<float>(noseRoot.y,i);
	}

	//smooth
	cv::medianBlur(profileCurveX, profileCurveX,5);

	//get max
	int indexMax[2];
	cv::minMaxIdx(profileCurveX,NULL,NULL,NULL,indexMax);
	noseRoot.x = indexMax[1] + noseTip.x-Common::detectNoseRootAreaHalfWidth;


	/*
	cv::Mat normalizeMap = Common::norm_0_255(_depthMap);
	cv::Mat colorMap;
	cvtColor(normalizeMap, colorMap, CV_GRAY2RGB);
	int thickness = -1;
	int lineType = 8;
	float diameter = 2;

	int deltaX = 90;

	cv::line(colorMap,
			 cv::Point(deltaX,0),
			 cv::Point(deltaX, noseTip.y),
			 cv::Scalar( 0, 255, 0), //blue B G R
			 1,
			 lineType
			);

	for(int i = 0; i < profileCurveY.size()-1; i++) {
		cv::line(colorMap,
				 cv::Point(qRound(profileCurveYgradient[i]*1)+deltaX,i),
				 cv::Point(qRound(profileCurveYgradient[i+1]*1)+deltaX, i+1),
				 cv::Scalar( 255, 0, 0), //blue B G R
				 1,
				 lineType
				);

		//qDebug() << profileCurveYgradient.at(i);
		cv::line(colorMap,
				 cv::Point(qRound(profileCurveY[i])+5,i),
				 cv::Point(qRound(profileCurveY[i+1])+5, i+1),
				 cv::Scalar( 0, 0, 255), //red B G R
				 1,
				 lineType
				);
	}

	cv::circle(colorMap,noseRoot, diameter, cv::Scalar( 0, 0, 255 ), thickness, lineType );
	cv::imshow("nose root" + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), colorMap);
	*/

}

void LandmarkDetector::detectNoseBottom() {


	cv::Mat noseArea = _depthMap(cv::Range(noseTip.y, noseTip.y+Common::detectNoseBottomAreaHeight),
								 cv::Range(noseTip.x-Common::detectNoseBottomAreaHalfWidth, noseTip.x+Common::detectNoseBottomAreaHalfWidth) );


	std::vector<float> profileCurveY(noseArea.rows);
	std::vector<float> profileCurveYgradient(noseArea.rows);

	for(unsigned long i = 0; i < profileCurveY.size(); i++) {
		double max;
		cv::minMaxLoc(noseArea.row(i), NULL, &max);
		profileCurveY[i] = max;
	}

	//smooth
	cv::medianBlur(profileCurveY, profileCurveY,5);
	//gradient
	cv::Sobel(profileCurveY, profileCurveYgradient,CV_32F,1,0,3,1,0,cv::BORDER_DEFAULT);

	//coord Y = minimum gradient value
	int index[2];
	cv::minMaxIdx(profileCurveYgradient,NULL,NULL,index,NULL);
	//qDebug() << "old index: " << index[1];
	for(unsigned long i= index[1]; i < profileCurveYgradient.size(); i++) {
		if(profileCurveYgradient[i] > -Common::detectHightPassFilterValue) {
			noseBottom.y = i+noseTip.y;
			//qDebug() << "new index: " << i;
			break;
		}
	}


	//coord X - maximum profile gradient
	cv::minMaxIdx(noseArea.row(index[1]),NULL,NULL,NULL,index);
	noseBottom.x = index[1] + noseTip.x-Common::detectNoseBottomAreaHalfWidth;

	/*
	int deltaX = 180;

	cv::Mat normalizeMap = Common::norm_0_255(_depthMap);
	cv::Mat colorMap;
	cvtColor(normalizeMap, colorMap, CV_GRAY2RGB);
	int thickness = -1;
	int lineType = 8;
	float diameter = 0.5;

	//stred filter
	cv::line(colorMap,
			 cv::Point(deltaX, noseTip.y),
			 cv::Point(deltaX, noseTip.y+Common::detectNoseBottomAreaHeight),
			 cv::Scalar( 0,255,0 )); //green

	//zaporny filter
	cv::line(colorMap,
			 cv::Point(deltaX-Common::detectHightPassFilterValue, noseTip.y),
			 cv::Point(deltaX-Common::detectHightPassFilterValue, noseTip.y+Common::detectNoseBottomAreaHeight),
			 cv::Scalar( 0,255,0 )); //green



	for(int i = 0; i < profileCurveYgradient.size()-1; i++) {
		cv::line(colorMap,
				 cv::Point(qRound(profileCurveYgradient[i])+deltaX,i+noseTip.y),
				 cv::Point(qRound(profileCurveYgradient[i+1])+deltaX, i+1+noseTip.y),
				 cv::Scalar( 255, 0, 0), //blue B G R
				 1,
				 lineType
				);

		cv::circle(colorMap,cv::Point(qRound(profileCurveY[i]), i+noseTip.y), diameter, cv::Scalar( 255, 0, 255 ), thickness, lineType );
	}

	cv::circle(colorMap,noseBottom, 2, cv::Scalar( 0, 0, 255 ), thickness, lineType );

	cv::imshow("nose bottom" + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), colorMap);

	*/
}

void LandmarkDetector::detectNoseCorners() {


	//compute x profile curve
	//podla rozmerov sa potom rata ja finalna poloha nosu !!

	cv::Mat noseCornersArea = _depthMap(cv::Range(noseTip.y-Common::detectNoseCornersAreaHalfHeight, noseTip.y+Common::detectNoseCornersAreaHalfHeight),
										cv::Range(noseTip.x-Common::detectNoseCornersAreaHalfWidth, noseTip.x+Common::detectNoseCornersAreaHalfWidth));

	//cv::imshow("nose corners area",Common::norm_0_255(noseCornersArea));

	std::vector<float> profileCurveX(noseCornersArea.cols);
	for(unsigned long i = 0; i < profileCurveX.size(); i++) {
		double  max;
		cv::minMaxLoc(noseCornersArea.col(i), NULL, &max);
		profileCurveX[i] = max;
		//cv::circle(colorMap,cv::Point(i+noseTip.x-Common::detectorNoseCornersAreaHalfWidth, qRound(max)), 0.5, cv::Scalar( 0, 255, 255 ), thickness, lineType );
	}
	//median blur
	cv::medianBlur(profileCurveX, profileCurveX,5);

	//compute gradient
	std::vector<float> profileCurveXgradient(profileCurveX.size());
	cv::Sobel(profileCurveX, profileCurveXgradient,CV_32F,1,0,3,1,0,cv::BORDER_DEFAULT);


	int index[2];
	//left corner = index of maximum value
	cv::minMaxIdx(profileCurveXgradient,NULL,NULL,NULL,index);
	//outerNoseCornerLeft.x = index[1]+noseTip.x-Common::detectorNoseCornersAreaHalfWidth;

	//qDebug() << "old index: " << index[1];
	for(int i= index[1]; i > 0; i--) {
		if(profileCurveXgradient[i] < Common::detectHightPassFilterValue) {
			noseCornerLeft.x = i+noseTip.x-Common::detectNoseCornersAreaHalfWidth;
			noseCornerLeft.y = noseTip.y;
			//qDebug() << "new index: " << i;
			break;
		}
	}


	//right corner = index of minimum value
	cv::minMaxIdx(profileCurveXgradient,NULL,NULL,index,NULL);
	noseCornerRight.x = index[1]+noseTip.x-Common::detectNoseCornersAreaHalfWidth;

	//qDebug() << "old index: " << index[1];
	for(unsigned long i= index[1]; i < profileCurveXgradient.size(); i++) {
		if(profileCurveXgradient[i] > -Common::detectHightPassFilterValue) {
			noseCornerRight.x = i+noseTip.x-Common::detectNoseCornersAreaHalfWidth;
			noseCornerRight.y = noseTip.y;
			//qDebug() << "new index: " << i;
			break;
		}
	}

	/*
	cv::Mat normalizeMap = Common::norm_0_255(_depthMap);
	cv::Mat colorMap;
	cvtColor(normalizeMap, colorMap, CV_GRAY2RGB);
	int thickness = -1;
	int lineType = 8;
	float diameter = 2;
	int deltaX = 100;

	cv::line(colorMap,
			 cv::Point(noseTip.x-Common::detectNoseCornersAreaHalfWidth, deltaX),
			 cv::Point(noseTip.x+Common::detectNoseCornersAreaHalfWidth, deltaX),
			 cv::Scalar( 0,255,0 )); //green

	//stred
	cv::line(colorMap,
			 cv::Point(noseTip.x-Common::detectNoseCornersAreaHalfWidth, deltaX),
			 cv::Point(noseTip.x+Common::detectNoseCornersAreaHalfWidth, deltaX),
			 cv::Scalar( 0,255,0 )); //green

	//kladny filter
	cv::line(colorMap,
			 cv::Point(noseTip.x-Common::detectNoseCornersAreaHalfWidth, deltaX+Common::detectHightPassFilterValue),
			 cv::Point(noseTip.x+Common::detectNoseCornersAreaHalfWidth, deltaX+Common::detectHightPassFilterValue),
			 cv::Scalar( 0,255,0 )); //green

	//zaporny filter
	cv::line(colorMap,
			 cv::Point(noseTip.x-Common::detectNoseCornersAreaHalfWidth, deltaX-Common::detectHightPassFilterValue),
			 cv::Point(noseTip.x+Common::detectNoseCornersAreaHalfWidth, deltaX-Common::detectHightPassFilterValue),
			 cv::Scalar( 0,255,0 )); //green


	for(int i = 0; i < profileCurveXgradient.size()-1; i++) {
		cv::line(colorMap,
				 cv::Point(i+noseTip.x-Common::detectNoseCornersAreaHalfWidth, qRound(profileCurveXgradient[i])+deltaX),
				 cv::Point(i+1+noseTip.x-Common::detectNoseCornersAreaHalfWidth, qRound(profileCurveXgradient[i+1])+deltaX),
				 cv::Scalar( 255, 0, 0 ),
				 1,
				 lineType
				);

		cv::circle(colorMap,cv::Point(i+noseTip.x-Common::detectNoseCornersAreaHalfWidth, qRound(profileCurveX[i])), 0.5, cv::Scalar( 0, 255, 255 ), thickness, lineType );

		qDebug() << profileCurveXgradient[i];
	}

	cv::line(colorMap,
			 cv::Point(noseCornerLeft.x, noseTip.y-20),
			 cv::Point(noseCornerLeft.x, noseTip.y+20),
			 cv::Scalar( 0,255,0 )); //green
	cv::line(colorMap,
			 cv::Point(noseCornerRight.x, noseTip.y-20),
			 cv::Point(noseCornerRight.x, noseTip.y+20),
			 cv::Scalar( 255,0,0 )); //blue

	cv::imshow("nose corners"+QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), colorMap);


	*/
}

void  LandmarkDetector::derivateVector(std::vector<float> &src, std::vector<float> &dst) {
	for(unsigned long i = 0; i < src.size() - 1; i++) {
		dst[i] = (src[i+1] - src[i])*10;
		//qDebug() << i << ":" << dst[i];
	}
	dst.back() = src.back();
}

void LandmarkDetector::averageBlur(std::vector<float> &src, std::vector<float> &dst, int ksize) {

	int kHalfSize = ksize / 2;
	std::vector<float> tmpDst(src.size());

	qDebug() << ksize << "-->" << kHalfSize;
	for(unsigned long i  = 0; i < src.size(); i++) {

		float average = 0;
		int counter = 0;
		//qDebug() << "i: " << i << "j=" << i - ksize << "az" << i+ ksize;
		for(int j = i - kHalfSize; j <= i+ kHalfSize; j++) {
			if(j >= 0 && j < src.size()) {
				//qDebug() << "j=" << j <<"=" << src[j];
				average += src[j];
				counter++;
			}
		}


		//qDebug() << "counter=" << counter;
		average /= (float)counter;
		//qDebug() << i <<" average: " << average;

		tmpDst[i] = average;
	}

	//copy tmpDst to dst
	dst = tmpDst;

}


bool LandmarkDetector::checkLandmarks(Landmarks &srcLandmarks, Landmarks &refLandmarks) {
	VectorOfLandmarks src = srcLandmarks.getLandmarks();
	VectorOfLandmarks ref =  refLandmarks.getLandmarks();

	assert(src.size() == ref.size());

	for(int i =0; i < src.size(); i++) {

		if(src[i].second ) {
			//qDebug() << i << "distance x:" << qAbs(src[i].first.x - ref[i].first.x);
			//qDebug() << i << "distance y:" << qAbs(src[i].first.y - ref[i].first.y);

			if(qAbs(src[i].first.x - ref[i].first.x) > Common::lmDeltaFromAvg) {
				return false;
			}

			if(qAbs(src[i].first.y - ref[i].first.y) > Common::lmDeltaFromAvg) {
				return false;
			}
		}

	}
	return true;
}
