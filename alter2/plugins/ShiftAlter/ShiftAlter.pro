#-------------------------------------------------
#
# Project created by QtCreator 2010-10-31T08:46:00
#
#-------------------------------------------------

TARGET = ShiftAlter
TEMPLATE = lib

include("../../../Eks/EksCore/GeneralOptions.pri")


SOURCES += splugin.cpp \
    sappdatabase.cpp \
    saparteditor.cpp \
    saparteditorinterface.cpp \
    sashaderparteditorinterface.cpp \
    saviewport.cpp \
    sadocument.cpp

HEADERS += splugin.h \
    saglobal.h \
    sappdatabase.h \
    saparteditor.h \
    saparteditorinterface.h \
    sashaderparteditorinterface.h \
    saviewport.h \
    sadocument.h

INCLUDEPATH += $$ROOT/Eks/EksCore $$ROOT/Eks/EksGui $$ROOT/Eks/EksScript $$ROOT/Eks/Eks3D/include $$ROOT/Shift ../../ $$ROOT/Shift/GraphicsCore $$ROOT/Shift/MathsCore $$ROOT/Shift/MeshCore

QT += opengl
LIBS += -lshift -lEksCore -lEksGui -lEksScript -lEks3D -lalter -lShiftGraphicsCore -lShiftMathsCore -lShiftMeshCore

FORMS +=

RESOURCES +=






















