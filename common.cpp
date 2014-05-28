#include "common.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

#include <iostream>
#include <assert.h>
#include <cmath>
#include <QStringList>



const cv::Point3d Common::averageFaceTL = cv::Point3d(-50,70,0);
const cv::Point3d Common::averageFaceBR = cv::Point3d(50,-10,0);

const cv::Point2d Common::depthMapTL = cv::Point2d(-80,100);
const cv::Point2d Common::depthMapBR = cv::Point2d(80,-70);
const int Common::depthMapPixelsX = 160*2;
const int Common::depthMapPixelsY = 170*2;
const int Common::faceWidth = 100;
const cv::Rect Common::faceCropArea = cv::Rect(60,50,200,190); //treba zmenit aj detectNoseTipAreaStartY
//const cv::Rect Common::faceCropArea = cv::Rect(50,30,220,240); //orig

const QString Common::wrongDpSuffixLabel = "_wrong_dp";
const QString Common::wrongLmSuffixLabel = "_wrong_lm";

const int Common::alignerFindBestStartHalfRangeX = 20;//50;
const int Common::alignerFindBestStartHalfRangeY = 20;//80;
const int Common::alignerFindBestStartPosStep = 10;
const int Common::alignedFindBestStartPosZShift = 0;
const int Common::alignerConvergentTreshold = 10;
const int Common::alignerMaxIterations = 150;
const int Common::alignerDistanceTresholdToContinue = 8000;

const QString Common::depthmapIterationsCountLabel = "iterations_to_align";
const QString Common::depthmapDistanceFromModelLabel = "distance_from_model";
const QString Common::depthmapDepthmapLabel = "depthmap"; //povodne bolo deptamp !!!!!!!!!!
const double Common::depthmapInitValue = -999.0;


const QString Common::lmAvgLmLabel = "average-landmarks.xml";
const QString Common::lmSavePosLabel = "landmark";
const QString Common::lmSaveIsLabel = "isEntered";
const int Common::lmDeltaFromAvg = 25;



const QString Common::eigenMethod0Label = "method-0";
const QString Common::eigenMethod1Label = "method-1";
const QString Common::eigenMethod2Label = "method-2";
const QString Common::eigenMethod3Label = "method-3";
const QString Common::eigenMethod1rLabel = "method-1r";
const QString Common::eigenMethod2rLabel = "method-2r";
const QString Common::eigenMethod3rLabel = "method-3r";

const QString Common::eigenEigenvectorLabel = "eigenvectors";
const QString Common::eigenEigenvaluesLabel = "eigenvalues";
const QString Common::eigenMeanLabel = "mean";
const QString Common::vectorSizeLabel = "array-size";
const QString Common::subVectorSizeLabel = "sub-vector-size";

// const int Common::detectNoseTipAreaStartY = 90; //orig
const int Common::detectNoseTipAreaStartY = 70;
const int Common::detectNoseTipAreaHeight = 80;
const int Common::detectEyeAreaHalfHeight = 40;
const int Common::detectEyeAreaWidth = 60;
const int Common::detectNoseCornersAreaHalfWidth = 70;
const int Common::detectNoseCornersAreaHalfHeight = 20; //pri vacsej oblasti uz zacinaju zasahovat fuzy
const int Common::detectNoseBottomAreaHalfWidth = 20;
const int Common::detectNoseBottomAreaHeight = 40;
const int Common::detectNoseRootAreaHalfWidth = 30;
const int Common::detectNoseRootMinDistanceFromTip = 50;
const double Common::detectHightPassFilterValue = 4.0;


