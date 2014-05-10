#ifndef COMMON_H
#define COMMON_H

#include <QVector>
#include <QMap>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

#include <opencv2/opencv.hpp>



typedef cv::Mat_<double> Matrix;
typedef cv::Mat_<cv::Vec3b> ImageBGR;
typedef cv::Mat_<uint8_t> ImageGrayscale;

/**
 * @brief The Common class
 */
class Common {
public:
    //static double absSum(Matrix &m);
    static QVector<cv::Vec3i> delaunayTriangulation(QVector<cv::Point2d> &points);

    static void printMatrix(const Matrix &m);
    static void delay(int msec);

	//setings

	static const cv::Point3d averageFaceTL, averageFaceBR;
	static const cv::Point2d depthMapTL, depthMapBR;
	static const int depthMapPixelsX, depthMapPixelsY;
	static const QString pathToWarehouse;
	static const QString pathToDepthmapF2003,pathToFall2003,pathToSpring2003, pathToDepthmapS2003, pathToDepthmapS2004,pathToSpring2004;

	static const QString pathToSubspacesDir;



	static const int alignerFindBestStartPosRangeX,alignerFindBestStartPosRangeY, alignerFindBestStartPosStep;
	static const int alignerConvergentTreshold, alignerMaxIterations;
	static const int alignerDistanceTresholdToContinue;

	static const cv::Rect faceCropArea;
	static const int detectEyeAreaWidth, detectEyeAreaHalfHeight;
	static const int detectNoseCornersAreaHalfWidth, detectNoseCornersAreaHalfHeight;
	static const int detectNoseBottomAreaHalfWidth, detectNoseBottomAreaHeight;
	static const int detectNoseRootAreaHalfWidth;
	static const int detectNoseRootMinDistanceFromTip;



	static const QString lmPathToLmDir, lmAvgLmLabel, lmSavePosLabel, lmSaveIsLabel;

	static const QString eigenMethot1Label,eigenMethot2Label,eigenMethot3Label;
	static const QString eigenEigenvectorLabel, eigenEigenvaluesLabel, eigenMeanLabel, eigenArraySizeLabel;

	static const double detectHightPassFilterValue;

	static const QString depthmapIterationsCountLabel, depthmapDistanceFromModelLabel, depthmapDepthmapLabel;

	static cv::Mat norm_0_255(cv::InputArray _src);

	static void loadFilesPathFromDir(QString pathToDir, QStringList &list, const QStringList &filters = QStringList("*xml"));
	static void loadDepthMap(QString path, cv::Mat &depthMap,  double &distance, int &iterations);
	static void processLoadedMap(cv::Mat &depthMap);




    /*
    static void printMatrix(CvMat *m);

    static bool matrixContainsNan(const Matrix &m);
    static void savePlot(const QVector<double> &x, const QVector<double> &y, const QVector<double> &z, const QString &path);
    static void savePlot(const QVector<cv::Point3d> &values, const QString &path, bool append);
    static void savePlot(const QVector<double> &x, const QVector<double> &y, const QString &path);
    static void savePlot(const QVector<double> values[], int axisCount, const QString &path);
    static void saveMatrix(const Matrix &m, const QString &path);
    static void saveMap(QMap<double, double> &map, const QString &path);
    static Matrix loadMatrix(const QString &path);
    */
};

double euclideanDistance(const cv::Point3d &p1, const cv::Point3d &p2);
double euclideanDistance(const cv::Point &p1, const cv::Point &p2);

#endif // COMMON_H
