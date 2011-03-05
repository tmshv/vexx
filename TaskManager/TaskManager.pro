# -------------------------------------------------
# Project created by QtCreator 2010-09-12T19:47:05
# -------------------------------------------------
TARGET = TaskManager
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    item.cpp \
    taskdatabase.cpp
HEADERS += mainwindow.h \
    item.h \
    taskdatabase.h \
    taskglobal.h
INCLUDEPATH += ../EksCore/ \
    ../Shift/
LIBS += -L../bin \
    -lShift \
    -lEksCore
DESTDIR = ../bin
FORMS += 
