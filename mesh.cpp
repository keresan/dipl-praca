#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <opencv/highgui.h>
#include <opencv2/flann/flann.hpp>


#include "mesh.h"
//#include "surfaceprocessor.h"
//#include "delaunay.h" -> do common.h DelaunayTriangulation
#include "common.h"
#include "averageface.h"
//#include "procrustes.h"



void Mesh::recalculateMinMax()
{
    cv::minMaxIdx(pointsMat.colRange(0,1), &minx, &maxx);
    cv::minMaxIdx(pointsMat.colRange(1,2), &miny, &maxy);
    cv::minMaxIdx(pointsMat.colRange(2,3), &minz, &maxz);
}

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
/*
void Mesh::scale(cv::Point3d scaleParam)
{
    qDebug() << "Mesh::scale(cv::Point3d scaleParam)";
    Procrustes3D::scale(pointsMat, scaleParam);
    minx *= scaleParam.x;
    miny *= scaleParam.y;
    minz *= scaleParam.z;
    maxx *= scaleParam.x;
    maxy *= scaleParam.y;
    maxz *= scaleParam.z;
}

void Mesh::rotate(cv::Vec3d xyz)
{
    rotate(xyz(0), xyz(1), xyz(2));
}
*/

void Mesh::rotate(double x, double y, double z)
{
    qDebug() << "Mesh::rotate(double x, double y, double z)";
    AverageFace::rotate(pointsMat, x, y, z);
    recalculateMinMax();
}



void Mesh::transform(Matrix &m) {
   // qDebug() << "Mesh::transform(Matrix &m)";
    AverageFace::transform(pointsMat, m);

    recalculateMinMax();
}

