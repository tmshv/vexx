# -------------------------------------------------
# Project created by QtCreator 2009-08-10T11:33:45
# -------------------------------------------------
QT += network
INCLUDEPATH += ../include/
DEFINES += DATABASE_BUILD
TARGET = database
TEMPLATE = lib

SOURCES += ../src/fieldManager.cpp \
    ../src/field.cpp \
    ../src/binaryStream.cpp \
    ../src/table.cpp \
    ../src/networkSocket.cpp \
    ../src/networkServer.cpp

HEADERS += ../include/table.h \
    ../include/fieldManager.h \
    ../include/field.h \
    ../include/dataField.h \
    ../include/database.h \
    ../include/binaryStream.h \
    ../include/utility.h \
    ../include/networkSocket.h \
    ../include/networkServer.h

FORMS += networkServerLog.ui

DESTDIR = ../../bin
