#-------------------------------------------------
#
# Project created by QtCreator 2012-02-16T11:35:32
#
#-------------------------------------------------

QT       -= gui

TARGET = EksScript
TEMPLATE = lib

include("../EksCore/GeneralOptions.pri")
include("EksScriptOptions.pri")

SOURCES += XScriptGlobal.cpp

HEADERS += XScriptGlobal.h

INCLUDEPATH += $$ROOT/EksScript/v8
