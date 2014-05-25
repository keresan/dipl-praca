#include "facedivider.h"

FaceDivider::FaceDivider(cv::Mat &depthMap, Landmarks &landmarks, Landmarks &averageLandmarks) {

	this->_depthmap = depthMap;
	this->_landmarks = landmarks;
	this->_averageLandmarks = averageLandmarks;
}

void FaceDivider::divide(DivideMethod method, tFaceAreas &areas) {
	switch(method) {
		case method0:
			divideByMethod0(areas);
		break;
		case method1:
			divideByMethod1(areas);
		break;
		case method2:
			divideByMethod2(areas);
		break;
		case method3:
			divideByMethod3(areas);
		break;
		case method1rigid:
			divideByMethod1rigid(areas);
		break;
		case method2rigid:
			divideByMethod2rigid(areas);
		break;
		case method3rigid:
			divideByMethod3rigid(areas);
		break;
		default:
			throw std::runtime_error("divide(): unknown divide method");
	}
}

void FaceDivider::setResizeParam(int finalWidth) {
	//compute scale factor by width
	float scaleFactor = (float)finalWidth / _depthmap.cols;

	//qDebug() << _depthmap.cols << "->"<< finalWidth << ", scaleFactorX:" << scaleFactor;

	_averageLandmarks.scale(scaleFactor);
	_landmarks.scale(scaleFactor);


	cv::Size size;
	cv::resize(_depthmap,_depthmap,size,scaleFactor,scaleFactor,cv::INTER_LINEAR);
}



void FaceDivider::divideByMethod0(tFaceAreas &areas) {
	cv::Mat face;
	_depthmap.copyTo(face);

	areas.append(face);
}

void FaceDivider::divideByMethod1(tFaceAreas &areas) {

	assert(_landmarks.is(Landmarks::NoseBottom));
	int mouthAreaHeight = _depthmap.rows - _averageLandmarks.pos(Landmarks::NoseBottom).y;
	int noseAreaHeight  = _averageLandmarks.pos(Landmarks::NoseBottom).y;

	cv::Point noseBottom = _landmarks.pos(Landmarks::NoseBottom);


	cv::Mat noseArea = _depthmap(cv::Range(0,noseBottom.y), cv::Range::all()) ;

	cv::Mat mouthArea = _depthmap(cv::Range(noseBottom.y,_depthmap.rows), cv::Range::all());

	cv::Mat noseAreaScaled, mouthAreaScaled;

	resizeArea(noseArea, noseAreaScaled, _depthmap.cols, noseAreaHeight);
	resizeArea(mouthArea, mouthAreaScaled, _depthmap.cols,mouthAreaHeight);

	/*
	cv::imshow("upper area scaled",Common::norm_0_255(noseAreaScaled));
	cv::imshow("mouth area scaled",Common::norm_0_255(mouthAreaScaled));
	*/

	areas.append(noseAreaScaled);
	areas.append(mouthAreaScaled);

}

void FaceDivider::divideByMethod1rigid(tFaceAreas &areas) {
	int divideYcoord = _depthmap.rows / 2;

	cv::Mat noseArea = _depthmap(cv::Range(0,divideYcoord), cv::Range::all()) ;
	cv::Mat mouthArea = _depthmap(cv::Range(divideYcoord,_depthmap.rows), cv::Range::all());

	areas.append(noseArea);
	areas.append(mouthArea);
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

	cv::Mat eyesArea  = _depthmap(cv::Range(0, noseMiddlePoint.y),
									 cv::Range::all());

	cv::Mat noseArea = _depthmap(cv::Range(noseMiddlePoint.y, _landmarks.pos(Landmarks::NoseBottom).y),
									  cv::Range::all());

	cv::Mat mouthArea = _depthmap(cv::Range(_landmarks.pos(Landmarks::NoseBottom).y,_depthmap.rows),
								  cv::Range::all());

	int eyesAreaHeight = noseMiddlePointAvg.y;
	int noseAreaHeight =  _averageLandmarks.pos(Landmarks::NoseBottom).y - noseMiddlePointAvg.y;
	int mouthAreaHeight = _depthmap.rows - _averageLandmarks.pos(Landmarks::NoseBottom).y;

	cv::Mat eyesAreaScaled, noseAreaScaled, mouthAreaScaled;

	resizeArea(eyesArea, eyesAreaScaled, _depthmap.cols, eyesAreaHeight);
	resizeArea(noseArea, noseAreaScaled, _depthmap.cols, noseAreaHeight);
	resizeArea(mouthArea, mouthAreaScaled, _depthmap.cols, mouthAreaHeight);


	/*
	cv::imshow("eyesArea",Common::norm_0_255(eyesAreaScaled));
	cv::imshow("noseArea",Common::norm_0_255(noseAreaScaled));
	cv::imshow("mouthArea",Common::norm_0_255(mouthAreaScaled));
	*/

	areas.append(eyesAreaScaled);
	areas.append(noseAreaScaled);
	areas.append(mouthAreaScaled);


}