const QString Common::cmpResultLabel = "result";
const QString Common::cmpResultGenuineMethod0Label = "method0-genuine-1.xml";
const QString Common::cmpResultImposterMethod0Label = "method0-imposter-1.xml";
const QString Common::cmpResultGenuineMethod1Label = "method1-genuine-1.xml";
const QString Common::cmpResultImposterMethod1Label = "method1-imposter-1.xml";
const QString Common::cmpResultGenuineMethod2Label = "method2-genuine-1.xml";
const QString Common::cmpResultImposterMethod2Label = "method2-imposter-1.xml";
const QString Common::cmpResultGenuineMethod3Label = "method3-genuine-1.xml";
const QString Common::cmpResultImposterMethod3Label = "method3-imposter-1.xml";
const QString Common::cmpResultGenuineMethod1rLabel = "method1r-genuine-1.xml";
const QString Common::cmpResultImposterMethod1rLabel = "method1r-imposter-1.xml";
const QString Common::cmpResultGenuineMethod2rLabel = "method2r-genuine-1.xml";
const QString Common::cmpResultImposterMethod2rLabel = "method2r-imposter-1.xml";
const QString Common::cmpResultGenuineMethod3rLabel = "method3r-genuine-1.xml";
const QString Common::cmpResultImposterMethod3rLabel = "method3r-imposter-1.xml";

const QString Common::cmpResultArenaGenuineMethod0Label = "method0-arena-genuine-1.xml";
const QString Common::cmpResultArenaImposterMethod0Label = "method0-arena-imposter-1.xml";
const QString Common::cmpResultArenaGenuineMethod1Label = "method1-arena-genuine-1.xml";
const QString Common::cmpResultArenaImposterMethod1Label = "method1-arena-imposter-1.xml";
const QString Common::cmpResultArenaGenuineMethod2Label = "method2-arena-genuine-1.xml";
const QString Common::cmpResultArenaImposterMethod2Label = "method2-arena-imposter-1.xml";
const QString Common::cmpResultArenaGenuineMethod3Label = "method3-arena-genuine-1.xml";
const QString Common::cmpResultArenaImposterMethod3Label = "method3-arena-imposter-1.xml";
const QString Common::cmpResultArenaGenuineMethod1rLabel = "method1r-arena-genuine-1.xml";
const QString Common::cmpResultArenaImposterMethod1rLabel = "method1r-arena-imposter-1.xml";
const QString Common::cmpResultArenaGenuineMethod2rLabel = "method2r-arena-genuine-1.xml";
const QString Common::cmpResultArenaImposterMethod2rLabel = "method2r-arena-imposter-1.xml";
const QString Common::cmpResultArenaGenuineMethod3rLabel = "method3r-arena-genuine-1.xml";
const QString Common::cmpResultArenaImposterMethod3rLabel = "method3r-arena-imposter-1.xml";

const QString Common::cmpResultBothGenuineMethod0Label = "method0-both-genuine-1.xml";
const QString Common::cmpResultBothImposterMethod0Label = "method0-both-imposter-1.xml";
const QString Common::cmpResultBothGenuineMethod1Label = "method1-both-genuine-1.xml";
const QString Common::cmpResultBothImposterMethod1Label = "method1-both-imposter-1.xml";
const QString Common::cmpResultBothGenuineMethod2Label = "method2-both-genuine-1.xml";
const QString Common::cmpResultBothImposterMethod2Label = "method2-both-imposter-1.xml";
const QString Common::cmpResultBothGenuineMethod3Label = "method3-both-genuine-1.xml";
const QString Common::cmpResultBothImposterMethod3Label = "method3-both-imposter-1.xml";
const QString Common::cmpResultBothGenuineMethod1rLabel = "method1r-both-genuine-1.xml";
const QString Common::cmpResultBothImposterMethod1rLabel = "method1r-both-imposter-1.xml";
const QString Common::cmpResultBothGenuineMethod2rLabel = "method2r-both-genuine-1.xml";
const QString Common::cmpResultBothImposterMethod2rLabel = "method2r-both-imposter-1.xml";
const QString Common::cmpResultBothGenuineMethod3rLabel = "method3r-both-genuine-1.xml";
const QString Common::cmpResultBothImposterMethod3rLabel = "method3r-both-imposter-1.xml";

const QString Common::scnArrayLabel = "array";

