# -------------------------------------------------
# Project created by QtCreator 2010-04-21T15:51:26
# -------------------------------------------------
TARGET = EksAdd
TEMPLATE = lib
QT += core
DEFINES += EKSADD_BUILD
LIBS += -L../../bin/ \
    -lEksCore
DESTDIR = ../../bin
HEADERS += ../include/XAddGlobal.h \
    ../include/XNodeDefinition.h \
    ../include/XNetwork.h \
    ../include/XNodeObserver.h
SOURCES += ../src/XNodeDefinition.cpp \
    ../src/XNetwork.cpp \
    ../src/XNodeObserver.cpp
INCLUDEPATH += ../include \
    ../../EksCore/
