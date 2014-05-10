#ifndef FACEALIGNER_H
#define FACEALIGNER_H

#include "mesh.h"
#include "averageface.h"


struct tTransformValue {
    Matrix rotation;
    cv::Point3d translate;
};

class FaceAligner {
public:
    FaceAligner();

	int align(Mesh &face, Mesh &modelFace, int maxIterations = 100, int treshold = 50, bool test = false);

	void findBestStartingPosition(Mesh &face, Mesh &modelFace, int  rangeX = 30, int rangeY = 30, int step = 15, bool test = false);

	int computeAlign(Mesh &still, Mesh &moving, int maxIterations, int threshold,bool test = false);

    void alignFace(Mesh &face);
    void alignFaceFast(Mesh &face);

	void buildIndex(cv::flann::Index &index, Mesh &face, cv::Mat &features);


    QVector<tTransformValue> transformValues;

	double finalDistance;
	double finalIterations;

private:


};

#endif // FACEALIGNER_H
