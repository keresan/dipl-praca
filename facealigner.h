#ifndef FACEALIGNER_H
#define FACEALIGNER_H

#include "mesh.h"
#include "averageface.h"

struct transformValues {
     QVector<Matrix> rotation;
     QVector<cv::Point3d> translate;
};

class FaceAligner {
public:
    FaceAligner();

    static transformValues *align(Mesh &still, Mesh &moving, int maxIterations, int threshold);

};

#endif // FACEALIGNER_H