const QString Common::scnStdDevLabel = "standard-deviation";
const QString Common::scnColumnLabel = "column";
const QString Common::scnMeanLabel = "arithmetic-mean";
const QString Common::scnMinLabel = "min-value";
const QString Common::scnMaxLabel = "max-value";

const QString Common::statErrLabel = "equal-error-rate";
const QString Common::statHistGenuineLabel = "hist-genuine";
const QString Common::statHistImposterLabel = "hist-imposter";

const QString Common::statEerMethod0Label = "method0-eer.xml";
const QString Common::statEerMethod1Label = "method1-eer.xml";
const QString Common::statEerMethod2Label = "method2-eer.xml";
const QString Common::statEerMethod3Label = "method3-eer.xml";
const QString Common::statEerMethod1rLabel = "method1r-eer.xml";
const QString Common::statEerMethod2rLabel = "method2r-eer.xml";
const QString Common::statEerMethod3rLabel = "method3r-eer.xml";

const QString Common::statEerArenaMethod0Label = "method0-eer-arena.xml";
const QString Common::statEerArenaMethod1Label = "method1-eer-arena.xml";
const QString Common::statEerArenaMethod2Label = "method2-eer-arena.xml";
const QString Common::statEerArenaMethod3Label = "method3-eer-arena.xml";
const QString Common::statEerArenaMethod1rLabel = "method1r-eer-arena.xml";
const QString Common::statEerArenaMethod2rLabel = "method2r-eer-arena.xml";
const QString Common::statEerArenaMethod3rLabel = "method3r-eer-arena.xml";

const QString Common::scnStatMethod0Label = "method0-scn.xml";
const QString Common::scnStatMethod1Label = "method1-scn.xml";
const QString Common::scnStatMethod2Label = "method2-scn.xml";
const QString Common::scnStatMethod3Label = "method3-scn.xml";
const QString Common::scnStatMethod1rLabel = "method1r-scn.xml";
const QString Common::scnStatMethod2rLabel = "method2r-scn.xml";
const QString Common::scnStatMethod3rLabel = "method3r-scn.xml";

const QString Common::scnStatArenaMethod0Label = "method0-scn-arena.xml";
const QString Common::scnStatArenaMethod1Label = "method1-scn-arena.xml";
const QString Common::scnStatArenaMethod2Label = "method2-scn-arena.xml";
const QString Common::scnStatArenaMethod3Label = "method3-scn-arena.xml";
const QString Common::scnStatArenaMethod1rLabel = "method1r-scn-arena.xml";
const QString Common::scnStatArenaMethod2rLabel = "method2r-scn-arena.xml";
const QString Common::scnStatArenaMethod3rLabel = "method3r-scn-arena.xml";

QString Common::pathToWarehouse;
QString Common::pathToSubspacesDir;
QString Common::pathToComResultDir;
QString Common::pathToScnData;
QString Common::pathToLandmarkDir;

QString Common::pathToCreatedDepthmaps;
QString Common::pathTo3DModels;

QString Common::pathToAverageModelNormalize;
QString Common::pathToAverageDepthmap;
QString Common::pathToAverageLm;
QString Common::pathToAverageFaceDir;

/**
 * @brief Set and create structure of directories. One of faceModelDir or createdDepthmapDir must not be empty.
 * @param rootDir Path to root directory
 * @param faceModelDir Path to directory with 3D models. If depthmaps are already computed, can be empty.
 */
