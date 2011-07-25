# -------------------------------------------------
# Project created by QtCreator 2009-07-16T19:16:41
# -------------------------------------------------
QT += opengl \
    network
TARGET = testProject
TEMPLATE = app

LIBS += -L../../bin -ljLib -lGLEW -lMagickCore -lglut

INCLUDEPATH += ../include/
SOURCES += jlibpatchtest.cpp
HEADERS += jlibpatchtest.h
OTHER_FILES += postFrag2.txt \
    postFrag.txt \
    frag.txt \
    vert.txt \
    combineFrag.txt
