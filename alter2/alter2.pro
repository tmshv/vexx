
# -------------------------------------------------
# Project created by QtCreator 2010-06-09T07:53:26
# -------------------------------------------------
QT += xml
TARGET = alter
TEMPLATE = lib
DESTDIR = ../bin/
INCLUDEPATH += ../EksCore/
LIBS += -L../bin \
    -lEksCore
DEFINES += ALTER_BUILD
SOURCES += acore.cpp \
    aabstractplugin.cpp
QMAKE_CXXFLAGS += -Wall
HEADERS += acore.h \
    aglobal.h \
    aabstractplugin.h \
    aplugin.h
macx:LIBS += -framework \
    CoreFoundation
