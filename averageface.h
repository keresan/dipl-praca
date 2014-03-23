#ifndef AVERAGEFACE_H
#define AVERAGEFACE_H

#include "common.h"
#include "mesh.h"

#include <QStringList>
#include <QDir>
#include <QTextStream>
#include <QDebug>

/**
 * @brief create average face
 */
class AverageFace {
public:
	AverageFace(QString pathToLandmarks, QString pathToFaces, QString startFileLandmark);

    static cv::Point3d getMeanPoint(QVector<cv::Point3d>& points);
    static void readVector3dPointsFromFile(QString path, QVector<cv::Point3d> &v);

    static Matrix getOptimalRotation(QVector<cv::Point3d> &from, QVector<cv::Point3d> &to);
    static Matrix getOptimalRotation(Mesh &from, Mesh &to);

    static cv::Point3d getOptimalTranslation(QVector<cv::Point3d> &from, QVector<cv::Point3d> &to);
    static cv::Point3d getOptimalTranslation(Mesh &from, Mesh &to);


    /* shift all points */
    static void translate(QVector<cv::Point3d> &v,cv::Point3d shift);
    static void translate(Matrix &m, cv::Point3d shift);

    /* apply matrix m to all points */
    static void transform(cv::Point3d &p, const Matrix &m);
    static void transform(QVector<cv::Point3d> &points, const Matrix &m);
    static void transform(Matrix &points, const Matrix &m);

    static void averageMatrices(Matrix &src, Matrix &dst, int dstWeight);

	void process(QString resultFilePath);

    static void rotate(Matrix &points, double x, double y, double z);
    static Matrix getRotateMatrix(double x, double y, double z);

private:
    QString _pathToLandmarks;
	QString _pathToFaces;
    QStringList _listOfFiles;
    QDir _dir;
    QString _dirPath;

    QVector<cv::Point3d> _startVector;
    Mesh _startModel;

    Mesh _averageModel;
    Mesh _gridMesh;

    /**
     * @brief vaha _averageModel pri pocitani priemeru s dalsim modelom.
     *        S kazdym dalsim pridanym modelom vzrastie vaha o +1.
     */
    int _weight;




};

#endif // AVERAGEFACE_H