void FaceDivider::divideByMethod2rigid(tFaceAreas &areas) {
	int divideYcoord1 = _depthmap.rows / 3;
	int divideYcoord2 = _depthmap.rows / 3 * 2;

	cv::Mat eyesArea  = _depthmap(cv::Range(0, divideYcoord1), cv::Range::all());
	cv::Mat noseArea = _depthmap(cv::Range(divideYcoord1, divideYcoord2), cv::Range::all());
	cv::Mat mouthArea = _depthmap(cv::Range(divideYcoord2,_depthmap.rows), cv::Range::all());

	areas.append(eyesArea);
	areas.append(noseArea);
	areas.append(mouthArea);

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

	cv::Mat leftEyeArea  = _depthmap(cv::Range(0, noseMiddlePoint.y),
									 cv::Range(0, noseMiddlePoint.x));

	cv::Mat rightEyeArea  = _depthmap(cv::Range(0, noseMiddlePoint.y),
									  cv::Range(noseMiddlePoint.x, _depthmap.cols));


	cv::Mat leftCheekArea = _depthmap(cv::Range(noseMiddlePoint.y, _landmarks.pos(Landmarks::NoseBottom).y),
									  cv::Range(0,_landmarks.pos(Landmarks::NoseCornerLeft).x));

	cv::Mat noseArea = _depthmap(cv::Range(noseMiddlePoint.y, _landmarks.pos(Landmarks::NoseBottom).y),
									  cv::Range(_landmarks.pos(Landmarks::NoseCornerLeft).x, _landmarks.pos(Landmarks::NoseCornerRight).x));

	cv::Mat rightCheekArea = _depthmap(cv::Range(noseMiddlePoint.y, _landmarks.pos(Landmarks::NoseBottom).y),
									  cv::Range(_landmarks.pos(Landmarks::NoseCornerRight).x, _depthmap.cols));



	cv::Mat mouthArea = _depthmap(cv::Range(_landmarks.pos(Landmarks::NoseBottom).y,_depthmap.rows),
								  cv::Range::all());

	int eyesAreaHeight = noseMiddlePointAvg.y;
	int leftEyeAreaWidth = noseMiddlePointAvg.x;
	int rightEyeAreaWidth = _depthmap.cols - noseMiddlePointAvg.x;

	int middleAreaHeight =  _averageLandmarks.pos(Landmarks::NoseBottom).y - noseMiddlePointAvg.y;
	int leftCheekAreaWidth = _averageLandmarks.pos(Landmarks::NoseCornerLeft).x;
	int rightCheekAreaWidth = _depthmap.cols - _averageLandmarks.pos(Landmarks::NoseCornerRight).x;
	int noseAreaWidth = _averageLandmarks.pos(Landmarks::NoseCornerRight).x - _averageLandmarks.pos(Landmarks::NoseCornerLeft).x;
	int mouthAreaHeight = _depthmap.rows - _averageLandmarks.pos(Landmarks::NoseBottom).y;

	cv::Mat leftEyeAreaScaled, rightEyeAreaScaled,  leftCheekAreaScaled, noseAreaScaled, rightCheekAreaScaled, mouthAreaScaled;

	resizeArea(leftEyeArea, leftEyeAreaScaled, leftEyeAreaWidth, eyesAreaHeight);
	resizeArea(rightEyeArea, rightEyeAreaScaled, rightEyeAreaWidth, eyesAreaHeight);

	resizeArea(leftCheekArea, leftCheekAreaScaled, leftCheekAreaWidth, middleAreaHeight);
	resizeArea(noseArea, noseAreaScaled, noseAreaWidth, middleAreaHeight);
	resizeArea(rightCheekArea, rightCheekAreaScaled, rightCheekAreaWidth, middleAreaHeight);
	resizeArea(mouthArea, mouthAreaScaled, _depthmap.cols, mouthAreaHeight);


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
	areas.append(leftCheekAreaScaled);
	areas.append(mouthAreaScaled);

}

void FaceDivider::divideByMethod3rigid(tFaceAreas &areas) {
	int divideYcoord1 = _depthmap.rows / 3;
	int divideYcoord2 = _depthmap.rows / 3 * 2;

	int divideXeyes = _depthmap.cols / 2;

	int divideX1 = _depthmap.cols / 3;
	int divideX2 = _depthmap.cols / 3 * 2;

	cv::Mat leftEyeArea  = _depthmap(cv::Range(0, divideYcoord1), cv::Range(0,divideXeyes));
	cv::Mat rightEyeArea  = _depthmap(cv::Range(0, divideYcoord1), cv::Range(divideXeyes, _depthmap.cols));


	cv::Mat leftCheekArea = _depthmap(cv::Range(divideYcoord1, divideYcoord2), cv::Range(0, divideX1));
	cv::Mat noseArea = _depthmap(cv::Range(divideYcoord1, divideYcoord2), cv::Range(divideX1, divideX2));
	cv::Mat rightCheekArea = _depthmap(cv::Range(divideYcoord1, divideYcoord2), cv::Range(divideX2, _depthmap.cols));

	cv::Mat mouthArea = _depthmap(cv::Range(divideYcoord2,_depthmap.rows), cv::Range::all());

	areas.append(leftEyeArea);
	areas.append(rightEyeArea);
	areas.append(leftCheekArea);
	areas.append(noseArea);
	areas.append(rightCheekArea);
	areas.append(mouthArea);

}

void FaceDivider::resizeArea(cv::Mat &src, cv::Mat &dst, int width, int height) {

	cv::resize(src,dst,cv::Size(width, height),0,0,cv::INTER_LINEAR);
}