void Common::setDirStruct(QString rootDir, QString faceModelDir) {


	QDir root(rootDir);
	QDir models(faceModelDir);

	pathToWarehouse = root.absolutePath();
	pathTo3DModels = models.absolutePath();


	pathToCreatedDepthmaps = pathToWarehouse + "/created_depthmap/";
	pathToSubspacesDir = pathToWarehouse + "/subspaces/";
	pathToComResultDir = pathToWarehouse + "/compare_result/";
	pathToScnData = pathToWarehouse + "/score_data_init/";
	pathToLandmarkDir = pathToWarehouse + "/landmarks/";
	pathToAverageFaceDir = pathToWarehouse + "/averageFace/";

	//create directories
	root.mkpath(pathToSubspacesDir);
	root.mkpath(pathToComResultDir);
	root.mkpath(pathToScnData);
	root.mkpath(pathToLandmarkDir);
	root.mkpath(pathToAverageFaceDir);

	pathToAverageModelNormalize = pathToAverageFaceDir + "averageFace_final_norm.obj";
	pathToAverageDepthmap = pathToAverageFaceDir + "averageFace_final.xml";
	pathToAverageLm = pathToLandmarkDir + lmAvgLmLabel;
}

/**
 * @brief Print matrix row by row using qDebug().
 * @param m Matrix to print
 */
void Common::printMatrix(const Matrix &m) {
    QDebug deb = qDebug();
    for (int r = 0; r < m.rows; r++)
    {
        for (int c = 0; c < m.cols; c++) {
            //QString str = QString("%1").arg(m(r,c),7);

            deb <<  QString::number( m(r,c),'f',3);
            //std::cout << m(r,c) << " ";
        }
        deb << "\n";
        //std::cout << std::endl;
    }
}

/**
 * @brief Delay application for some time.
 * @param msec Time to delay
 */
