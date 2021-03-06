#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <OpenGL/glu.h>

#include "mesh.h"
#include "landmarks.h"

/**
 * @brief Class for painting mesh
 */
class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    double depth[640*480];
    uint8_t rgb[640*480*3];

public:

    GLWidget(QWidget *parent = 0);

    ~GLWidget();
    void resizeGL(int width, int height);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void addFace(const Mesh *f) { faces << f; updateGL(); }
	void addLandmarks(Landmarks *landmarks) { this->landmarks << landmarks; }
    void addCurve(QVector<cv::Point3d> &curve) { curves << curve; }
    void deleteAll();
    void clearAll();

    const Mesh *getFace() { if (faces.empty()) return 0; else return faces[0]; }


protected:
    void init();
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private:
    QVector<const Mesh*> faces;
	QVector<Landmarks*> landmarks;
    QVector<QVector<cv::Point3d> > curves;

    void  line(int i, int j);

    int   xnew, ynew, znew;                   /* soucasna pozice, ze ktere se pocitaji rotace a posuny */
    int   xold, yold, zold;                   /* minula pozice, ze ktere se pocitaji rotace a posuny */
    int   xx1, yy1, zz1;                      /* body, ve kterych se nachazi kurzor mysi */
    int   stav;                               /* stav tlacitek mysi */
};

#endif // GLWIDGET_H
