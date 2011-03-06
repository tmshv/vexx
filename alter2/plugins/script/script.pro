# -------------------------------------------------
# Project created by QtCreator 2010-06-09T14:26:27
# -------------------------------------------------
QT += script scripttools
INCLUDEPATH += ../../ \
    ../../../EksCore \
    ../../../shift \
    ../../../alter2/plugins/UI/
DESTDIR = ../../../bin
LIBS += -L../../../bin \
    -lalter \
    -lEksCore \
    -lUI \
    -lshift
TARGET = script
TEMPLATE = lib
DEFINES += SCRIPT_BUILD
SOURCES += scplugin.cpp \
    scsurface.cpp \
    scshiftdynamicpropertyinformation.cpp \
    scshiftproperty.cpp \
    scembeddedtypes.cpp
HEADERS += scplugin.h \
    scglobal.h \
    scsurface.h \
    scshiftdynamicpropertyinformation.h \
    scshiftproperty.h \
    scembeddedtypes.h \
    scwrappedclass.h

RESOURCES += \
    ScResources.qrc

OTHER_FILES += \
    CoreUtils.js
