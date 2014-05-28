#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <opencv/highgui.h>
#include <opencv2/flann/flann.hpp>


#include "mesh.h"
#include "common.h"
#include "averageface.h"

/**
 * @brief Constructor.
 */
Mesh::Mesh() {
	this->init();
}

/**
 * @brief Constructor.
 * @param src Source mesh
 */
Mesh::Mesh(const Mesh &src) {
	minx = src.minx;
	maxx = src.maxx;
	miny = src.miny;
	maxy = src.maxy;
	minz = src.minz;
	maxz = src.maxz;

	pointsMat = src.pointsMat.clone();
	triangles = src.triangles;
	//colors = src.colors;
	_color = src._color;
	name = src.name;

}

/**
 * @brief Constructor. Create mesh from 3d model stored in file.
 * @param path 3D model.
 * @param centralizeLoadedMesh Centroid of face to point (0,0,0)
 */
Mesh::Mesh(const QString path, bool centralizeLoadedMesh) {

	this->init();

	if(path.endsWith(".abs", Qt::CaseInsensitive)) {
		loadFromABS(path,centralizeLoadedMesh);

	} else if(path.endsWith(".obj", Qt::CaseInsensitive)) {
		loadFromOBJ(path,centralizeLoadedMesh);
	} else {
		throw std::runtime_error("Mesh(): unsuported type of 3D model:"+ path.toStdString() );
	}

	//get name
	QFileInfo fileInfo(path);
	name = fileInfo.baseName();
}

Mesh::~Mesh() {
}

/**
 * @brief Set initialization values of mesh.
 */
void Mesh::init() {
	minx = 1e300;
	maxx = -1e300;
	miny = 1e300;
	maxy = -1e300;
	minz = 1e300;
	maxz = -1e300;
	_color = QColor(Qt::red);
}

/**
 * @brief Recalculate minimum and maximum for each axis.
 */
void Mesh::recalculateMinMax() {
    cv::minMaxIdx(pointsMat.colRange(0,1), &minx, &maxx);
    cv::minMaxIdx(pointsMat.colRange(1,2), &miny, &maxy);
    cv::minMaxIdx(pointsMat.colRange(2,3), &minz, &maxz);
}

/**
 * @brief Translate centroid of mesh to point (0,0,0)
 */
void Mesh::centralize()
{
    int count = pointsMat.rows;
    double sumx = cv::sum(pointsMat.colRange(0,1))[0];
    double sumy = cv::sum(pointsMat.colRange(1,2))[0];
    double sumz = cv::sum(pointsMat.colRange(2,3))[0];

    for (int i = 0; i < count; i++)
    {
        pointsMat(i, 0) -= sumx/count;
        pointsMat(i, 1) -= sumy/count;
        pointsMat(i, 2) -= sumz/count;
    }

    minx -= sumx/count;
    miny -= sumy/count;
    maxx -= sumx/count;
    maxy -= sumy/count;
    minz -= sumz/count;
    maxz -= sumz/count;
}

/**
 * @brief Translate mesh.
 * @param translationVector Values of translation
 */
void Mesh::translate(cv::Point3d translationVector) {
    //qDebug() << "Mesh::translate(cv::Point3d translationVector)";
    AverageFace::translate(pointsMat, translationVector);
    minx += translationVector.x;
    miny += translationVector.y;
    minz += translationVector.z;
    maxx += translationVector.x;
    maxy += translationVector.y;
    maxz += translationVector.z;
}

/**
 * @brief Rotate mesh
 * @param x Parameter of rotation
 * @param y Parameter of rotation
 * @param z Parameter of rotation
 */
void Mesh::rotate(double x, double y, double z) {
    qDebug() << "Mesh::rotate(double x, double y, double z)";
    AverageFace::rotate(pointsMat, x, y, z);
    recalculateMinMax();
}

/**
 * @brief Transform mesh.
 * @param Transformation matrix 3x3
 */
void Mesh::transform(Matrix &m) {
   // qDebug() << "Mesh::transform(Matrix &m)";
    AverageFace::transform(pointsMat, m);

    recalculateMinMax();
}