void Mesh::calculateTriangles()
{
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

Mesh Mesh::fromABS(const QString &filename, const QString &texture, bool centralizeLoadedMesh) {
    assert(filename.endsWith(".abs", Qt::CaseInsensitive));

    cv::Mat_<cv::Vec3b> image = cv::imread(texture.toStdString());

    //qDebug() << "loading" << filename;
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
    //qDebug() << "flags loaded";

    for (int i = 0; i < total; i++) {
        in >> (xPoints[i]);
    }
    //qDebug() << "x points loaded";

    for (int i = 0; i < total; i++) {
        in >> (yPoints[i]);
    }
    //qDebug() << "y points loaded";

    for (int i = 0; i < total; i++) {
        in >> (zPoints[i]);
    }
    //qDebug() << "z points loaded";

    VectorOfPoints points;
    VectorOfColors colors;
    for (int i = 0; i < total; i++) {
        if (flags[i]) {
            cv::Point3d p;
            p.x = xPoints[i];
            p.y = yPoints[i];
            p.z = zPoints[i];
            points.append(p);

            int x = i % 640;
            int y = i / 640;
            colors << image(y, x);
        }
    }

    delete [] xPoints;
    delete [] yPoints;
    delete [] zPoints;

    Mesh mesh = Mesh::fromPointcloud(points, centralizeLoadedMesh);
    mesh.colors = colors;
    return mesh;
}

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
/*
Mesh Mesh::fromMap(Map &depth, Map &intensities, bool centralizeLoadedMesh)
{
    assert(depth.w == intensities.w);
    assert(depth.h == intensities.h);
    QMap<QPair<int,int>, int> coordToIndex;

    VectorOfPoints points;
    VectorOfColors colors;
    int index = 0;
    for (int y = 0; y < depth.h; y++)
    {
        for (int x = 0; x < depth.w; x++)
        {
            if (depth.isSet(x,y))
            {
                assert(intensities.isSet(x,y));

                points << cv::Point3d(x, depth.h-y-1, depth.get(x,y));

                uchar intensity = intensities.get(x, y);
                colors << cv::Vec3b(intensity, intensity, intensity);

                coordToIndex[QPair<int,int>(x,y)] = index;
                index++;
            }
        }
    }

    Mesh mesh = Mesh::fromPointcloud(points, centralizeLoadedMesh, false);
    mesh.colors = colors;

    // triangles
    for (int y = 0; y < depth.h; y++)
    {
        for (int x = 0; x < depth.w; x++)
        {
            if (depth.isSet(x,y) &&
                depth.isValidCoord(x, y+1) && depth.isSet(x, y+1) &&
                depth.isValidCoord(x+1, y+1) && depth.isSet(x+1, y+1))
            {
                mesh.triangles << cv::Vec3i(coordToIndex[QPair<int,int>(x,y)], coordToIndex[QPair<int,int>(x,y+1)], coordToIndex[QPair<int,int>(x+1,y+1)]);
            }

            if (depth.isSet(x,y) &&
                depth.isValidCoord(x+1, y+1) && depth.isSet(x+1, y+1) &&
                depth.isValidCoord(x+1, y) && depth.isSet(x+1, y))
            {
                mesh.triangles << cv::Vec3i(coordToIndex[QPair<int,int>(x,y)], coordToIndex[QPair<int,int>(x+1,y+1)], coordToIndex[QPair<int,int>(x+1,y)]);
            }
        }
    }

    return mesh;
}
*/
Mesh Mesh::fromOBJ(const QString &filename, bool centralizeLoadedMesh)
{
    qDebug() << "loading" << filename;
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

Mesh::Mesh()
{
    minx = 1e300;
    maxx = -1e300;
    miny = 1e300;
    maxy = -1e300;
    minz = 1e300;
    maxz = -1e300;
    _color = QColor(Qt::red);
}

Mesh::Mesh(const Mesh &src)
{
    minx = src.minx;
    maxx = src.maxx;
    miny = src.miny;
    maxy = src.maxy;
    minz = src.minz;
    maxz = src.maxz;

    pointsMat = src.pointsMat.clone();
    triangles = src.triangles;
    colors = src.colors;
}


Mesh::~Mesh()
{
    //qDebug() << "deleting mesh";
}

QString formatNumber(double n, char decimalPoint)
{
    return QString::number(n).replace('.', decimalPoint);
}

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







void Mesh::printStats()
{
    qDebug() << "x-range: " << minx << maxx;
    qDebug() << "y-range: "<< miny << maxy;
    qDebug() << "z-range: "<< minz << maxz;
    qDebug() << "points: "<< pointsMat.rows;
    qDebug() << "triangles: "<< triangles.count();
}





QVector<cv::Point3d>* Mesh::getVectorOfPoint3d() {
    QVector<cv::Point3d>* result = new QVector<cv::Point3d>;

    for (int r = 0; r < pointsMat.rows; r++) {
        cv::Point3d p(pointsMat(r, 0),pointsMat(r, 1),pointsMat(r, 2));

        result->append(p);

    }

    return result;
}


/**
 * @brief vyhlada najblyzsie body z inputMesh
 * @param inputMesh
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

Mesh Mesh::getExtract2dGrid(Mesh &grid) {

    VectorOfPoints newPoints;
    VectorOfColors newColors;

    //convert 3d to 2d
    Matrix pointsMat2d(pointsMat.operator ()(cv::Range::all(),cv::Range(0,2)));
    Matrix inputPointsMat2d(grid.pointsMat.operator ()(cv::Range::all(),cv::Range(0,2)));

    cv::flann::Index index;
    cv::Mat features;
    features = cv::Mat(pointsMat2d.rows, 2, CV_32F);

    pointsMat2d.convertTo(features, CV_32F);
    index.build(features, cv::flann::LinearIndexParams());

    for (int r = 0; r < inputPointsMat2d.rows; r++) {
        cv::Mat query;
        inputPointsMat2d.row(r).convertTo(query, CV_32F);

        std::vector<int> resultIndicies;
        std::vector<float> resultDistances;
        index.knnSearch(query, resultIndicies, resultDistances, 1);
        int pIndex = resultIndicies[0];

        cv::Point3d p(pointsMat(pIndex, 0),pointsMat(pIndex, 1),pointsMat(pIndex, 2));
        newPoints.append(p);
        if (colors.count() > 0) {
            newColors << colors[pIndex];
        }
    }

    Mesh result = Mesh::fromPointcloud(newPoints, false, true);
    result.colors = newColors;

    return result;

}


Mesh Mesh::zLevelSelect(double zValue)
{
    VectorOfPoints newPoints;
    VectorOfColors newColors;
    for (int r = 0; r < pointsMat.rows; r++)
    {
        if (pointsMat(r, 2) >= zValue)
        {
            newPoints << cv::Point3d(pointsMat(r, 0), pointsMat(r, 1), pointsMat(r, 2));
            if (colors.count() > 0)
            {
                newColors << colors[r];
            }
        }
    }

    Mesh result = Mesh::fromPointcloud(newPoints, false, true);
    result.colors = newColors;
    return result;
}

Mesh Mesh::radiusSelect(double radius, cv::Point3d center)
{
    VectorOfPoints newPoints;
    VectorOfColors newColors;
    for (int r = 0; r < pointsMat.rows; r++)
    {
        cv::Point3d p(pointsMat(r, 0), pointsMat(r, 1), pointsMat(r, 2));
        if (euclideanDistance(p, center) <= radius)
        {
            newPoints << p;
            if (colors.count() > 0)
            {
                newColors << colors[r];
            }
        }
    }

    Mesh result = Mesh::fromPointcloud(newPoints, false, true);
    result.colors = newColors;
    return result;
}


void Mesh::averageMesh(Mesh &src, Mesh &dst, int dstWeight) {
    //AverageFace::averageMatrices(src.pointsMat, dst.pointsMat, dstWeight);


    for (int r = 0; r < dst.pointsMat.rows; r++) {
        //dst = (dst*dstWeight + src) / (dstWeight + 1);

        //qDebug() << "compute: " << r;
        dst.pointsMat(r,0) = (dst.pointsMat(r,0) * dstWeight + src.pointsMat(r,0)) / (dstWeight + 1);
        dst.pointsMat(r,1) = (dst.pointsMat(r,1) * dstWeight + src.pointsMat(r,1)) / (dstWeight + 1);
        dst.pointsMat(r,2) = (dst.pointsMat(r,2) * dstWeight + src.pointsMat(r,2)) / (dstWeight + 1);

    }

}

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
        if (colors.count() > 0){
            newColors.append(colors[r]);
        }

    }
    Mesh result = Mesh::fromPointcloud(newPoints, false, true);
    result.colors = newColors;
    return result;

}

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

int Mesh::getClosed2dPoint(cv::Point2d point) {

    int index = 0; //index of closed point
    //cv::Point3f closedPoint(pointsMat(0, 0), pointsMat(0, 1), pointsMat(0, 2));
    double closedDelta;

    double deltaX = abs(pointsMat(index,0) - point.x);
    double deltaY = abs(pointsMat(index,1) - point.y);
    double delta = deltaX*deltaX + deltaY*deltaY;
    closedDelta = delta;

    for (int r = 1; r < pointsMat.rows; r++) {

        deltaX = abs(pointsMat(index,0) - pointsMat(r,0));
        deltaY = abs(pointsMat(index,1) - pointsMat(r,1));
        delta = deltaX*deltaX + deltaY*deltaY;

        if(delta < closedDelta) {
            index = r;
            closedDelta = delta;
        }
    }
    //qDebug() << "return idnex:"<<index;
    return index;
}


Mesh Mesh::create2dGrid(cv::Point3d topLeft, cv::Point3d bottomRight, int stepX, int stepY) {
    VectorOfPoints newPoints;
    int gridSizeX = (abs(topLeft.x) + abs(bottomRight.x))  / stepX +1;
    int gridSizeY = (abs(topLeft.y) + abs(bottomRight.y))  / stepY +1;

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





Mesh Mesh::selectGrid(cv::Point3d topLeft, cv::Point3d bottomRight, int stepX, int stepY) {
    VectorOfPoints newPoints;
    VectorOfColors newColors;

    cv::Point2d actualPoint(topLeft.x, topLeft.y);
    int gridSizeX = (abs(topLeft.x) + abs(bottomRight.x)) / 2 / stepX;
    int gridSizeY = (abs(topLeft.y) + abs(bottomRight.y)) / 2 / stepY;
    int r;

    qDebug() << "gridSizeX: " << gridSizeX;
    qDebug() << "gridSizeY: " << gridSizeY;

    for(int gridX = 0; gridX < gridSizeX; gridX++) {
        for(int gridY = 0; gridY < gridSizeY; gridY++) {

            r = getClosed2dPoint(actualPoint);
            qDebug() << "actual point: " << actualPoint.x << actualPoint.y;
            qDebug() << "closed point: " << pointsMat(r, 0) << pointsMat(r, 1);
            qDebug();

            newPoints.append(cv::Point3d(pointsMat(r, 0), pointsMat(r, 1), pointsMat(r, 2)));
            if (colors.count() > 0){
                newColors.append(colors[r]);
            }
            //next

        }
        actualPoint.x += stepX;
    }

    Mesh result = Mesh::fromPointcloud(newPoints, false, true);
    result.colors = newColors;

    return result;
}
