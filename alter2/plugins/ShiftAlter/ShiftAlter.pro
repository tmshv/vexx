#-------------------------------------------------
#
# Project created by QtCreator 2010-10-31T08:46:00
#
#-------------------------------------------------

TARGET = ShiftAlter
TEMPLATE = lib

DEFINES += SHIFTALTER_BUILD

SOURCES += splugin.cpp
QMAKE_CXXFLAGS += -Wall

HEADERS += splugin.h \
    saglobal.h

INCLUDEPATH += ../../../EksCore ../../../Shift ../../

DESTDIR += ../../../bin

LIBS += -L../../../bin -lshift -lEksCore -lalter
