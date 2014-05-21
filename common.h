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

	static const QString pathToSubspacesDir, pathToComResultDir;
	static const QString pathToAverageFace, pathToAverageDepthmap,pathToAverageLm, pathToScnData;
	static const QString wrongDpSuffixLabel, wrongLmSuffixLabel;
	static const QString vectorSizeLabel, subVectorSizeLabel;
	static const int lmDeltaFromAvg;

	static const int alignerFindBestStartHalfRangeX,alignerFindBestStartHalfRangeY, alignerFindBestStartPosStep;
	static const int alignedFindBestStartPosZShift;
	static const int alignerConvergentTreshold, alignerMaxIterations;
	static const int alignerDistanceTresholdToContinue;

	static const cv::Rect faceCropArea;
	static const int faceWidth;
	static const int detectEyeAreaWidth, detectEyeAreaHalfHeight;
	static const int detectNoseCornersAreaHalfWidth, detectNoseCornersAreaHalfHeight;
	static const int detectNoseBottomAreaHalfWidth, detectNoseBottomAreaHeight;
	static const int detectNoseRootAreaHalfWidth;
	static const int detectNoseRootMinDistanceFromTip;
	static const int detectNoseTipAreaStartY, detectNoseTipAreaHeight;

	static const double depthmapInitValue;

	static const QString lmPathToLmDir, lmAvgLmLabel, lmSavePosLabel, lmSaveIsLabel;

	static const QString eigenMethod0Label, eigenMethod1Label,eigenMethod2Label,eigenMethod3Label;
	static const QString eigenEigenvectorLabel, eigenEigenvaluesLabel, eigenMeanLabel;

	static const double detectHightPassFilterValue;

	static const QString depthmapIterationsCountLabel, depthmapDistanceFromModelLabel, depthmapDepthmapLabel;

	static const QString cmpResultLabel;
	static const QString cmpResultGenuineMethod0Label, cmpResultImposterMethod0Label;
	static const QString cmpResultGenuineMethod1Label, cmpResultImposterMethod1Label;
	static const QString cmpResultGenuineMethod1Label_2, cmpResultImposterMethod1Label_2;
	static const QString cmpResultGenuineMethod2Label, cmpResultImposterMethod2Label;
	static const QString cmpResultGenuineMethod3Label, cmpResultImposterMethod3Label;

	static const QString cmpResultArenaGenuineMethod0Label, cmpResultArenaImposterMethod0Label;
	static const QString cmpResultArenaGenuineMethod1Label, cmpResultArenaImposterMethod1Label;
	static const QString cmpResultArenaGenuineMethod2Label, cmpResultArenaImposterMethod2Label;
	static const QString cmpResultArenaGenuineMethod3Label, cmpResultArenaImposterMethod3Label;

	static const QString cmpResultBothGenuineMethod0Label, cmpResultBothImposterMethod0Label;
	static const QString cmpResultBothGenuineMethod1Label, cmpResultBothImposterMethod1Label;
	static const QString cmpResultBothGenuineMethod2Label, cmpResultBothImposterMethod2Label;
	static const QString cmpResultBothGenuineMethod3Label, cmpResultBothImposterMethod3Label;


	static const QString scnArrayLabel;
	static const QString scnColumnLabel;
	static const QString scnMeanLabel;
	static const QString scnStdDevLabel;
	static const QString scnMinLabel;
	static const QString scnMaxLabel;

	static const QString scnStatMethod0Label;
	static const QString scnStatMethod1Label;
	static const QString scnStatMethod2Label;
	static const QString scnStatMethod3Label;

	static const QString scnStatArenaMethod0Label;
	static const QString scnStatArenaMethod1Label;
	static const QString scnStatArenaMethod2Label;
	static const QString scnStatArenaMethod3Label;

	static const QString statErrLabel;
	static const QString statHistGenuineLabel;
	static const QString statHistImposterLabel;
	static const QString statEerMethod0Label;
	static const QString statEerMethod1Label;
	static const QString statEerMethod2Label;
	static const QString statEerMethod3Label;

	static const QString statEerArenaMethod0Label;
	static const QString statEerArenaMethod1Label;
	static const QString statEerArenaMethod2Label;
	static const QString statEerArenaMethod3Label;



	static cv::Mat norm_0_255(cv::InputArray _src);

	static void loadFilesPathFromDir(QString pathToDir, QStringList &list, const QStringList &filters = QStringList("*xml"));

	static void loadDepthmap(QString fileName, QString dirPath, cv::Mat &depthmap,  double &distance, int &iterations);

	static void loadDepthmapProcess(QString fileName, QString dirPath, cv::Mat &depthmap, cv::Mat &averageFace,  double &distance, int &iterations);
	static void saveDepthmap(QString fileName, QString dirPath, cv::Mat &depthmap,  double distance, int iterations);

	static void saveCmpResult(cv::Mat &resultMatrix, QString fileName, QString dirPath = Common::pathToComResultDir);
	static void loadCmpResult(cv::Mat &resultMatrix, QString fileName, QString dirPath = Common::pathToComResultDir);

	static void processLoadedMap(cv::Mat &depthmap, cv::Mat &averageFace);

	static void replaceInitValues(cv::Mat &depthmap, cv::Mat &src);

	static bool isGenuinePerson(QString baseName1, QString baseName2);

	static void convertToMatrix(QVector<QVector<float> > &src, cv::Mat &dst);

};

double euclideanDistance(const cv::Point3d &p1, const cv::Point3d &p2);
double euclideanDistance(const cv::Point &p1, const cv::Point &p2);

#endif // COMMON_H
