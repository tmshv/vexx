#-------------------------------------------------
#
# Project created by QtCreator 2010-06-09T08:02:34
#
#-------------------------------------------------

TARGET = Synapse
TEMPLATE = app
QT += opengl


include("../EksCore/GeneralOptions.pri")

INCLUDEPATH += $$ROOT/Alter2 $$ROOT/EksCore $$ROOT/Eks3D/include $$ROOT/Shift/GraphicsCore $$ROOT/Shift/MathsCore $$ROOT/Alter2/plugins/script $$ROOT/Alter2/plugins/ShiftAlter $$ROOT/shift/ SynapseCore/ $$ROOT/Alter2/plugins/UI

LIBS += -lalter -lEksCore -lscript -lShiftAlter -lshift -lShiftGraphicsCore -lShiftMathsCore -lSynapseCore -lUI -lEks3D

SOURCES += main.cpp \
    syimagenode.cpp \
    synodecanvas.cpp \
    sydocument.cpp \
    sypreviewviewport.cpp \
    syinterface.cpp \
    syviewer.cpp \
    syimagetexture.cpp

OTHER_FILES += scripts/startup.js \
    scripts/nodes/add.js

HEADERS += \
    syimagenode.h \
    synodecanvas.h \
    sydocument.h \
    sypreviewviewport.h \
    syinterface.h \
    syviewer.h \
    syimagetexture.h