/**
 * @brief Connect points to triangles.
 */
void Mesh::calculateTriangles() {
    QVector<cv::Point2d> points2d;
    for (int r = 0; r < pointsMat.rows; r++)
    {
        cv::Point2d p;
        p.x = pointsMat(r, 0);
        p.y = pointsMat(r, 1);
        points2d.append(p);
    }

    triangles = Common::delaunayTriangulation(points2d);
    //triangles = Delaunay::process(points2d);
}

/**
 * @brief Create mesh from ABS 3d model.
 * @param filename File of 3d model
 * @param centralizeLoadedMesh Centroid of face to point (0,0,0)
 */
void Mesh::loadFromABS(const QString &filename, bool centralizeLoadedMesh) {
	//qDebug() << "loading" << filename;
	assert(filename.endsWith(".abs", Qt::CaseInsensitive));
	QFile f(filename);
	bool exists = f.exists();
	assert(exists);
	bool opened = f.open(QIODevice::ReadOnly);
	assert(opened);
	QTextStream in(&f);

	int mapHeight;
	in >> mapHeight;
	in.readLine();
	int mapwidth;
	in >> mapwidth;

	in.readLine();
	in.readLine();

	int total = mapwidth*mapHeight;

	int flags[total];
	double *xPoints = new double[total];
	double *yPoints = new double[total];
	double *zPoints = new double[total];

	for (int i = 0; i < total; i++) {
		in >> (flags[i]);
	}

	for (int i = 0; i < total; i++) {
		in >> (xPoints[i]);
	}

	for (int i = 0; i < total; i++) {
		in >> (yPoints[i]);
	}

	for (int i = 0; i < total; i++) {
		in >> (zPoints[i]);
	}

	VectorOfPoints points;
	for (int i = 0; i < total; i++) {
		if (flags[i])
		{
			cv::Point3d p;
			p.x = xPoints[i];
			p.y = yPoints[i];
			p.z = zPoints[i];
			points.append(p);
		}
	}

	delete [] xPoints;
	delete [] yPoints;
	delete [] zPoints;

	this->loadFromPointcloud(points, centralizeLoadedMesh);
}

/**
 * @brief Create mesh from ABS 3d model.
 * @param filename File of 3d model
 * @param centralizeLoadedMesh Centroid of face to point (0,0,0)
 * @return Created mesh
 */
Mesh Mesh::fromABS(const QString &filename, bool centralizeLoadedMesh) {
    //qDebug() << "loading" << filename;
    assert(filename.endsWith(".abs", Qt::CaseInsensitive));
    QFile f(filename);
    bool exists = f.exists();
    assert(exists);
    bool opened = f.open(QIODevice::ReadOnly);
    assert(opened);
    QTextStream in(&f);

    int mapHeight;
    in >> mapHeight;
    in.readLine();
    int mapwidth;
    in >> mapwidth;

    in.readLine();
    in.readLine();

    int total = mapwidth*mapHeight;
    //qDebug() << "total points" << total;

    int flags[total];
    double *xPoints = new double[total];
    double *yPoints = new double[total];
    double *zPoints = new double[total];

    for (int i = 0; i < total; i++) {
        in >> (flags[i]);
    }
   // qDebug() << "flags loaded";

    for (int i = 0; i < total; i++) {
        in >> (xPoints[i]);
    }
   // qDebug() << "x points loaded";

    for (int i = 0; i < total; i++) {
        in >> (yPoints[i]);
    }
    //qDebug() << "y points loaded";

    for (int i = 0; i < total; i++) {
        in >> (zPoints[i]);
    }
    //qDebug() << "z points loaded";

    VectorOfPoints points;
    for (int i = 0; i < total; i++) {
        if (flags[i])
        {
            cv::Point3d p;
            p.x = xPoints[i];
            p.y = yPoints[i];
            p.z = zPoints[i];
            points.append(p);
        }
    }

    delete [] xPoints;
    delete [] yPoints;
    delete [] zPoints;

    return Mesh::fromPointcloud(points, centralizeLoadedMesh);
}


