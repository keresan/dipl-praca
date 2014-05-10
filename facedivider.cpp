#include "facedivider.h"

FaceDivider::FaceDivider(cv::Mat &depthMap, Landmarks &landmarks, Landmarks &averageLandmarks) {

	this->_depthMap = depthMap;
	this->_landmarks = landmarks;
	this->_averageLandmarks = averageLandmarks;



}

void FaceDivider::divide(DivideMethod method, tFaceAreas &areas) {
	switch(method) {
		case toHalfByNoseBottom:
			divideHalfByNoseBottom(areas);
		break;
		case method2:
			divideByMethod2(areas);
		break;
		case method3:
			divideByMethod3(areas);
		break;
		default:
			assert(false);
	}
}

void FaceDivider::divideHalfByNoseBottom(tFaceAreas &areas) {

	assert(_landmarks.is(Landmarks::NoseBottom));
	int mouthAreaHeight = _depthMap.rows - _averageLandmarks.pos(Landmarks::NoseBottom).y;
	int noseAreaHeight  = _averageLandmarks.pos(Landmarks::NoseBottom).y;

	cv::Point noseBottom = _landmarks.pos(Landmarks::NoseBottom);

	//qDebug() << "depthMap:" << _depthMap.rows << _depthMap.cols;


	cv::Mat upperArea = _depthMap(cv::Range(0,noseBottom.y), cv::Range::all()) ;

	//upperArea.copyTo(upperAreaCopied);

	cv::Mat mouthArea = _depthMap(cv::Range(noseBottom.y,_depthMap.rows), cv::Range::all());

	cv::Mat noseAreaScaled, mouthAreaScaled;

	resizeArea(upperArea, noseAreaScaled, _depthMap.cols, noseAreaHeight);
	resizeArea(mouthArea, mouthAreaScaled, _depthMap.cols,mouthAreaHeight);


	//qDebug() << 111;

	/*
	cv::imshow("upper area",Common::norm_0_255(upperArea));
	cv::imshow("mouth area",Common::norm_0_255(mouthArea));

	cv::imshow("upper area scaled",Common::norm_0_255(upperAreaScaled));
	cv::imshow("mouth area scaled",Common::norm_0_255(mouthAreaScaled));
	*/

	areas.append(noseAreaScaled);
	areas.append(mouthAreaScaled);

	//qDebug() << 222;

}

void FaceDivider::divideByMethod2(tFaceAreas &areas) {
	assert(_landmarks.is(Landmarks::NoseBottom));
	assert(_landmarks.is(Landmarks::NoseCornerLeft));
	assert(_landmarks.is(Landmarks::NoseCornerRight));
	assert(_landmarks.is(Landmarks::NoseRoot));
	assert(_landmarks.is(Landmarks::NoseTip));

	cv::Point noseMiddlePoint,noseMiddlePointAvg;
	noseMiddlePoint.x = qRound((_landmarks.pos(Landmarks::NoseTip).x + _landmarks.pos(Landmarks::NoseRoot).x) / 2.0);
	noseMiddlePoint.y = qRound((_landmarks.pos(Landmarks::NoseTip).y + _landmarks.pos(Landmarks::NoseRoot).y) / 2.0);

	noseMiddlePointAvg.x = qRound((_averageLandmarks.pos(Landmarks::NoseTip).x + _averageLandmarks.pos(Landmarks::NoseRoot).x) / 2.0);
	noseMiddlePointAvg.y = qRound((_averageLandmarks.pos(Landmarks::NoseTip).y + _averageLandmarks.pos(Landmarks::NoseRoot).y) / 2.0);

	cv::Mat eyesArea  = _depthMap(cv::Range(0, noseMiddlePoint.y),
									 cv::Range::all());

	cv::Mat noseArea = _depthMap(cv::Range(noseMiddlePoint.y, _landmarks.pos(Landmarks::NoseBottom).y),
									  cv::Range::all());

	cv::Mat mouthArea = _depthMap(cv::Range(_landmarks.pos(Landmarks::NoseBottom).y,_depthMap.rows),
								  cv::Range::all());

	int eyesAreaHeight = noseMiddlePointAvg.y;
	int noseAreaHeight =  _averageLandmarks.pos(Landmarks::NoseBottom).y - noseMiddlePointAvg.y;
	int mouthAreaHeight = _depthMap.rows - _averageLandmarks.pos(Landmarks::NoseBottom).y;

	cv::Mat eyesAreaScaled, noseAreaScaled, mouthAreaScaled;

	resizeArea(eyesArea, eyesAreaScaled, _depthMap.cols, eyesAreaHeight);
	resizeArea(noseArea, noseAreaScaled, _depthMap.cols, noseAreaHeight);
	resizeArea(mouthArea, mouthAreaScaled, _depthMap.cols, mouthAreaHeight);


	/*
	cv::imshow("eyesArea",Common::norm_0_255(eyesAreaScaled));
	cv::imshow("noseArea",Common::norm_0_255(noseAreaScaled));
	cv::imshow("mouthArea",Common::norm_0_255(mouthAreaScaled));
	*/

	areas.append(eyesAreaScaled);
	areas.append(noseAreaScaled);
	areas.append(mouthAreaScaled);


}

