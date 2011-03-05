# -------------------------------------------------
# Project created by QtCreator 2009-11-09T12:18:21
# -------------------------------------------------
QT += xml
TARGET = baseFileTypes
TEMPLATE = lib
DEFINES += XMLFILETYPE_LIBRARY
INCLUDEPATH += ../../SDK \
    ../../../EksCore
LIBS += -lalter \
    -L../../../bin \
    -lEksCore
DESTDIR = ../../../bin/plugins
HEADERS += xmlFileType.h \
    binaryFileType.h
SOURCES += xmlFileType.cpp \
    baseFileTypes.cpp \
    binaryFileType.cpp