/**
 * @brief Create mesh from point cloud.
 * @param pointcloud Vector of points
 * @param centralizeLoadedMesh Centroid of face to point (0,0,0)
 * @param calculateTriangles Calculate Triangles
 */
void Mesh::loadFromPointcloud(VectorOfPoints &pointcloud, bool centralizeLoadedMesh, bool calculateTriangles) {

	int n = pointcloud.count();
	this->pointsMat = Matrix(n, 3);
	for (int i = 0; i < n; i++) {
		const cv::Point3d &p = pointcloud.at(i);
		this->pointsMat(i, 0) = p.x;
		this->pointsMat(i, 1) = p.y;
		this->pointsMat(i, 2) = p.z;
	}

	if (calculateTriangles) {
		this->calculateTriangles();
	}
	this->recalculateMinMax();

	if (centralizeLoadedMesh) {
		this->centralize();
	}
}

/**
 * @brief Create mesh from point cloud.
 * @param pointcloud Vector of points
 * @param centralizeLoadedMesh  Centroid of face to point (0,0,0)
 * @param calculateTriangles Calculate Triangles
 * @return Created mesh
 */
Mesh Mesh::fromPointcloud(VectorOfPoints &pointcloud, bool centralizeLoadedMesh, bool calculateTriangles) {
    Mesh m;

    int n = pointcloud.count();
    m.pointsMat = Matrix(n, 3);
    for (int i = 0; i < n; i++)
    {
        const cv::Point3d &p = pointcloud.at(i);
        m.pointsMat(i, 0) = p.x;
        m.pointsMat(i, 1) = p.y;
        m.pointsMat(i, 2) = p.z;
    }

    if (calculateTriangles) m.calculateTriangles();
    m.recalculateMinMax();
    if (centralizeLoadedMesh) m.centralize();

    return m;
}

/**
 * @brief Create depthmap from OBJ 3d model
 * @param filename File of 3d model
 * @param centralizeLoadedMesh Centroid of face to point (0,0,0)
 */
void Mesh::loadFromOBJ(const QString &filename, bool centralizeLoadedMesh) {
	assert(filename.endsWith(".obj", Qt::CaseInsensitive));
	QFile f(filename);
	bool fileExists = f.exists();
	assert(fileExists);
	bool fileOpened = f.open(QIODevice::ReadOnly);
	assert(fileOpened);
	QTextStream in(&f);

	VectorOfPoints points;
	VectorOfTriangles triangles;
	while (!in.atEnd())
	{
		QString line = in.readLine();
		QStringList items = line.split(QChar(' '));

		if (items[0].compare("v") == 0)
		{
			double x = items[1].toDouble();
			double y = items[2].toDouble();
			double z = items[3].toDouble();

			points << cv::Point3d(x,y,z);
		}
		else if (items[0].compare("f") == 0)
		{
			int t1 = items[1].toInt()-1;
			int t2 = items[2].toInt()-1;
			int t3 = items[3].toInt()-1;

			triangles << cv::Vec3i(t1, t2, t3);
		}
	}

	this->loadFromPointcloud(points, centralizeLoadedMesh, false);
	this->triangles = triangles;
}

/**
 * @brief Create depthmap from OBJ 3d model
 * @param filename File of 3d model
 * @param centralizeLoadedMesh Centroid of face to point (0,0,0)
 * @return Created mesh
 */
Mesh Mesh::fromOBJ(const QString &filename, bool centralizeLoadedMesh) {
    assert(filename.endsWith(".obj", Qt::CaseInsensitive));
    QFile f(filename);
    bool fileExists = f.exists();
    assert(fileExists);
    bool fileOpened = f.open(QIODevice::ReadOnly);
    assert(fileOpened);
    QTextStream in(&f);

    VectorOfPoints points;
    VectorOfTriangles triangles;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList items = line.split(QChar(' '));

        if (items[0].compare("v") == 0)
        {
            double x = items[1].toDouble();
            double y = items[2].toDouble();
            double z = items[3].toDouble();

            points << cv::Point3d(x,y,z);
        }
        else if (items[0].compare("f") == 0)
        {
            int t1 = items[1].toInt()-1;
            int t2 = items[2].toInt()-1;
            int t3 = items[3].toInt()-1;

            triangles << cv::Vec3i(t1, t2, t3);
        }
    }

    Mesh result = Mesh::fromPointcloud(points, centralizeLoadedMesh, false);
    result.triangles = triangles;
    return result;
}

