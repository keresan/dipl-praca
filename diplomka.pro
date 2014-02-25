#-------------------------------------------------
#
# Project created by QtCreator 2014-02-25T14:19:29
#
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
#LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
#LIBS += -lopencv_core -lopencv_highgui
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
        mainwindow.cpp

HEADERS  += mainwindow.h


FORMS    += mainwindow.ui
