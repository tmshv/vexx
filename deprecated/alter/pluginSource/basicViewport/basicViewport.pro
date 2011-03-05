# -------------------------------------------------
# Project created by QtCreator 2009-11-09T11:02:33
# -------------------------------------------------
QT += opengl
TARGET = basicViewport
TEMPLATE = lib
DEFINES += VIEWPORT_LIBRARY
SOURCES += BasicViewport.cpp \
    viewport.cpp \
    boundingBox.cpp \
    background.cpp \
    viewportDock.cpp \
    viewportEntity.cpp
HEADERS += BasicViewport.h \
    background.h \
    viewport.h \
    boundingBox.h \
    viewportDock.h \
    viewportTool.h \
    viewportEntity.h \
    viewportGlobal.h
INCLUDEPATH += ../../SDK \
    ../../../jLib/include/ \
    ../../../EksCore
DESTDIR = ../../plugins/
QMAKE_CXXFLAGS += -msse
LIBS += -L../../../bin \
    -lalter \
    -ljLib \
    -lEksCore
