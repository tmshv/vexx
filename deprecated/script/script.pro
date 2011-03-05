# -------------------------------------------------
# Project created by QtCreator 2009-11-25T12:56:56
# -------------------------------------------------
QT += script
TARGET = script
TEMPLATE = lib
DEFINES += SCRIPT_LIBRARY
SOURCES += script.cpp \
    scriptDock.cpp \
    highlighter.cpp \
    appScript.cpp
HEADERS += script.h \
    scriptDock.h \
    highlighter.h \
    appScript.h
INCLUDEPATH += ../alter/SDK
INCLUDEPATH += ../jLib/include
LIBS += -L../bin/ \
    -lalter
FORMS += scriptDock.ui
DESTDIR = ../alter/plugins
RESOURCES += resources.qrc

QMAKE_CXXFLAGS += -msse
