#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T15:56:15
#
#-------------------------------------------------
TARGET = ShiftGraphicsCore
TEMPLATE = lib

QT += opengl

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
    3D/GCTransform.h \
    3D/GCRenderToTexture.h \
    3D/GCRenderToScreen.h \
    3D/GCRenderTarget.h \
    3D/GCGeometry.h \
    3D/GCCamera.h \
    3D/GCRenderable.h \
    3D/Renderable/GCGrid.h \
    3D/GCScene.h \
    3D/GCViewport.h \
    3D/GCShadingGroup.h \
    3D/GCShader.h \
    3D/Renderable/GCCuboid.h

INCLUDEPATH += $$ROOT/shift/GraphicsCore $$ROOT/EksCore $$ROOT/Eks3D/include $$ROOT/Shift

LIBS += -lshift -lEksCore -lEks3D

SOURCES += \
    GraphicsCore.cpp \
    GCImage.cpp \
    GCImageChannel.cpp \
    GCShiftRenderModel.cpp \
    GCAbstractNodeDelegate.cpp \
    GCNodeController.cpp \
    GCQImage.cpp \
    3D/GCTransform.cpp \
    3D/GCRenderToTexture.cpp \
    3D/GCRenderToScreen.cpp \
    3D/GCRenderTarget.cpp \
    3D/GCGeometry.cpp \
    3D/GCCamera.cpp \
    3D/GCRenderable.cpp \
    3D/Renderable/GCGrid.cpp \
    3D/GCScene.cpp \
    3D/GCViewport.cpp \
    3D/GCShadingGroup.cpp \
    3D/GCShader.cpp \
    3D/Renderable/GCCuboid.cpp