/**
 * @brief Convert numer to string with decimal point
 * @param n Number
 * @param decimalPoint Char between integer and flaot part
 * @return Converted string.
 */
QString formatNumber(double n, char decimalPoint) {
    return QString::number(n).replace('.', decimalPoint);
}

/**
 * @brief Save mesh as OBJ 3d model.
 * @param path File to save
 * @param decimalPoint Char between integer and flaot part of number
 */
void Mesh::writeOBJ(const QString &path, char decimalPoint)
{
    QFile outFile(path);
    outFile.open(QFile::WriteOnly);
    QTextStream outStream(&outFile);

    for (int r = 0; r < pointsMat.rows; r++)
    {
        outStream << "v " << formatNumber(pointsMat(r, 0), decimalPoint)
                  << " " << formatNumber(pointsMat(r, 1), decimalPoint)
                  << " " << formatNumber(pointsMat(r, 2), decimalPoint) << endl;
    }

    int tCount = triangles.count();
    for (int i = 0; i < tCount; i++)
    {
        cv::Vec3i &t = triangles[i];
        outStream << "f " << (t[0]+1) << " " << (t[1]+1) << " " << (t[2]+1) << endl;
    }
}

/**
 * @brief Print stats of mesh.
 */
void Mesh::printStats() {
    qDebug() << "x-range: " << minx << maxx;
    qDebug() << "y-range: "<< miny << maxy;
    qDebug() << "z-range: "<< minz << maxz;
    qDebug() << "points: "<< pointsMat.rows;
    qDebug() << "triangles: "<< triangles.count();
}


/**
 * @brief Convert mesh to vector of point.
 * @return Vector of point
 */
QVector<cv::Point3d>* Mesh::getVectorOfPoint3d() {
    QVector<cv::Point3d>* result = new QVector<cv::Point3d>;

    for (int r = 0; r < pointsMat.rows; r++) {
        cv::Point3d p(pointsMat(r, 0),pointsMat(r, 1),pointsMat(r, 2));

        result->append(p);

    }

    return result;
}


/**
 * @brief Search nearest point for each point int mesh
 * @param inputMesh Mesh of points
 * @param index indexed to search up
 * @param distance Distance to nearest point
 * @return
 */
Mesh Mesh::getClosedPoints(Mesh &inputMesh, cv::flann::Index &index, float *distance) {
    VectorOfPoints newPoints;
    *distance = 0.0f;

    for (int r = 0; r < pointsMat.rows ; r++) {

        cv::Mat query;
        pointsMat.row(r).convertTo(query, CV_32F);

        std::vector<int> resultIndicies;
        std::vector<float> resultDistances;
        index.knnSearch(query, resultIndicies, resultDistances, 1);
        int pIndex = resultIndicies[0];

        cv::Point3d p(inputMesh.pointsMat(pIndex, 0),inputMesh.pointsMat(pIndex, 1),inputMesh.pointsMat(pIndex, 2));

        newPoints.append(p);

        *distance += resultDistances[0];

    }
    Mesh result = Mesh::fromPointcloud(newPoints, false, false);

    return result;

}

float Mesh::getClosedDistance(cv::flann::Index &index) {
	float distance = 0;

	QTime myTimer;
	myTimer.start();

	for (int r = 0; r < pointsMat.rows ; r++) {

		cv::Mat query;
		pointsMat.row(r).convertTo(query, CV_32F);

		std::vector<int> resultIndicies;
		std::vector<float> resultDistances;
		index.knnSearch(query, resultIndicies, resultDistances, 1);
		//int pIndex = resultIndicies[0];

		//cv::Point3d p(inputMesh.pointsMat(pIndex, 0),inputMesh.pointsMat(pIndex, 1),inputMesh.pointsMat(pIndex, 2));
		distance += resultDistances[0];

	}

	return distance;
}

