#-------------------------------------------------
#
# Project created by QtCreator 2014-06-11T21:01:50
#
#-------------------------------------------------

QT       += core gui widgets opengl

TARGET = QTTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    geometryengine.cpp \
    mainwidget.cpp

HEADERS  += mainwindow.h \
    geometryengine.h \
    mainwidget.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    shaders.qrc \
    texture.qrc

