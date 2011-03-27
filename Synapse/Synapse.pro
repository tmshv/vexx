#-------------------------------------------------
#
# Project created by QtCreator 2010-06-09T08:02:34
#
#-------------------------------------------------

TARGET = Synapse

TEMPLATE = app

INCLUDEPATH += ../Alter2 ../EksCore ../ALter2/plugins/script

LIBS += -L../bin -lalter -lEksCore -lscript

SOURCES += main.cpp

DESTDIR = ../bin

OTHER_FILES += scripts/startup.js \
    scripts/nodes/add.js