/**
 * @brief Mesh::getExtract2dGrid - extract grid from face, use only x,y coords
 * @param grid
 * @param dst
 */
void Mesh::getExtract2dGrid(Mesh &grid, Mesh &dst) {

    VectorOfPoints newPoints;
    VectorOfColors newColors;

    //convert 3d to 2d
    Matrix pointsMat2d(pointsMat.operator ()(cv::Range::all(),cv::Range(0,2)));
    Matrix gridPointsMat2d(grid.pointsMat.operator ()(cv::Range::all(),cv::Range(0,2)));

    cv::flann::Index index;
    cv::Mat features;
    features = cv::Mat(pointsMat2d.rows, 2, CV_32F);

    pointsMat2d.convertTo(features, CV_32F);
    index.build(features, cv::flann::LinearIndexParams());

    //iterate over grid's pointsMat - point by point
    for (int r = 0; r < gridPointsMat2d.rows; r++) {

        //convert query point
        cv::Mat queryPoint;
        gridPointsMat2d.row(r).convertTo(queryPoint, CV_32F);

        std::vector<int> resultIndicies;
        std::vector<float> resultDistances;

        //search for 1 nearest point
        index.knnSearch(queryPoint, resultIndicies, resultDistances, 1);

        int pIndex = resultIndicies[0];

        //save nearest matrix's point
        cv::Point3d p(pointsMat(pIndex, 0),pointsMat(pIndex, 1),pointsMat(pIndex, 2));

        newPoints.append(p);
    }

    dst = Mesh::fromPointcloud(newPoints, false, true);
}

/**
 * @brief Extract part of mesh.
 * @param grid Part of mesh to be extracted
 * @return Extracted mesh
 */
Mesh Mesh::getExtract2dGrid(Mesh &grid) {

    Mesh destination;
    getExtract2dGrid(grid, destination);

    return destination;
}

/**
 * @brief Mesh::getExtract2dGrid Extract grid from face, used only x,y coords. if distance of closed point is higher then maxDistance, skip it
 * @param grid Extracted area
 * @param dst Extreacted mesh
 */
void Mesh::getExtract2dGrid_2(Mesh &grid, Mesh &dst) {

    //count distance between 2 poinst of grid
	float x = qAbs(grid.pointsMat(0,0) - grid.pointsMat(1,0)); //x
	float y = qAbs(grid.pointsMat(0,1) - grid.pointsMat(1,1)); //x
    float maxDistance = sqrt(x*x + y*y)/2;
    //qDebug() << "max distance: " << maxDistance;

    VectorOfPoints newPoints;


    //convert 3d to 2d
    Matrix pointsMat2d(pointsMat.operator ()(cv::Range::all(),cv::Range(0,2)));
    Matrix gridPointsMat2d(grid.pointsMat.operator ()(cv::Range::all(),cv::Range(0,2)));

    cv::flann::Index index;
    cv::Mat features;
    features = cv::Mat(pointsMat2d.rows, 2, CV_32F);

    pointsMat2d.convertTo(features, CV_32F);
    index.build(features, cv::flann::LinearIndexParams());

    //iterate over grid's pointsMat - point by point
    for (int r = 0; r < gridPointsMat2d.rows; r++) {

        //convert query point
        cv::Mat queryPoint;
        gridPointsMat2d.row(r).convertTo(queryPoint, CV_32F);

        std::vector<int> resultIndicies;
        std::vector<float> resultDistances;

        //search for 1 nearest point
        index.knnSearch(queryPoint, resultIndicies, resultDistances, 1);

        int pIndex = resultIndicies[0];


        /*
        qDebug("[%.1f:%.1f:%.1f] -> [%.3f:%.3f:%.3f] = %0.2f",
               grid.pointsMat(r,0), grid.pointsMat(r,1), grid.pointsMat(r,2),
               pointsMat(pIndex,0), pointsMat(pIndex,1), pointsMat(pIndex,2),
               resultDistances[0]);
        */
        cv::Point3d p;
        if(resultDistances[0] > maxDistance) {
            //save grid point
            //p = cv::Point3d(grid.pointsMat(r, 0),grid.pointsMat(r, 1),grid.pointsMat(r, 2));
            ;
        } else {
            //save nearest matrix's point
            p = cv::Point3d(pointsMat(pIndex, 0),pointsMat(pIndex, 1),pointsMat(pIndex, 2));
            newPoints.append(p);

        }
    }
    dst = Mesh::fromPointcloud(newPoints, false, true);
}

