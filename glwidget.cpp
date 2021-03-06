#include "glwidget.h"

#include <opencv/cvaux.h> /*bolo tu cvaux.hpp*/
#include <opencv/cv.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <QColor>

GLWidget::GLWidget(QWidget* parent) : QGLWidget(parent) {
    init();
}

GLWidget::~GLWidget() {
}

/**
 * @brief Set size of window
 * @return Window size
 */
QSize GLWidget::sizeHint() const {
     return QSize(600, 600);
}

/**
 * @brief Minimum size of window
 * @return Size of window
 */
QSize GLWidget::minimumSizeHint() const {
     return QSize(200, 200);
}

/**
 * @brief Initialization.
 */
void GLWidget::init() {
    //refreshData();
    xnew = ynew = znew = xold = yold = zold = xx1 = yy1 = zz1 = stav = 0;
    //startTimer(100);
}

/**
 * @brief Delete all painted object.
 */
void GLWidget::deleteAll() {
    for (int i = 0; i < faces.count(); i++)
        delete faces[i];
    faces.clear();
}

/**
 * @brief Clear all painted object.
 */
void GLWidget::clearAll() {
    faces.clear();
}

/**
 * @brief Initializa OpenGL
 */
void GLWidget::initializeGL() {
    glClearColor(0.78f,0.78f,0.78f,0.0f);

    //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    //glEnable( GL_TEXTURE_2D );
    //glEnable( GL_CULL_FACE );
    glEnable( GL_DEPTH_TEST );

    //glEnable(GL_COLOR_MATERIAL);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

/**
 * @brief Paint faces.
 */
void GLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -400.0);
    glTranslatef(0, 0, znew);
    glRotatef(xnew, 1, 0, 0);
    glRotatef(ynew, 0, 1, 0);

    //if (!face) return;

    //float color[] = {244/255.0f,164/255.0f,96/255.0f};

    //float color[] = {214.0/255.0, 180.0/255.0, 155.0/255.0};

    //glColor3fv(color);


	//paint faces
	foreach(const Mesh *face, faces) {
        float color[] = {face->_color.redF(), face->_color.greenF(), face->_color.blueF()};
        glColor3fv(color);
        if (face->triangles.count() == 0) {
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);

            glBegin(GL_POINTS);
			for (int i = 0; i < face->pointsMat.rows; i++) {
				const Matrix &p = face->pointsMat;
				//const Color &c = face->colors[i];
				//glColor3b(c[2]/2,c[1]/2,c[0]/2);
                glVertex3f(p(i, 0)/10, p(i, 1)/10, p(i, 2)/10);

            }
            glEnd();
        } else {

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, color);


            glShadeModel(GL_SMOOTH);//nefunguje
            glPointSize(1);
            glBegin(GL_TRIANGLES);
            int c = face->triangles.count();
            const Matrix &p = face->pointsMat;
			for (int i = 0; i < c; i++) {
                int p1 = face->triangles[i][0];
                int p2 = face->triangles[i][1];
                int p3 = face->triangles[i][2];

                // U = p2 - p1
                float ux = p(p2, 0) - p(p1, 0);
                float uy = p(p2, 1) - p(p1, 1);
                float uz = p(p2, 2) - p(p1, 2);

                // V = p3 - p1
                float vx = p(p3, 0) - p(p1, 0);
                float vy = p(p3, 1) - p(p1, 1);
                float vz = p(p3, 2) - p(p1, 2);

                // n = cross(U,V)
                float nx = uy*vz - uz*vy;
                float ny = uz*vx - ux*vz;
                float nz = ux*vy - uy*vx;

                // normalize
                float size = 1.0f/sqrt(nx*nx + ny*ny + nz*nz);
                nx = nx * size;
                ny = ny * size;
                nz = nz * size;

                // flat shading
                glNormal3f(nx, ny, nz);

                glVertex3f(p(p1, 0), p(p1, 1), p(p1, 2));
                glVertex3f(p(p2, 0), p(p2, 1), p(p2, 2));
                glVertex3f(p(p3, 0), p(p3, 1), p(p3, 2));
            }

            glEnd();
        }
    }

	// paint asix
	glLineWidth(2);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glBegin(GL_LINES);

	//x
	glColor3f(1, 0, 1); //purple
	glVertex3f(0, 0, 0);
	glVertex3f(200, 0, 0);

	//y
	glColor3f(1, 1, 0); //yellow
	glVertex3f(0, 0, 0);
	glVertex3f(0, 200, 0);

	//z
	glColor3f(0, 0, 1); //blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 200);


	//temporary lines
	glColor3f(0, 1, 1); //blue
	glVertex3f(0, 100, 60);
	glVertex3f(0, -100, 60);

	glVertex3f(40, 100, 30);
	glVertex3f(40, -100, 30);

	glVertex3f(-40, 100, 30);
	glVertex3f(-40, -100, 30);



	glEnd();
}

/**
 * @brief Occurs when widget is resized
 * @param width New width
 * @param height New height
 */
void GLWidget::resizeGL(int width, int height) {
    //proces resize keep good aspect ratio for 3D scene
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int side = qMax(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.01, 1000.0);
    //GLKMatrix4MakePerspective(45.0, 1.0, 0.01, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  int x = event->y();
  int y = event->x();

  if (event->button() == Qt::LeftButton) {             /* leve tlacitko aktivuje rotaci */
      stav = 1;                                 /* nastaveni pro funkci motion */
      xx1 = x;                                  /* zapamatovat pozici kurzoru mysi */
      yy1 = y;
  }
  if (event->button() == Qt::RightButton) {            /* prave tlacitko aktivuje posun */
      stav = 2;                                 /* nastaveni pro funkci motion */
      zz1 = y;                                  /* zapamatovat pozici kurzoru mysi */

  }

  updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    //int x = event->y();
    //int y = event->x();

    if (event->button() == Qt::LeftButton) {             /* leve tlacitko aktivuje rotaci */
        stav = 0;                                 /* normalni stav */
        xold = xnew;                              /* zapamatovat novy pocatek */
        yold = ynew;
    }
    if (event->button() == Qt::RightButton) {            /* prave tlacitko aktivuje posun */
        // = 0;
        zold = znew;                              /* zapamatovat novy pocatek */
    }
    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    //std::cout << "move " << stav << " ";
    int x = event->y();
    int y = event->x();
    //std::cout << x << " " << y << std::endl;
    if (stav == 1) {                              /* stav presunu */
        xnew = xold+x-xx1;                          /* vypocitat novou pozici */
        ynew = yold+y-yy1;
        updateGL();
    }
    if (stav == 2)
    {                              // stav rotace
        znew = zold+y-zz1;         // vypocitat novou pozici
        updateGL();
    }
}
