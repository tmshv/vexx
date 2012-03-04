#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T15:56:15
#
#-------------------------------------------------
TARGET = ShiftMeshCore
TEMPLATE = lib

include("../../EksCore/GeneralOptions.pri")

HEADERS += MeshCore.h \
    MCCube.h \
    MCKernel.h \
    MCPolyhedron.h \
    MCCGALSetup.h

INCLUDEPATH += $$ROOT/shift/GraphicsCore $$ROOT/EksCore $$ROOT/Eks3D/include $$ROOT/Shift $$ROOT/shift/MeshCore

LIBS += -lshift -lEksCore -lEks3D -lShiftGraphicsCore

SOURCES += \
    MeshCore.cpp \
    MCCube.cpp \
    MCPolyhedron.cpp





