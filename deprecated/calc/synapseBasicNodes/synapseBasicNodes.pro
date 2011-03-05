# -------------------------------------------------
# Project created by QtCreator 2010-04-19T13:53:33
# -------------------------------------------------
TARGET = synapseBasicNodes
TEMPLATE = lib
DEFINES += SYNAPSEBASICNODES_LIBRARY
SOURCES += synapsebasicnodes.cpp \
    videoNode.cpp \
    transformNodes.cpp \
    switchNode.cpp \
    rotoshapeNode.cpp \
    reorderNode.cpp \
    imageNode.cpp
HEADERS += videoNode.h \
    transformNodes.h \
    switchNode.h \
    rotoshapeNode.h \
    reorderNode.h \
    imageNode.h
INCLUDEPATH += ../ \
    ../../alter/SDK \
    ../../EksCore/ \
    ../../jLib/include
LIBS += -L../../bin/ \
    -L../../bin/plugins \
    -lalter \
    -lEksCore \
    -lcalc \
    -ljLib
DESTDIR = ../../bin/synapseNodes