void FaceDivider::divideByMethod3(tFaceAreas &areas) {
	assert(_landmarks.is(Landmarks::NoseBottom));
	assert(_landmarks.is(Landmarks::NoseCornerLeft));
	assert(_landmarks.is(Landmarks::NoseCornerRight));
	assert(_landmarks.is(Landmarks::NoseRoot));
	assert(_landmarks.is(Landmarks::NoseTip));


	cv::Point noseMiddlePoint,noseMiddlePointAvg;
	noseMiddlePoint.x = qRound((_landmarks.pos(Landmarks::NoseTip).x + _landmarks.pos(Landmarks::NoseRoot).x) / 2.0);
	noseMiddlePoint.y = qRound((_landmarks.pos(Landmarks::NoseTip).y + _landmarks.pos(Landmarks::NoseRoot).y) / 2.0);


	noseMiddlePointAvg.x = qRound((_averageLandmarks.pos(Landmarks::NoseTip).x + _averageLandmarks.pos(Landmarks::NoseRoot).x) / 2.0);
	noseMiddlePointAvg.y = qRound((_averageLandmarks.pos(Landmarks::NoseTip).y + _averageLandmarks.pos(Landmarks::NoseRoot).y) / 2.0);


	//crop from depthmap

	cv::Mat leftEyeArea  = _depthMap(cv::Range(0, noseMiddlePoint.y),
									 cv::Range(0, noseMiddlePoint.x));

	cv::Mat rightEyeArea  = _depthMap(cv::Range(0, noseMiddlePoint.y),
									  cv::Range(noseMiddlePoint.x, _depthMap.cols));


	cv::Mat leftCheekArea = _depthMap(cv::Range(noseMiddlePoint.y, _landmarks.pos(Landmarks::NoseBottom).y),
									  cv::Range(0,_landmarks.pos(Landmarks::NoseCornerLeft).x));

	cv::Mat noseArea = _depthMap(cv::Range(noseMiddlePoint.y, _landmarks.pos(Landmarks::NoseBottom).y),
									  cv::Range(_landmarks.pos(Landmarks::NoseCornerLeft).x, _landmarks.pos(Landmarks::NoseCornerRight).x));

	cv::Mat rightCheekArea = _depthMap(cv::Range(noseMiddlePoint.y, _landmarks.pos(Landmarks::NoseBottom).y),
									  cv::Range(_landmarks.pos(Landmarks::NoseCornerRight).x, _depthMap.cols));



	cv::Mat mouthArea = _depthMap(cv::Range(_landmarks.pos(Landmarks::NoseBottom).y,_depthMap.rows),
								  cv::Range::all());

	int eyesAreaHeight = noseMiddlePointAvg.y;
	int leftEyeAreaWidth = noseMiddlePointAvg.x;
	int rightEyeAreaWidth = _depthMap.cols - noseMiddlePointAvg.x;

	int middleAreaHeight =  _averageLandmarks.pos(Landmarks::NoseBottom).y - noseMiddlePointAvg.y;
	int leftCheekAreaWidth = _averageLandmarks.pos(Landmarks::NoseCornerLeft).x;
	int rightCheekAreaWidth = _depthMap.cols - _averageLandmarks.pos(Landmarks::NoseCornerRight).x;
	int noseAreaWidth = _averageLandmarks.pos(Landmarks::NoseCornerRight).x - _averageLandmarks.pos(Landmarks::NoseCornerLeft).x;
	int mouthAreaHeight = _depthMap.rows - _averageLandmarks.pos(Landmarks::NoseBottom).y;

	cv::Mat leftEyeAreaScaled, rightEyeAreaScaled,  leftCheekAreaScaled, noseAreaScaled, rightCheekAreaScaled, mouthAreaScaled;

	resizeArea(leftEyeArea, leftEyeAreaScaled, leftEyeAreaWidth, eyesAreaHeight);
	resizeArea(rightEyeArea, rightEyeAreaScaled, rightEyeAreaWidth, eyesAreaHeight);

	resizeArea(leftCheekArea, leftCheekAreaScaled, leftCheekAreaWidth, middleAreaHeight);
	resizeArea(noseArea, noseAreaScaled, noseAreaWidth, middleAreaHeight);
	resizeArea(rightCheekArea, rightCheekAreaScaled, rightCheekAreaWidth, middleAreaHeight);
	resizeArea(mouthArea, mouthAreaScaled, _depthMap.cols, mouthAreaHeight);

	/*
	cv::imshow("leftEyeArea",Common::norm_0_255(leftEyeAreaScaled));
	cv::imshow("rightEyeArea",Common::norm_0_255(rightEyeAreaScaled));
	cv::imshow("leftCheekArea",Common::norm_0_255(leftCheekAreaScaled));
	cv::imshow("noseArea",Common::norm_0_255(noseAreaScaled));
	cv::imshow("rightCheekArea",Common::norm_0_255(rightCheekAreaScaled));
	cv::imshow("mouthArea",Common::norm_0_255(mouthAreaScaled));
	*/


	areas.append(leftEyeAreaScaled);
	areas.append(rightEyeAreaScaled);
	areas.append(leftCheekAreaScaled);
	areas.append(noseAreaScaled);
	areas.append(rightCheekAreaScaled);
	areas.append(mouthAreaScaled);

}

void FaceDivider::resizeArea(cv::Mat &src, cv::Mat &dst, int width, int height) {

	cv::resize(src,dst,cv::Size(width, height),0,0,cv::INTER_LINEAR);
}
