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


	int computeAlign(Mesh &still, Mesh &moving, int maxIterations, int threshold);



    void alignFace(Mesh &face);
    void alignFaceFast(Mesh &face);


    QVector<tTransformValue> transformValues;




private:


};

#endif // FACEALIGNER_H
