# -------------------------------------------------
# Project created by QtCreator 2009-11-09T10:40:50
# -------------------------------------------------
TARGET = entityHelpers
TEMPLATE = lib
DEFINES += ENTITYHELPERS_LIBRARY
SOURCES += EntityHelpers.cpp \
    propertiesDock.cpp \
    explorerDock.cpp
HEADERS += EntityHelpers.h \
    propertiesDock.h \
    explorerDock.h
DESTDIR = ../../plugins/
INCLUDEPATH += ../../SDK \
    ../../../jLib/include/ \
    ./qtpropertybrowser/src \
    ../../../widgetLibrary/ \
    ../../../EksCore
FORMS += explorerDock.ui
QMAKE_CXXFLAGS += -msse
LIBS += -L../../../bin \
    -lalter \
    -ljLib \
    -lEksGui \
    -lEksCore