/**
 * @brief Extract mesh by asix Z.
 * @param zValue Value of axis Z
 * @return Extracted mesh
 */
Mesh Mesh::zLevelSelect(double zValue)
{
    VectorOfPoints newPoints;
    VectorOfColors newColors;
    for (int r = 0; r < pointsMat.rows; r++) {
        if (pointsMat(r, 2) >= zValue) {
            newPoints << cv::Point3d(pointsMat(r, 0), pointsMat(r, 1), pointsMat(r, 2));

        }
    }

    Mesh result = Mesh::fromPointcloud(newPoints, false, true);

    return result;
}

/**
 * @brief Extract mesh by radius
 * @param radius Radius of selected area
 * @param center Center of selected area
 * @return Extracted mesh
 */
Mesh Mesh::radiusSelect(double radius, cv::Point3d center) {
    VectorOfPoints newPoints;
    VectorOfColors newColors;
    for (int r = 0; r < pointsMat.rows; r++) {
        cv::Point3d p(pointsMat(r, 0), pointsMat(r, 1), pointsMat(r, 2));
        if (euclideanDistance(p, center) <= radius) {
            newPoints << p;
        }
    }

    Mesh result = Mesh::fromPointcloud(newPoints, false, true);
    return result;
}

/**
 * @brief Compute average mesh
 * @param src Mesh 1
 * @param dst Mesh 2
 * @param dstWeight Weight of Mesh 2
 */
void Mesh::averageMesh(Mesh &src, Mesh &dst, int dstWeight) {
    //AverageFace::averageMatrices(src.pointsMat, dst.pointsMat, dstWeight);

	assert(dst.pointsMat.rows == src.pointsMat.rows);

    for (int r = 0; r < dst.pointsMat.rows; r++) {
        //dst = (dst*dstWeight + src) / (dstWeight + 1);

        //qDebug() << "compute: " << r;
        dst.pointsMat(r,0) = (dst.pointsMat(r,0) * dstWeight + src.pointsMat(r,0)) / (dstWeight + 1);
        dst.pointsMat(r,1) = (dst.pointsMat(r,1) * dstWeight + src.pointsMat(r,1)) / (dstWeight + 1);
        dst.pointsMat(r,2) = (dst.pointsMat(r,2) * dstWeight + src.pointsMat(r,2)) / (dstWeight + 1);

    }

}

/**
 * @brief Crop mesh by points.
 * @param topLeft Top Left point of croped area
 * @param bottomRight Bottom right point of croped area
 * @return Extracted mesh
 */
Mesh Mesh::crop(cv::Point3d topLeft, cv::Point3d bottomRight) {
	VectorOfPoints newPoints;

	for (int r = 0; r < pointsMat.rows; r++) {
		 cv::Point3d p(pointsMat(r, 0), pointsMat(r, 1), pointsMat(r, 2));

		 if(p.x < topLeft.x || p.y > topLeft.y) {
			 continue;
		 }
		 if(p.x > bottomRight.x || p.y < bottomRight.y) {
			 continue;
		 }
		 newPoints.append(p);
	 }
	 Mesh result = Mesh::fromPointcloud(newPoints, false, true);
	 return result;
}

/**
 * @brief Crop mesh by points.
 * @param topLeft Top Left point of croped area
 * @param bottomRight Bottom right point of croped area
 */
