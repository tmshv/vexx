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
include($$ROOT/EksScript/EksScriptOptions.pri)

SOURCES += main.cpp

INCLUDEPATH += $$ROOT/EksScript/v8
