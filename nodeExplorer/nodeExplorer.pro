# -------------------------------------------------
# Project created by QtCreator 2009-12-10T18:14:08
# -------------------------------------------------
QT += opengl
TARGET = nodeExplorer
TEMPLATE = lib
DEFINES += NODEEXPLORER_LIBRARY
SOURCES += nodeExplorerDock.cpp \
    main.cpp \
    nodeExplorerWidget.cpp \
    nodeWidget.cpp
HEADERS += nodeExplorerDock.h \
    nodeExplorer.h \
    nodeExplorerWidget.h \
    nodeWidget.h
INCLUDEPATH += ../jLib/include/ \
    ../alter/SDK
LIBS += -L../bin \
    -ljLib
DESTDIR = ../alter/plugins/
