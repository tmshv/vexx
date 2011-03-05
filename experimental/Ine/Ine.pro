#-------------------------------------------------
#
# Project created by QtCreator 2010-10-30T11:39:16
#
#-------------------------------------------------

TARGET = Ine
TEMPLATE = lib

DEFINES += INE_BUILD

SOURCES +=

HEADERS += IneManager.h \
    IneGlobal.h \
    IneInterface.h \
    IneDriver.h

INCLUDEPATH += ../EksCore

LIBS += -L../bin -lEksCore

DESTDIR = ../bin
