#include "facealigner.h"

FaceAligner::FaceAligner() {

}

int FaceAligner::align(Mesh &face, Mesh &modelFace, int maxIterations, int treshold, bool test) {

	transformValues.clear();

	QTime myTimer;
	myTimer.start();

	//back up face model
	Mesh bpModelFace(modelFace);

	//pre-align

	findBestStartingPosition(face,
							 bpModelFace,
							 Common::alignerFindBestStartHalfRangeX,
							 Common::alignerFindBestStartHalfRangeY,
							 Common::alignerFindBestStartPosStep,
							 test
							 );

	//compute align

	int result = computeAlign(face,bpModelFace, maxIterations, treshold,test);

	if(result > maxIterations) {
		qDebug() << "prekroceny limit zarovnania: " << face.name;
	}
	if(test) {

	}
	//qDebug() << "max iterations:" << maxIterations <<", treshold:" << treshold;
	//qDebug() << "pocet iteracii:" << result << "(max:" << maxIterations << ")";

	//transform
	alignFaceFast(face);


	return result;
}

int FaceAligner::computeAlign(Mesh &still, Mesh &moving, int maxIterations, int threshold, bool test) {

    //pokus o vytvorenie meshu still s mensim poctom bodov
    //vysledok: celkovy vypocet trva 4x dlhsie => treba urychlit getExtract2dGrid()
    //Mesh grid =  Mesh::create2dGrid(cv::Point3d(-100,150,0), cv::Point3d(100,150,0),2,2);
    //Mesh *stillDeprived = new Mesh();
    //still.getExtract2dGrid_2(grid, *stillDeprived);

	int iterations;

    //Matrix rotateMatrixMul = Matrix::ones(3,3);
    //transformValues* values = new transformValues;

    //get closed point
	float lastDistance = 0;

    //build index
	cv::flann::Index index;
    cv::Mat features;
	features = cv::Mat(still.pointsMat.rows, 2, CV_32F);
	still.pointsMat.convertTo(features, CV_32F);
    index.build(features, cv::flann::KMeansIndexParams());

	float distance = 0.0f;
	for(iterations = 0; iterations < maxIterations; iterations++) {  

		Mesh closedPoints = moving.getClosedPoints(still, index, &distance);

		if(test) {
			qDebug() << iterations <<":distance: " << distance;
		}
		if(abs(lastDistance - distance) <= threshold) {
			if( distance > Common::alignerDistanceTresholdToContinue && iterations < maxIterations ) {
				//qDebug() << iterations <<":distance: " << distance;
				//qDebug() << "turn back";
			} else {

				break;
			}
        }

        tTransformValue transValue;
        transValue.translate = AverageFace::getOptimalTranslation(moving, closedPoints);
        transValue.rotation = AverageFace::getOptimalRotation(moving, closedPoints);
        transformValues.append(transValue);

        //move points
        moving.translate(transValue.translate);
        moving.transform(transValue.rotation);
        lastDistance = distance;

		//qDebug() << "last distance: " << lastDistance;
    }

	finalDistance = distance;
	finalIterations = iterations;
	return iterations;
}


void FaceAligner::alignFace(Mesh &face) {

    // computeAlign() must be call first !
    assert(transformValues.count() > 0);

    //backward
    for(int i = transformValues.count() -1; i >=0; i--) {
        Matrix r = transformValues.at(i).rotation;
        cv::Point3d p = transformValues.at(i).translate;
        r = r.t();
        p = -p;

        AverageFace::transform(face.pointsMat, r);
        AverageFace::translate(face.pointsMat,p);
        //window->repaint();
        //parent->centralWidget()->repaint();
    }
}