void Common::delay(int msec) {
    QTime dieTime= QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/**
 * @brief Load files names from directory and concat it with ther absolute path.
 * @param pathToDir Path of directory for file search
 * @param list Result
 * @param filters Filter of file name or file type
 */
void Common::loadFilesPathFromDir(QString pathToDir, QStringList &list, const QStringList &filters) {

	QDir dir;
	dir.setPath(pathToDir);
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	dir.setNameFilters(filters);
	list = dir.entryList();

	//get absolute path
	for(int i = 0; i < list.size(); i++ ) {
		list[i] = dir.absoluteFilePath(list.at(i));
	}
}

/**
 * @brief Load depthmap from xml file and process it.
 * @param fileName Name of depthmap to load
 * @param dirPath Path to directory in which depthmap is
 * @param depthmap Loaded depthmap
 * @param averageFace Depthmap of average face, need to be there for face proccesing
 * @param distance Loaded distance from map
 * @param iterations Loaded iterations
 * @see @link processLoadedMap @endlink
 */
void Common::loadDepthmapProcess(QString fileName, QString dirPath, cv::Mat &depthmap, cv::Mat &averageFace, double &distance, int &iterations) {
	QDir dir(dirPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::READ);

	storage[Common::depthmapDepthmapLabel.toStdString()] >> depthmap;
	storage[Common::depthmapDistanceFromModelLabel.toStdString()] >> distance;
	storage[Common::depthmapIterationsCountLabel.toStdString()] >> iterations;

	storage.release();

	if(depthmap.rows == 0 || depthmap.cols == 0) {
		qDebug() << path;
		throw std::runtime_error("loadDepthmapProcess(): empty depthmap");
	}
	processLoadedMap(depthmap, averageFace);
}

/**
 * @brief Load depthmap from xml file.
 * @param fileName Name of depthmap to load
 * @param dirPath Path to directory in which depthmap is
 * @param depthmap Loaded depthmap
 * @param distance Loaded distance from map
 * @param iterations Loaded iterations
 */
void Common::loadDepthmap(QString fileName, QString dirPath, cv::Mat &depthmap, double &distance, int &iterations) {
	QDir dir(dirPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::READ);

	storage[Common::depthmapDepthmapLabel.toStdString()] >> depthmap;
	storage[Common::depthmapDistanceFromModelLabel.toStdString()] >> distance;
	storage[Common::depthmapIterationsCountLabel.toStdString()] >> iterations;

	storage.release();
}

/**
 * @brief Crop, blur and replece unitialized pixels with pixels from depthmap
 * @param depthmap
 * @param averageFace
 * @see @link replaceInitValues @endlink
 */
void Common::processLoadedMap(cv::Mat &depthmap, cv::Mat &averageFace) {

	//crop
	depthmap = depthmap(Common::faceCropArea);

	//replace unknown value with values from averageFace
	Common::replaceInitValues(depthmap,averageFace);

	//blur
	cv::medianBlur(depthmap,depthmap,5);

	//blur gaussian

	for(int i = 0; i < 2; i++) {
		cv::Size size(3,3);
		cv::GaussianBlur(depthmap, depthmap,size,0,0);
	}

}

/**
 * @brief Save depthmap to xml file.
 * @param fileName Name of file to save depthmap
 * @param dirPath Direcotry path to save
 * @param depthmap Depthmap to save
 * @param distance Distance to save
 * @param iterations Iterations to save
 */
void Common::saveDepthmap(QString fileName, QString dirPath, cv::Mat &depthmap, double distance, int iterations) {

	QDir dir(dirPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::WRITE);

	storage << Common::depthmapIterationsCountLabel.toStdString() << iterations;
	storage << Common::depthmapDistanceFromModelLabel.toStdString() << distance;
	storage << Common::depthmapDepthmapLabel.toStdString() << depthmap;
	storage.release();
}

/**
 * @brief Save results of face comparation.
 * @param resultMatrix Result of comparation
 * @param fileName Name of file to save
 * @param dirPath Directory path to save file
 */
void Common::saveCmpResult(cv::Mat &resultMatrix, QString fileName, QString dirPath) {
	QDir dir(dirPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::WRITE);
	storage << Common::cmpResultLabel.toStdString() << resultMatrix;

	storage.release();
}

/**
 * @brief Load result of camparation from file
 * @param resultMatrix Matrix of loaded results
 * @param fileName Name of file with results
 * @param dirPath Direcotry path to load file
 */
void Common::loadCmpResult(cv::Mat &resultMatrix, QString fileName, QString dirPath) {
	QDir dir(dirPath);
	QString path = dir.absoluteFilePath(fileName);

	cv::FileStorage storage(path.toStdString(), cv::FileStorage::READ);
	storage[Common::cmpResultLabel.toStdString()] >> resultMatrix;

	storage.release();

}

/**
 * @brief Euclidian distance of two 3D points.
 * @param p1 Point one
 * @param p2 Point two
 * @return euclidian distance
 */
double euclideanDistance(const cv::Point3d &p1, const cv::Point3d &p2)
{
    double dx = p1.x-p2.x;
    double dy = p1.y-p2.y;
    double dz = p1.z-p2.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

/**
 * @brief Euclidian distance od 2D points
 * @param p1 Point one
 * @param p2 Point two
 * @return
 */
double euclideanDistance(const cv::Point &p1, const cv::Point &p2)
{
    double dx = p1.x-p2.x;
    double dy = p1.y-p2.y;
    return sqrt(dx*dx + dy*dy);
}

/**
 * @brief Delaunay triangulation. Compute triangles from cloud points.
 * @param points Vector of 2D points
 * @return Vector of triangles
 */
QVector<cv::Vec3i> Common::delaunayTriangulation(QVector<cv::Point2d> &points) {
    int n = points.count();
    QVector<cv::Vec3i> result;

    int minx = INT_MAX;
    int maxx = INT_MIN;
    int miny = INT_MAX;
    int maxy = INT_MIN;

    for (int i = 0; i < n; i++) {
        int x = points[i].x;
        int y = points[i].y;
        if (x > maxx) maxx = x;
        if (x < minx) minx = x;
        if (y > maxy) maxy = y;
        if (y < miny) miny = y;
    }

    minx -= 10;
    miny -= 10;
    maxx += 10;
    maxy += 10;

    //qDebug() << "Delaunay in x =" << minx << ".." << maxx << "; y =" << miny << ".." << maxy << "; |points| =" << n;
    QMap<QPair<float, float>, int> coord2Index;

    CvRect rect = cv::Rect(minx, miny, maxx-minx, maxy-miny);
    cv::Subdiv2D subdiv(rect);
    std::vector<cv::Point2f> fPoints(n);
    for (int i = 0; i < n; i++) {
        float x = points[i].x;
        float y = points[i].y;
        fPoints[i] = cv::Point2f(x, y);
        coord2Index[QPair<float, float>(x, y)] = i;
    }
    subdiv.insert(fPoints);

    std::vector<cv::Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);

    //int edge;
    for (unsigned int i = 0; i < triangleList.size(); i++) {
        cv::Vec6f &triangle = triangleList.at(i);
        //int v1,v2,v3;
        cv::Point2f p1(triangle[0],triangle[1]);
        cv::Point2f p2(triangle[2],triangle[3]);
        cv::Point2f p3(triangle[4],triangle[5]);

        if (p1.x >= maxx) continue;
        if (p1.y >= maxy) continue;
        if (p2.x >= maxx) continue;
        if (p2.y >= maxy) continue;
        if (p3.x >= maxx) continue;
        if (p3.y >= maxy) continue;

        if (p1.x <= minx) continue;
        if (p1.y <= miny) continue;
        if (p2.x <= minx) continue;
        if (p2.y <= miny) continue;
        if (p3.x <= minx) continue;
        if (p3.y <= miny) continue;

        //save indecies of triangles points
        cv::Vec3i triangleIndicies;
        triangleIndicies[0] = coord2Index[QPair<float, float>(p1.x, p1.y)];
        triangleIndicies[1] = coord2Index[QPair<float, float>(p2.x, p2.y)];
        triangleIndicies[2] = coord2Index[QPair<float, float>(p3.x, p3.y)];

        result.append(triangleIndicies);
    }

    return result;
}

/**
 * @brief Normalize input image to range 0 - 255. Important for showing image using opencv.
 * @param _src Vector of input images
 * @return
 */
cv::Mat Common::norm_0_255(cv::InputArray _src) {
	cv::Mat src = _src.getMat();
	// Create and return normalized image:
	cv::Mat dst;
	switch(src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

/**
 * @brief Replace unknown values of point in depthamp with values from src
 * @param depthmap
 * @param src
 */
void Common::replaceInitValues(cv::Mat &depthmap, cv::Mat &src) {

	if(depthmap.rows != src.rows && depthmap.cols != src.cols) {
		throw std::runtime_error("depthmap sizes not equal");
	}

	for(int r = 0; r < depthmap.rows; r++) {
		for(int c = 0; c < depthmap.cols; c++) {
			if(depthmap.at<float>(r,c) < Common::depthmapInitValue+2) {

				float replaceValue = src.at<float>(r,c);
				if(replaceValue < Common::depthmapInitValue+2) {
					throw std::runtime_error("replaceInitValues(): unknown pixel value in source depthmap");
				} else {
					depthmap.at<float>(r,c) = replaceValue;
				}
			}
		}
	}
}


/**
 * @brief Decide if base names belong to same person or not.
 * @param baseName1 - label of person 1
 * @param baseName2 - label of person 2
 * @return True if base names belong to same person, otherwise false
 */
bool Common::isGenuinePerson(QString baseName1, QString baseName2) {
	QStringList person1 = baseName1.split('d');
	QStringList person2 = baseName2.split('d');

	assert(person1.size() == person2.size());
	assert(person1.size() == 2);

	if(person1.at(0) == person2.at(0)) {
		return true;
	} else {
		return false;
	}
}

/**
 * @brief Convert vector of vector to matrix
 * @param src Source vector
 * @param dst Destination matrix
 */
void Common::convertToMatrix(QVector<QVector<float> > &src, cv::Mat &dst) {
	dst = cv::Mat(src.size(), src.first().size(), CV_32F);

	for(int i=0; i<dst.rows; i++)
		 for(int j=0; j<dst.cols; j++)
			  dst.at<float>(i, j) = src.at(i).at(j);
}


