#include "facealigner.h"

FaceAligner::FaceAligner() {



}

void FaceAligner::computeAlign(Mesh &still, Mesh &moving, int maxIterations, int threshold) {

    //Matrix rotateMatrixMul = Matrix::ones(3,3);
    //transformValues* values = new transformValues;

    //get closed point
    float lastDistance = std::numeric_limits<float>::max();

    //build index
    cv::flann::Index index;
    cv::Mat features;
    features = cv::Mat(still.pointsMat.rows, 2, CV_32F);
    still.pointsMat.convertTo(features, CV_32F);
    index.build(features, cv::flann::KMeansIndexParams());

    for(int iter = 0; iter < maxIterations; iter++) {
        float distance = 0.0f;
        Mesh closedPoints = moving.getClosedPoints(still, index, &distance);

        //qDebug() << iter <<":distance: " << distance;
        if(lastDistance  <= distance + threshold) {
            //qDebug() << "okej";
            break;
        }

        tTransformValue transValue;
        transValue.translate = AverageFace::getOptimalTranslation(moving, closedPoints);
        transValue.rotation = AverageFace::getOptimalRotation(moving, closedPoints);
        transformValues.append(transValue);

        //move points
        moving.translate(transValue.translate);
        moving.transform(transValue.rotation);
        lastDistance = distance;
    }
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
