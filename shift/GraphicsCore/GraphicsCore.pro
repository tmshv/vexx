#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T15:56:15
#
#-------------------------------------------------
TARGET = ShiftGraphicsCore
TEMPLATE = lib

DEFINES += SHIFTGRAPHICSCORE_BUILD
QMAKE_CXXFLAGS += -Wall

HEADERS += GCGlobal.h \
    GraphicsCore.h \
    GCImage.h \
    GCImageChannel.h \
    GCShiftRenderModel.h \
    GCAbstractNodeDelegate.h \
    GCNodeController.h \
    GCQImage.h

INCLUDEPATH += ../../EksCore ../../Eks3D/include ../../Shift

DESTDIR += ../../bin

LIBS += -L../../bin -lshift -lEksCore -lEks3D

SOURCES += \
    GraphicsCore.cpp \
    GCImage.cpp \
    GCImageChannel.cpp \
    GCShiftRenderModel.cpp \
    GCAbstractNodeDelegate.cpp \
    GCNodeController.cpp \
    GCQImage.cpp
