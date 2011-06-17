# -------------------------------------------------
# Project created by QtCreator 2010-04-23T12:10:30
# -------------------------------------------------

TARGET = Tang
TEMPLATE = app

include("../EksCore/GeneralOptions.pri")

QT += opengl \
    xml \
    network \
    webkit

SOURCES += main.cpp \
    Interface.cpp \
    viewport.cpp \
    NewItemDialog.cpp \
    EnvironmentEntity.cpp \
    assettree.cpp \
    application.cpp \
    webview.cpp \
    objectid.cpp

LIBS += -lEksCore \
    -lEks3D \
    -lshift \
    -lalter \
    -lscript \
    -lUI

INCLUDEPATH += ../include \
    $$ROOT/EksCore \
    $$ROOT/Eks3D/include \
    $$ROOT/Shift \
    $$ROOT/Alter2 \
    $$ROOT/EksCore \
    $$ROOT/Alter2/Plugins/UI \
    $$ROOT/Alter2/Plugins/Script

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
