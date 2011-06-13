#-------------------------------------------------
#
# Project created by QtCreator 2010-06-09T08:02:34
#
#-------------------------------------------------

TARGET = Synapse

TEMPLATE = app

INCLUDEPATH += ../Alter2 ../EksCore ../Eks3D/include ../Shift/GraphicsCore ../Alter2/plugins/script ../Alter2/plugins/ShiftAlter ../shift/ SynapseCore/ ../Alter2/plugins/UI

LIBS += -L../bin -lalter -lEksCore -lscript -lShiftAlter -lshift -lShiftGraphicsCore -lSynapseCore -lUI -lEks3D

SOURCES += main.cpp \
    syimagenode.cpp \
    sypreviewviewer.cpp \
    syviewernode.cpp \
    synodecanvas.cpp

DESTDIR = ../bin

OTHER_FILES += scripts/startup.js \
    scripts/nodes/add.js

HEADERS += \
    syimagenode.h \
    sypreviewviewer.h \
    syviewernode.h \
    synodecanvas.h
