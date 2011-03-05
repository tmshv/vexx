# -------------------------------------------------
# Project created by QtCreator 2010-08-12T19:50:50
# -------------------------------------------------
QT += network \
    xml
TARGET = server
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    Server.cpp
HEADERS += Server.h
LIBS += -L../../bin \
    -lEksCore \
    -lEks3D
INCLUDEPATH += ../include \
    ../../EksCore \
    ../../Eks3D/include
DESTDIR = ../../bin
