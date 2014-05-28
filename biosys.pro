#-------------------------------------------------
# Martin Michalek, FIT VUTBR
# 2014
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = biosys
TEMPLATE = app

CONFIG   -= app_bundle
CONFIG   += console

#-------------------------------------------------
# opencv libraries
#-------------------------------------------------

# problem 1:
# Support for pkg-config is disabled by default in the Qt package for mac.
# So the qmake is configured to assume that there is no pkg-config on the system.
# problem 2:
# Qt Creator launched by Finder won't have /usr/local/bin in the PATH.
# Need to add export PATH=/usr/local/bin:$PATH to /etc/launchd.conf
#
# => too complitated, add lobraries manually is simplier.
#QT_CONFIG -= no-pkg-config
#CONFIG += link_pkgconfig
#PKGCONFIG += opencv
if(macx) {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9 #if opencv installed through macports
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
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
}

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
    landmarks.cpp \
    controller.cpp \
    comparator.cpp \
    stats.cpp \
    scorenormalizer.cpp \
    scorefusioner.cpp

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
    landmarks.h \
    controller.h \
    comparator.h \
    stats.h \
    scorenormalizer.h \
    scorefusioner.h

