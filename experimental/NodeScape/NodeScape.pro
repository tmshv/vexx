# -------------------------------------------------
# Project created by QtCreator 2010-09-09T20:02:00
# -------------------------------------------------
TARGET = NodeScape
TEMPLATE = app
DEFINES += SCAPE_BUILD
SOURCES += main.cpp \
    sccanvas.cpp \
    scnodegraphic.cpp \
    scnode.cpp \
    scconnection.cpp
HEADERS += sccanvas.h \
    scnodegraphic.h \
    scnode.h \
    scglobal.h \
    scconnection.h
INCLUDEPATH += ../EksCore
LIBS += -L../bin \
    -lEksCore
