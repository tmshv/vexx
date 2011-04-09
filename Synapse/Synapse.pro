#-------------------------------------------------
#
# Project created by QtCreator 2010-06-09T08:02:34
#
#-------------------------------------------------

TARGET = Synapse

TEMPLATE = app

INCLUDEPATH += ../Alter2 ../EksCore ../ALter2/plugins/script ../ALter2/plugins/ShiftAlter ../shift/ SynapseCore/

LIBS += -L../bin -lalter -lEksCore -lscript -lShiftAlter -lshift -lSynapseCore

SOURCES += main.cpp \
    syimagenode.cpp

DESTDIR = ../bin

OTHER_FILES += scripts/startup.js \
    scripts/nodes/add.js

HEADERS += \
    syimagenode.h
