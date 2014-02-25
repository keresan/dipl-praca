#include "facealigner.h"

FaceAligner::FaceAligner() {


}

transformValues* FaceAligner::align(Mesh &still, Mesh &moving, int maxIterations, int threshold) {

    //Matrix rotateMatrixMul = Matrix::ones(3,3);
    transformValues* values = new transformValues;

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

        qDebug() << iter <<":distance: " << distance;
        if(lastDistance  <= distance + threshold) {
            qDebug() << "okej";
            break;
        }

        transformValues t;
        cv::Point3d translation = AverageFace::getOptimalTranslation(moving, closedPoints);

        values->translate.append(translation);

        Matrix rotation = AverageFace::getOptimalRotation(moving, closedPoints);

        values->rotation.append(rotation);

        //move points
        moving.translate(translation);
        moving.transform(rotation);
        lastDistance = distance;
    }

    return values;


}
