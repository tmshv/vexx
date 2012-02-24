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

SOURCES += XScriptGlobal.cpp \
    XInterface.cpp \
    XInterfaceObject.cpp \
    XEngine.cpp \
    XContext.cpp \
    XScript.cpp

HEADERS += XScriptGlobal.h \
    XInterface.h \
    XInterfaceObject.h \
    XEngine.h \
    XContext.h \
    XScript.h

INCLUDEPATH += $$ROOT/EksScript/v8
