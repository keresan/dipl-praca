#include "averageface.h"
#include "glwidget.h"

/**
 * @brief AverageFace::AverageFace
 * @param pathToLandmarks Path to directory with anoted landmarks
 * @param pathToFaces Path to direcotry with FRGC database models
 * @param startFileLandmark Starting face model
 */
AverageFace::AverageFace(QString pathToLandmarks, QString pathToFaces, QString startFileLandmark) {

	_gridMesh = Mesh::create2dGrid(cv::Point3d(-130,130,0), cv::Point3d(130,-120,0),2,2);

    _pathToLandmarks = pathToLandmarks;
	_pathToFaces = pathToFaces;

	readVector3dPointsFromFile(pathToLandmarks+startFileLandmark, _startVector);

	_startModel = Mesh::fromABS(_pathToFaces+startFileLandmark.split('_').at(0),false);

    cv::Point3d meanV(getMeanPoint(_startVector));

	//qDebug() << "meanV:" << meanV.x << meanV.y << meanV.z;

    _gridMesh.translate(meanV);

    _startModel = _startModel.getExtract2dGrid(_gridMesh);
    _averageModel = _startModel;
    _weight = 1;

	//qDebug() << pathToLandmarks+startFileLandmark.split('_').at(0);

    //get list of files
    _dir.setPath(_pathToLandmarks);
    _dir.setFilter(QDir::Files | QDir::NoSymLinks);
	_dir.setNameFilters(QStringList()<<"*abs_landmarks.txt");
    _listOfFiles = _dir.entryList();
}

/**
 * @brief AverageFace::process
 * Create average face
 * @param resultFilePath Path to save result average face model.
 */
void AverageFace::process(QString resultFilePath) {

    foreach (QString actualFile, _listOfFiles) {

		//qDebug() << _weight << ": actual file:" << actualFile;
        QVector<cv::Point3d> actualVector;
        Mesh actualModel;
        cv::Point3d shift;
        Matrix rotation;

        readVector3dPointsFromFile(_pathToLandmarks+actualFile, actualVector);

        shift = getOptimalTranslation(actualVector, _startVector);
        translate(actualVector,shift);

        rotation = getOptimalRotation(actualVector,_startVector);

		actualModel = Mesh::fromABS(_pathToFaces+actualFile.split('_').at(0), false);

        actualModel.translate(shift);

        actualModel.transform(rotation);

        actualModel = actualModel.getExtract2dGrid(_gridMesh);

        //add matrix to average:

        Mesh::averageMesh(actualModel,_averageModel,_weight);

        _weight++;
    }

    _averageModel.centralize();
	_averageModel.writeOBJ(resultFilePath,'.');
    qDebug() << "ENDE";

}


/**
 * @brief AverageFace::averageMatrices
 * Compute average face.
 * @param src Source matrix
 * @param dst Source and destination matrix
 * @param dstWeight Weight of destination matrix
 */
void AverageFace::averageMatrices(Matrix &src, Matrix &dst, int dstWeight) {

	//qDebug() << "src" <<src.rows << "x" << src.cols;
	//qDebug() << "dst" <<dst.rows << "x" << dst.cols;

    dst = (dst*dstWeight + src) / (dstWeight + 1);
}

/**
 * @brief AverageFace::getMeanPoint
 * Compute mean point
 * @param points Vector of 3D points
 * @return mean 3D point
 */
cv::Point3d AverageFace::getMeanPoint(QVector<cv::Point3d> &points) {
    cv::Point3d mean(0,0,0);
    foreach(const cv::Point3d p, points) {
        mean += p;
    }

    mean.x /= points.count();
    mean.y /= points.count();
    mean.z /= points.count();

    return mean;
}

/**
 * @brief AverageFace::readVector3dPointsFromFile
 * Read vectror of Point3d, on each line expected format: x y z (divide by spaces)
 * @param path Path to file
 * @param v Vector of output points
 */
void AverageFace::readVector3dPointsFromFile(QString path, QVector<cv::Point3d> &v) {
	QFile f(path);
    bool exists = f.exists();
    assert(exists);
    bool opened = f.open(QIODevice::ReadOnly);
    assert(opened);

    cv::Point3d tmp;
    QString line;
    QStringList coords;
    QTextStream in(&f);
    while (!in.atEnd()) {
        line = in.readLine();
        coords = line.split(' ');
        tmp.x = coords.at(0).toDouble();
        tmp.y = coords.at(1).toDouble();
        tmp.z = coords.at(2).toDouble();

        v.append(tmp);
    }
}

/**
 * @brief AverageFace::getOptimalRotation
 * Compute optimal rotation between two sets of points
 * @param from Vector of points which should be rotating
 * @param to Still vector of points
 * @return Optimal rotation, matrix 3x3
 */
Matrix AverageFace::getOptimalRotation(QVector<cv::Point3d> &from, QVector<cv::Point3d> &to) {
    int n = from.count();
    assert(n == to.count());

    // calculate H
    Matrix H = Matrix::zeros(3, 3);
    for (int i = 0; i < n; i++)
    {
        Matrix A = (Matrix(3,1) << from.at(i).x, from.at(i).y, from.at(i).z);
        Matrix B = (Matrix(3,1) << to.at(i).x, to.at(i).y, to.at(i).z);
        H += (A * B.t());
    }

    // SVD(H)
    cv::SVD svd(H);

    // R = VU^T
    Matrix R = svd.vt.t() * svd.u.t();
    return R;
}

