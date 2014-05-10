#-------------------------------------------------
# Martin Michalek, FIT VUTBR
# 2014
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

TARGET = diplomka
TEMPLATE = app

#-------------------------------------------------
# opencv libraries
#-------------------------------------------------
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib
#INCLUDEPATH += /usr/local/Cellar/opencv/2.4.8.2/include
#LIBS += -L//usr/local/Cellar/opencv/2.4.8.2/lib
#INCLUDEPATH += ~/Users/martin/macports/include
#LIBS += -L~/Users/martin/macports/lib
LIBS += -lopencv_contrib \
	-lopencv_core -v\
	-lopencv_features2d \
	-lopencv_flann \
	-lopencv_gpu \
	-lopencv_highgui \
	-lopencv_imgproc \
	-lopencv_legacy \
	-lopencv_ml \
	-lopencv_objdetect \
	-lopencv_video \
	-lopencv_calib3d

cache()

#-------------------------------------------------
# sources
#-------------------------------------------------

SOURCES += main.cpp\
        mainwindow.cpp \
    mesh.cpp \
    landmarkdetector.cpp \
    glwidget.cpp \
    facealigner.cpp \
    common.cpp \
    averageface.cpp \
    run.cpp \
    depthmap.cpp \
    eigenface.cpp \
    facedivider.cpp \
    landmarks_old.cpp \
    landmarks.cpp \
    controller.cpp \
    comparator.cpp

HEADERS  += mainwindow.h \
    mesh.h \
    landmarkdetector.h \
    glwidget.h \
    facealigner.h \
    common.h \
    averageface.h \
    run.h \
    depthmap.h \
    eigenface.h \
    facedivider.h \
    landmarks_old.h \
    landmarks.h \
    controller.h \
    comparator.h


#FORMS    += mainwindow.ui
