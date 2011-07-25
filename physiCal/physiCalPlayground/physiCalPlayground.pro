# -------------------------------------------------
# Project created by QtCreator 2009-10-02T16:14:17
# -------------------------------------------------
QT += opengl
TARGET = physiCalPlayground
TEMPLATE = app
SOURCES += main.cpp \
    materialFront.cpp \
    groundFront.cpp \
    latticeFront.cpp
HEADERS += materialFront.h \
    groundFront.h \
    latticeFront.h
LIBS += -L../../bin \
    -ljLib \
    -lphysiCal \
    -lalter \
    -lglut
unix:LIBS += -lGLEW
win32:LIBS += -L../../jLib/external/lib \
    -lfreetype-6
INCLUDEPATH += ../../jLib/include/ \
    ../include \
    ../../alter/include \
    ../../physiCal/include
QMAKE_CXXFLAGS += -msse
