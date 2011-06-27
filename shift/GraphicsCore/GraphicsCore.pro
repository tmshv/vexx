#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T15:56:15
#
#-------------------------------------------------
TARGET = ShiftGraphicsCore
TEMPLATE = lib

include("../../EksCore/GeneralOptions.pri")

HEADERS += GCGlobal.h \
    GraphicsCore.h \
    GCImage.h \
    GCImageChannel.h \
    GCShiftRenderModel.h \
    GCAbstractNodeDelegate.h \
    GCNodeController.h \
    GCQImage.h \
    GCGeometry.h \
    GCTransform3D.h \
    GCCamera3D.h

INCLUDEPATH += $$ROOT/EksCore $$ROOT/Eks3D/include $$ROOT/Shift

LIBS += -lshift -lEksCore -lEks3D

SOURCES += \
    GraphicsCore.cpp \
    GCImage.cpp \
    GCImageChannel.cpp \
    GCShiftRenderModel.cpp \
    GCAbstractNodeDelegate.cpp \
    GCNodeController.cpp \
    GCQImage.cpp \
    GCGeometry.cpp \
    GCTransform3D.cpp \
    GCCamera3D.cpp
