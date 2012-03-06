#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T15:56:15
#
#-------------------------------------------------
TARGET = ShiftMeshCore
TEMPLATE = lib

include("../../EksCore/GeneralOptions.pri")

HEADERS += MeshCore.h \
    MCKernel.h \
    MCPolyhedron.h \
    MCCGALSetup.h \
    MCGeometry.h \
    MCCuboid.h \
    MCSphere.h

INCLUDEPATH += $$ROOT/shift/GraphicsCore $$ROOT/EksCore $$ROOT/Eks3D/include $$ROOT/Shift $$ROOT/shift/MeshCore

LIBS += -lshift -lEksCore -lEks3D -lShiftGraphicsCore

SOURCES += \
    MeshCore.cpp \
    MCPolyhedron.cpp \
    MCGeometry.cpp \
    MCCuboid.cpp \
    MCSphere.cpp \
    MCCGALCore.cpp \
    MCCGALRandom.cpp \
    MCCGALTimer.cpp





