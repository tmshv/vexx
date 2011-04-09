# -------------------------------------------------
# Project created by QtCreator 2010-06-09T14:26:27
# -------------------------------------------------
QT += script scripttools
INCLUDEPATH += ../../ \
    ../../../EksCore \
    ../../../shift \
    ../../../alter2/plugins/UI/ \
    ../../../alter2/plugins/ShiftAlter/
DESTDIR = ../../../bin
LIBS += -L../../../bin \
    -lalter \
    -lEksCore \
    -lUI \
    -lshift \
    -lShiftAlter
TARGET = script
TEMPLATE = lib
DEFINES += SCRIPT_BUILD
SOURCES += scplugin.cpp \
    scsurface.cpp \
    scshiftdynamicpropertyinformation.cpp \
    scshiftproperty.cpp \
    scembeddedtypes.cpp \
    scshiftpropertycontainer.cpp \
    scshiftentity.cpp \
    scshiftdatabase.cpp \
    scshiftfloatarrayproperty.cpp
HEADERS += scplugin.h \
    scglobal.h \
    scsurface.h \
    scshiftdynamicpropertyinformation.h \
    scshiftproperty.h \
    scembeddedtypes.h \
    scwrappedclass.h \
    scshiftpropertycontainer.h \
    scshiftentity.h \
    scshiftdatabase.h \
    scshiftfloatarrayproperty.h

RESOURCES += \
    ScResources.qrc

OTHER_FILES += \
    CoreUtils.js
