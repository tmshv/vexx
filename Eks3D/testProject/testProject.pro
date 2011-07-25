#-------------------------------------------------
#
# Project created by QtCreator 2010-04-23T12:10:30
#
#-------------------------------------------------
TARGET = testProject
CONFIG   += console
CONFIG   -= app_bundle
QT += opengl
TEMPLATE = app


SOURCES += main.cpp

LIBS += -L../../bin -lEksCore -lEks3D

INCLUDEPATH += ../include ../../EksCore
