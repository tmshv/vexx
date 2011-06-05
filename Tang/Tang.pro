# -------------------------------------------------
# Project created by QtCreator 2010-04-23T12:10:30
# -------------------------------------------------
TARGET = Tang

QT += opengl \
    xml \
    network \
    webkit

TEMPLATE = app
DESTDIR = ../bin/
SOURCES += main.cpp \
    Interface.cpp \
    viewport.cpp \
    NewItemDialog.cpp \
    EnvironmentEntity.cpp \
    assettree.cpp \
    application.cpp \
    webview.cpp \
    objectid.cpp
LIBS += -L../bin \
    -lEksCore \
    -lEks3D \
    -lshift \
    -lalter \
    -lscript \
    -lUI
INCLUDEPATH += ../include \
    ../EksCore \
    ../Eks3D/include \
    ../Shift \
    ../Alter2 \
    ../EksCore \
    ../Alter2/Plugins/UI \
    ../Alter2/Plugins/Script
HEADERS += Interface.h \
    viewport.h \
    NewItemDialog.h \
    EnvironmentEntity.h \
    assettree.h \
    application.h \
    webview.h \
    objectid.h

FORMS += NewItemDialog.ui

RESOURCES += \
    TangResources.qrc