void Mesh::cropMe(cv::Point2d topLeft, cv::Point2d bottomRight) {
	VectorOfPoints newPoints;
	for (int r = 0; r < pointsMat.rows; r++) {
		 cv::Point3d p(pointsMat(r, 0), pointsMat(r, 1), pointsMat(r, 2));

		 if(p.x < topLeft.x || p.y > topLeft.y) {
			 continue;
		 }
		 if(p.x > bottomRight.x || p.y < bottomRight.y) {
			 continue;
		 }
		//point is not OUT, so add it to new mesh
		newPoints.append(p);
	 }
	loadFromPointcloud(newPoints,false, true);
}

/**
 * @brief Crop mesh by distances from center point
 * @param center Center point of selected area
 * @param deltaPX Half width of selected area
 * @param deltaMX Half width of selected area
 * @param deltaPY Half height of selected area
 * @param deltaMY Half height of selected area
 * @return Extracted area.
 */
Mesh Mesh::crop(cv::Point3d center, int deltaPX, int deltaMX, int deltaPY, int deltaMY) {
    VectorOfPoints newPoints;
    VectorOfColors newColors;
    for (int r = 0; r < pointsMat.rows; r++) {

        cv::Point3d p(pointsMat(r, 0), pointsMat(r, 1), pointsMat(r, 2));

        if( p.x <= center.x - deltaMX || p.x >= center.x + deltaPX) {
            continue;
        }

        if( p.y <= center.y - deltaMY || p.y >= center.y + deltaPY) {
            continue;
        }
        //point is IN, so add it to new mesh
        newPoints.append(p);

	}
    Mesh result = Mesh::fromPointcloud(newPoints, false, true);

    return result;

}

/**
 * @brief Compute mean point of mesh
 * @return Mean Point
 */
cv::Point3d Mesh::getMeanPoint() {
    cv::Point3d mean;
    for (int r = 0; r < pointsMat.rows; r++) {
        mean.x += pointsMat(r, 0);
        mean.y += pointsMat(r, 1);
        mean.z += pointsMat(r, 2);

    }
    mean.x /= pointsMat.rows;
    mean.y /= pointsMat.rows;
    mean.z /= pointsMat.rows;

    return mean;
}

/**
 * @brief Search closed point from mesh
 * @param point Point to search
 * @return Index of closed point
 */
int Mesh::getClosed2dPoint(cv::Point2d point) {

    int index = 0; //index of closed point
    //cv::Point3f closedPoint(pointsMat(0, 0), pointsMat(0, 1), pointsMat(0, 2));
    double closedDelta;

	double deltaX = qAbs(pointsMat(index,0) - point.x);
	double deltaY = qAbs(pointsMat(index,1) - point.y);
    double delta = deltaX*deltaX + deltaY*deltaY;
    closedDelta = delta;

    for (int r = 1; r < pointsMat.rows; r++) {

		deltaX = qAbs(pointsMat(index,0) - pointsMat(r,0));
		deltaY = qAbs(pointsMat(index,1) - pointsMat(r,1));
        delta = deltaX*deltaX + deltaY*deltaY;

        if(delta < closedDelta) {
            index = r;
            closedDelta = delta;
        }
    }
    //qDebug() << "return idnex:"<<index;
    return index;
}


/**
 * @brief Creaete 3D grid
 * @param topLeft Top left point of grid
 * @param bottomRight Bottom right point of grid
 * @param stepX Distance between points in axis X
 * @param stepY Distance between points in axis Y
 * @return Created grid
 */
Mesh Mesh::create2dGrid(cv::Point3d topLeft, cv::Point3d bottomRight, int stepX, int stepY) {

    VectorOfPoints newPoints;
	int gridSizeX = qRound(qAbs(bottomRight.x - topLeft.x)  / stepX +1);
	int gridSizeY = qRound(qAbs(bottomRight.y - topLeft.y)  / stepY +1);

    cv::Point3d p = topLeft;

    for(int gridY = 0; gridY < gridSizeY; gridY++) {

        for(int gridX = 0; gridX < gridSizeX; gridX++) {
            newPoints.append(p);
            //next
            p.x += stepX;
        }
        p.x = topLeft.x;
        p.y -= stepY;
    }

    Mesh result = Mesh::fromPointcloud(newPoints, false, true);

    return result;
}

