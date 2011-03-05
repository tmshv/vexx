# -------------------------------------------------
# Project created by QtCreator 2010-03-28T15:27:53
# -------------------------------------------------
TARGET = debugBrowser
TEMPLATE = lib
DEFINES += DEBUGBROWSER_LIBRARY
SOURCES += main.cpp \
    debugDock.cpp \
    entityModel.cpp \
    AEntityModel.cpp
HEADERS += debugDock.h \
    entityModel.h \
    AEntityModel.h
FORMS += debugDock.ui
INCLUDEPATH += ../../SDK \
    ../../../jLib/include/ \
    ../../../EksCore
DESTDIR = ../../../bin/plugins
LIBS += -L../../../bin \
    -lalter \
    -ljLib \
    -lEksCore
