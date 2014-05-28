#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QtGlobal>
#include <QString>
#include <QRect>
#include <QColor>
#include <QRegularExpression>

#include "Opencv/cv.h"

//#include "map.h"
#include <QDebug>

#include <cmath>
//#include "maskedvector.h"
#include "common.h"


#define UNKNOWN_POINT cv::Point3d(-999,-999,-999)

typedef QVector<cv::Point3d> VectorOfPoints;
typedef QVector<cv::Vec3i> VectorOfTriangles;
typedef cv::Vec3b Color;
typedef QVector<Color> VectorOfColors;


/**
 * @brief Class for represent 3D model of face
 */
class Mesh {
public:
	typedef cv::Vec3i Triangle;
	typedef cv::Vec3b Color;

	typedef QVector<Triangle> Triangles;
	typedef QVector<Color> Colors;

    Matrix pointsMat;
    VectorOfTriangles triangles;
   // VectorOfColors colors;

    double minx, maxx, miny, maxy, minz, maxz;
    QColor _color;

	QString name;

    //static cv::Point3d UNKNOWN_POINT;

    Mesh();
    Mesh(const Mesh &src);
	Mesh(const QString path, bool centralizeLoadedMesh = false);
    virtual ~Mesh();

    void printStats();

    //transformations
    void centralize();
    void translate(cv::Point3d translationVector);
    void transform(Matrix &m);
    void rotate(double x, double y, double z);

    //selections
	Mesh crop(cv::Point3d center, int deltaPX, int deltaMX, int deltaPY, int deltaMY);
	Mesh crop(cv::Point3d topLeft, cv::Point3d bottomRight);

	void  cropMe(cv::Point2d topLeft, cv::Point2d bottomRight);

	Mesh zLevelSelect(double zValue);
    Mesh radiusSelect(double radius, cv::Point3d center = cv::Point3d(0,0,0));

    //read, write
    void writeOBJ(const QString &path, char decimalPoint);

	static Mesh fromABS(const QString &filename, bool centralizeLoadedMesh = false);
    static Mesh fromOBJ(const QString &filename, bool centralizeLoadedMesh = false);

	static Mesh fromPointcloud(VectorOfPoints &pointcloud, bool centralizeLoadedMesh = false, bool calculateTriangles = true);

	void loadFromABS(const QString &filename, bool centralizeLoadedMesh = false);
	void loadFromOBJ(const QString &filename, bool centralizeLoadedMesh = false);
	void loadFromPointcloud(VectorOfPoints &pointcloud, bool centralizeLoadedMesh = false, bool calculateTriangles = true);

    //grid
    static Mesh create2dGrid(cv::Point3d topLeft, cv::Point3d bottomRight, int stepX, int stepY);
    Mesh getExtract2dGrid(Mesh &grid);
    void getExtract2dGrid(Mesh &grid, Mesh &dst);
    void getExtract2dGrid_2(Mesh &grid, Mesh &dst);

    Mesh getClosedPoints(Mesh &inputMesh, cv::flann::Index &index, float *distance);
	float getClosedDistance(cv::flann::Index &index);
    int getClosed2dPoint(cv::Point2d point);

    static void averageMesh(Mesh &src, Mesh &dst, int dstWeight);

    cv::Point3d getMeanPoint();
    QVector<cv::Point3d>* getVectorOfPoint3d();

    void calculateTriangles();
    void recalculateMinMax();

private:
	void init();

};

#endif // MESH_H
