#-------------------------------------------------
#
# Project created by QtCreator 2009-08-21T23:47:31
#
#-------------------------------------------------

QT       -= gui

TARGET = mathTest
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += ../../include
LIBS += -L ../../../bin -ljLib -lGLEW -L/usr/lib -lMagickCore -lglut

INCLUDEPATH += ../../include/

TEMPLATE = app

SOURCES += main.cpp

QMAKE_CXXFLAGS += -msse
