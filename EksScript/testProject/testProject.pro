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

LIBS += -lEksCore -lEksScript
INCLUDEPATH += $$ROOT/EksScript/ $$ROOT/EksCore
