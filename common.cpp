#include "common.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <iostream>
#include <assert.h>
#include <cmath>
#include <QStringList>

/*
void Common::printMatrix(CvMat *m)
{
    assert(m != NULL);
    for (int r = 0; r < m->rows; r++)
    {
        for (int c = 0; c < m->cols; c++)
            std::cout << cvmGet(m, r, c) << " ";
        std::cout << std::endl;
    }
}
*/

void Common::printMatrix(const Matrix &m)
{
    QDebug deb = qDebug();
    for (int r = 0; r < m.rows; r++)
    {
        for (int c = 0; c < m.cols; c++)
            deb << m(r,c);
            //std::cout << m(r,c) << " ";
        deb << "\n";
        //std::cout << std::endl;
    }
}

void Common::delay(int sec)
{
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/*
bool Common::matrixContainsNan(const Matrix &m)
{
	int rows = m.rows;
	int cols = m.cols;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
            double val = m(r,c);
			if (val != val)
			{
				return true;
			}
		}
	}
	return false;
}

void Common::savePlot(const QVector<double> &x, const QVector<double> &y, const QVector<double> &z, const QString &path)
{
    int n = x.count();
    assert(n == y.count());
    assert(n == z.count());

    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    for (int i = 0; i < n; i++)
    {
        out << x[i] << " " << y[i] << " " << z[i] << "\n";
    }
}

void Common::savePlot(const QVector<cv::Point3d> &values, const QString &path, bool append)
{
    int n = values.count();

    QFile file(path);

    if (append)
    {
        file.open(QIODevice::Append);
    }
    else
    {
        file.open(QIODevice::WriteOnly);
    }

    QTextStream out(&file);

    if (append)
    {
        out << "\n";
    }

    for (int i = 0; i < n; i++)
    {
        out << values[i].x << " " << values[i].y << " " << values[i].z << "\n";
    }
}

void Common::savePlot(const QVector<double> &x, const QVector<double> &y, const QString &path)
{
    int n = x.count();
    assert(n == y.count());

    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    for (int i = 0; i < n; i++)
    {
        out << x[i] << " " << y[i] << "\n";
    }
}

void Common::savePlot(const QVector<double> values[], int axisCount, const QString &path)
{
    int n = values[0].count();

    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < axisCount; j++)
        {
            out << values[j][i] << " ";
        }

        out <<  "\n";
    }
}

void Common::saveMatrix(const Matrix &m, const QString &path)
{
    cv::FileStorage storage(path.toStdString(), cv::FileStorage::WRITE);
    storage << "m" << m;
}

Matrix Common::loadMatrix(const QString &path)
{
    cv::FileStorage storage(path.toStdString(), cv::FileStorage::READ);
    Matrix m;
    storage["m"] >> m;
    return m;
}

void Common::saveMap(QMap<double, double> &map, const QString &path)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    QList<double> keys = map.keys();
    for (int i = 0; i < keys.count(); i++)
    {
        double key = keys[i];
        double value = map[key];

        out << key << " " << value << "\n";
    }
}
*/
double euclideanDistance(const cv::Point3d &p1, const cv::Point3d &p2)
{
    double dx = p1.x-p2.x;
    double dy = p1.y-p2.y;
    double dz = p1.z-p2.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

double euclideanDistance(const cv::Point &p1, const cv::Point &p2)
{
    double dx = p1.x-p2.x;
    double dy = p1.y-p2.y;
    return sqrt(dx*dx + dy*dy);
}
/*
double Common::absSum(Matrix &m)
{
    double sum = 0;
    for (int r = 0; r < m.rows; r++)
    {
        for (int c = 0; c < m.cols; c++)
        {
            double val = m(r,c);
            sum += fabs(val);
        }
    }
    return sum;
}
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
    for (int i = 0; i < n; i++)
    {
        float x = points[i].x;
        float y = points[i].y;
        fPoints[i] = cv::Point2f(x, y);
        coord2Index[QPair<float, float>(x, y)] = i;
    }
    subdiv.insert(fPoints);

    std::vector<cv::Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);

    //int edge;
    for (unsigned int i = 0; i < triangleList.size(); i++)
    {
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

        //subdiv.locate(p1, edge, v1);
        //subdiv.locate(p2, edge, v2);
        //subdiv.locate(p3, edge, v3);

        cv::Vec3i triangleIndicies;
        triangleIndicies[0] = coord2Index[QPair<float, float>(p1.x, p1.y)];
        triangleIndicies[1] = coord2Index[QPair<float, float>(p2.x, p2.y)];
        triangleIndicies[2] = coord2Index[QPair<float, float>(p3.x, p3.y)];

        result.append(triangleIndicies);
    }
    return result;
}