/**
 * @brief AverageFace::getOptimalRotation
 * Compute optimal rotation between two mashes
 * @param from Mesh which should be rotating
 * @param to Still mesh
 * @return Optimal rotation, matrix 3x3
 */
Matrix AverageFace::getOptimalRotation(Mesh &from, Mesh &to) {

    assert(from.pointsMat.rows == to.pointsMat.rows);

    // calculate H
    Matrix H = Matrix::zeros(3, 3);
    for (int r = 0; r < from.pointsMat.rows; r++) {
        Matrix A = (Matrix(3,1) << from.pointsMat(r,0), from.pointsMat(r,1), from.pointsMat(r,2));
        Matrix B = (Matrix(3,1) << to.pointsMat(r,0), to.pointsMat(r,1), to.pointsMat(r,2));
        H += (A * B.t());
    }

    // SVD(H)
    cv::SVD svd(H);

    // R = VU^T
    Matrix R = svd.vt.t() * svd.u.t();
    return R;
}

/**
 * @brief AverageFace::getOptimalTranslation
 * Compute optimal translation between two sets of points
 * @param from Vector of points which should be translatin
 * @param to Still vector of points
 * @return Vector of translation
 */
cv::Point3d AverageFace::getOptimalTranslation(QVector<cv::Point3d> &from,QVector<cv::Point3d> &to) {

    cv::Point3d meanFrom = getMeanPoint(from);
    cv::Point3d meanTo = getMeanPoint(to);

    return meanTo - meanFrom;
}

/**
 * @brief AverageFace::getOptimalTranslation
 * Compute optimal translation between two mashes
 * @param from Mesh which should be translating
 * @param to Still mesh
 * @return Vector of translation
 */
cv::Point3d AverageFace::getOptimalTranslation(Mesh &from,Mesh &to) {

    cv::Point3d meanFrom = from.getMeanPoint();
    cv::Point3d meanTo = to.getMeanPoint();

    return meanTo - meanFrom;
}

/**
 * @brief AverageFace::translate
 * Translate each point by adding shift
 * @param v Vector of points to translate
 * @param shift Values of translation
 */
void AverageFace::translate(QVector<cv::Point3d> &v, cv::Point3d shift) {

    for(int i = 0; i < v.count(); i++) {
        v[i] += shift;
    }
}

/**
 * @brief AverageFace::translate
 * Translate matrix by adding shift
 * @param v Matrix to translate
 * @param shift Values of translation
 */
void AverageFace::translate(Matrix &m, cv::Point3d shift) {

    for (int r = 0; r < m.rows; r++) {
        m(r, 0) += shift.x;
        m(r, 1) += shift.y;
        m(r, 2) += shift.z;
    }
}

/**
 * @brief AverageFace::transform
 * Change point position with transform matrix
 * @param p Point to transform
 * @param m Transform matrix
 */
void AverageFace::transform(cv::Point3d &p, const Matrix &m){
    Matrix A = (Matrix(3,1) << p.x, p.y, p.z);
    A = m*A;
	p.x = A(0);
	p.y = A(1);
	p.z = A(2);
}

/**
 * @brief AverageFace::transform
 * Change position of each point in vector with transform matrix
 * @param points Vector of points to transform
 * @param m Transform matrix
 */
void AverageFace::transform(QVector<cv::Point3d> &points, const Matrix &m) {

    int n = points.count();
    for (int i = 0; i < n; i++) {
        cv::Point3d &p = points[i];
        transform(p, m);
    }
}

/**
 * @brief AverageFace::transform
 * Change each position of matrix with transform matrix
 * @param points Matrix of points to transform
 * @param m Transform matrix
 */
void AverageFace::transform(Matrix &points, const Matrix &m) {

    points = points*m.t(); //.inv();
}

/**
 * @brief AverageFace::rotate
 * Rotate point by parameters
 * @param points Point to rotate
 * @param x Rotation in axis X
 * @param y Rotation in axis Y
 * @param z Rotation in axis Z
 */
void AverageFace::rotate(Matrix &points, double x, double y, double z) {
    Matrix Rx = (Matrix(3,3) <<
                 1, 0, 0,
                 0, cos(x), -sin(x),
                 0, sin(x), cos(x));
    Matrix Ry = (Matrix(3,3) <<
                 cos(y), 0, sin(y),
                 0, 1, 0,
                 -sin(y), 0, cos(y));
    Matrix Rz = (Matrix(3,3) <<
                 cos(z), -sin(z), 0,
                 sin(z), cos(z), 0,
                 0, 0, 1);
    Matrix R = Rx*Ry*Rz;

    transform(points, R);
}

/**
 * @brief AverageFace::getRotateMatrix
 * Transform parameters of rotation to rotate matrix
 * @param x Rotation in axis X
 * @param y Rotation in axis Y
 * @param z Rotation in axis Z
 * @return Output matrix of rotaion
 */
Matrix AverageFace::getRotateMatrix(double x, double y, double z) {
    Matrix Rx = (Matrix(3,3) <<
                 1, 0, 0,
                 0, cos(x), -sin(x),
                 0, sin(x), cos(x));
    Matrix Ry = (Matrix(3,3) <<
                 cos(y), 0, sin(y),
                 0, 1, 0,
                 -sin(y), 0, cos(y));
    Matrix Rz = (Matrix(3,3) <<
                 cos(z), -sin(z), 0,
                 sin(z), cos(z), 0,
                 0, 0, 1);
    Matrix R = Rx*Ry*Rz;

    return R;
}