void FaceAligner::alignFaceFast(Mesh &face) {

    // computeAlign() must be call first !
    assert(transformValues.count() > 0);

    Matrix accR = transformValues.last().rotation.t();
    cv::Point3d accP = transformValues.last().translate;

    //backward
    for(int i = transformValues.count() -2; i >=0; i--) {
        Matrix r = transformValues.at(i).rotation;
        cv::Point3d p = transformValues.at(i).translate;

        accP += p;
        // ak by sa urobila transpozicia accR az na konci (tak ako sa upravi accP),
        // tak to dava troska ine hodnoty

        accR = r.t() * accR;
    }
    accP = -accP;

    AverageFace::transform(face.pointsMat, accR);
    AverageFace::translate(face.pointsMat,accP);
}

void FaceAligner::buildIndex(cv::flann::Index &index, Mesh &face, cv::Mat &features) {

	features = cv::Mat(face.pointsMat.rows, 2, CV_32F);
	face.pointsMat.convertTo(features, CV_32F);
	index.build(features, cv::flann::KMeansIndexParams());

}

void FaceAligner::findBestStartingPosition(Mesh &face, Mesh &modelFace, int rangeX, int rangeY, int step, bool test) {

	Mesh bpModelFace(modelFace);

	//qDebug() << "face points: " << face.pointsMat.rows << "x" << face.pointsMat.cols;
	//qDebug() << "face triangles: " << face.triangles.count();

	//QTime myTimer, myTimer2;
	//myTimer.start();

	//build index
	cv::flann::Index *index = new cv::flann::Index();
	cv::Mat features;
	buildIndex(*index, face, features);

	/*
	features = cv::Mat(face.pointsMat.rows, 2, CV_32F);
	face.pointsMat.convertTo(features, CV_32F);
	index.build(features, cv::flann::KMeansIndexParams());
	*/

	/**
	  * zistene zaujimaovsti:
	  * 1) ak sa index vytvori vo funkcii buildIndex(), tak for trva o 2.5 sekundy viacej
	  *	 - index sa v oboch pripadoch vytvori rovnako rychlo
	  *  - pouzitie index.release() na konci na to nema ziadny vplyv.
	  *
	  *  mozno to bude mat nieco s features, ze sa musi kopirovat spolu s indexom zo stacku...
	  *  a je to presne tak !! chyba medzi stolickou a klavesnicou...
	  */

	//qDebug() << "vytvorenie indexu: "<< myTimer.elapsed() << "ms";
	//myTimer.restart();

	int prevX = 0;
	int prevY = 0;

	float bestDistance = std::numeric_limits<float>::max();
	float actualDistance = 0;
	cv::Point2d  bestPos;

	//set modelFace in front of face
	bpModelFace.translate(cv::Point3d(0,0,Common::alignedFindBestStartPosZShift));

	for(int x = -rangeX; x <= rangeX; x +=step) {
		for(int y= rangeY; y >= -rangeY; y -=step) {
			//count delta
			int deltaX = x - prevX;
			int deltaY = y - prevY;

			//translate face
			bpModelFace.translate(cv::Point3d(deltaX, deltaY,0));
			//count distance

			//myTimer2.restart();
			actualDistance = bpModelFace.getClosedDistance(*index);
			if(test) {
				qDebug("%dx%d distance: %10.1f",x,y,actualDistance);
			}

			//qDebug() << x <<"x"<< y << "cas:"<< myTimer2.elapsed() << "ms";


			if(actualDistance < bestDistance) {
				bestDistance = actualDistance;
				bestPos.x = x;
				bestPos.y = y;
			}

			prevX = x;
			prevY = y;

		}
	}

	//move model back to 0:0
	//moving.translate(cv::Point3d(-rangeX, rangeY,-100));

	//move face to best position
	face.translate(cv::Point3d(-bestPos.x, -bestPos.y,0));
	//qDebug() << "bestPosition:" << bestPos.x << bestPos.y;

	index->release(); //len pre skusku, asi to vobec nepomaha
	delete index;

	if(test) {
		qDebug() << "best starting pos: " << bestPos.x << bestPos.y;
	}
}
