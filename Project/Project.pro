#-------------------------------------------------
#
# Project created by QtCreator 2011-08-21T16:21:20
#
#-------------------------------------------------

QT       += core gui xml

TARGET = Project
TEMPLATE = app

include("../EksCore/GeneralOptions.pri")

SOURCES += main.cpp

HEADERS  +=

LIBS += -lEksCore \
    -lshift \
    -lalter \
    -lscript \
    -lShiftAlter \
    -lUI

INCLUDEPATH += ../include \
    $$ROOT/EksCore \
    $$ROOT/Shift \
    $$ROOT/Alter2 \
    $$ROOT/EksCore \
    $$ROOT/Alter2/Plugins/UI \
    $$ROOT/Alter2/Plugins/Script \
    $$ROOT/Alter2/Plugins/ShiftAlter

OTHER_FILES += \
    startup.js \
    Project.js
