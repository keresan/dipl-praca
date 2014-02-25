#-------------------------------------------------
# Martin Michalek, FIT VUTBR
# 2014
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = diplomka
TEMPLATE = app

#-------------------------------------------------
# opencv libraries
#-------------------------------------------------
INCLUDEPATH += /opt/local/include
LIBS += -L/opt/local/lib
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
    landmarks.cpp \
    landmarkdetector.cpp \
    glwidget.cpp \
    facealigner.cpp \
    common.cpp \
    averageface.cpp

HEADERS  += mainwindow.h \
    mesh.h \
    landmarks.h \
    landmarkdetector.h \
    glwidget.h \
    facealigner.h \
    common.h \
    averageface.h


FORMS    += mainwindow.ui
