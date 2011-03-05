#-------------------------------------------------
#
# Project created by QtCreator 2010-06-09T08:02:34
#
#-------------------------------------------------

TARGET = testProject
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../ ../../EksCore ../plugins/IneAlter ../../Ine

LIBS += -L../../bin -lalter -lEksCore -lshift

SOURCES += main.cpp

DESTDIR = ../../bin
