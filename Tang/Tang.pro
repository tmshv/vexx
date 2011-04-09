# -------------------------------------------------
# Project created by QtCreator 2010-04-23T12:10:30
# -------------------------------------------------
TARGET = Tang

QT += opengl \
    xml \
    network

TEMPLATE = app
DESTDIR = ../bin/
SOURCES += main.cpp \
    Interface.cpp \
    viewport.cpp \
    NewItemDialog.cpp \
    EnvironmentEntity.cpp \
    assettree.cpp \
    application.cpp
LIBS += -L../bin \
    -lEksCore \
    -lEks3D \
    -lshift \
    -lalter \
    -lUI
INCLUDEPATH += ../include \
    ../EksCore \
    ../Eks3D/include \
    ../Shift \
    ../Alter2 \
    ../EksCore \
    ../Alter2/Plugins/UI
HEADERS += Interface.h \
    viewport.h \
    NewItemDialog.h \
    EnvironmentEntity.h \
    assettree.h \
    application.h

FORMS += NewItemDialog.ui

RESOURCES += \
    TangResources.qrc
