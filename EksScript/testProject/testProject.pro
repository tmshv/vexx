#-------------------------------------------------
#
# Project created by QtCreator 2012-02-16T11:35:58
#
#-------------------------------------------------

QT       += core
QT       -= gui

TEMPLATE = app
TARGET = testProject

include("../../EksCore/GeneralOptions.pri")

SOURCES += main.cpp


debug:LIBS += -L$$ROOT/EksScript/v8/debuglib/
release:LIBS += -L$$ROOT/EksScript/v8/releaselib/

LIBS += -lEksCore -lEksScript -lv8
INCLUDEPATH += $$ROOT/EksScript/ $$ROOT/EksScript/v8 $$ROOT/EksCore
