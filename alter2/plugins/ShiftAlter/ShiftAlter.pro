#-------------------------------------------------
#
# Project created by QtCreator 2010-10-31T08:46:00
#
#-------------------------------------------------

TARGET = ShiftAlter
TEMPLATE = lib

DEFINES += SHIFTALTER_BUILD

SOURCES += splugin.cpp \
    sappdatabase.cpp
QMAKE_CXXFLAGS += -Wall

HEADERS += splugin.h \
    saglobal.h \
    sappdatabase.h

INCLUDEPATH += ../../../EksCore ../../../Shift ../../

DESTDIR += ../../../bin

LIBS += -L../../../bin -lshift -lEksCore -lalter
