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
    viewport.cpp \
    webview.cpp \
    object.cpp

LIBS += -lEksCore \
    -lEksScript \
    -lEks3D \
    -lshift \
    -lalter \
    -lscript \
    -lShiftAlter \
    -lShiftGraphicsCore \
    -lUI

INCLUDEPATH += ../include \
    $$ROOT/EksCore \
    $$ROOT/EksScript \
    $$ROOT/Eks3D/include \
    $$ROOT/Shift \
    $$ROOT/Shift/GraphicsCore \
    $$ROOT/Alter2 \
    $$ROOT/EksCore \
    $$ROOT/Alter2/Plugins/UI \
    $$ROOT/Alter2/Plugins/Script \
    $$ROOT/Alter2/Plugins/ShiftAlter

HEADERS += \
    viewport.h \
    webview.h \
    object.h
RESOURCES +=







