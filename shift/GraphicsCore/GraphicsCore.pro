#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T15:56:15
#
#-------------------------------------------------
TARGET = ShiftGraphicsCore
TEMPLATE = lib

QT += opengl

include("../../Eks/EksCore/GeneralOptions.pri")

HEADERS += GCGlobal.h \
    GraphicsCore.h \
    GCShiftRenderModel.h \
    GCAbstractNodeDelegate.h \
    GCNodeController.h \
    GCBaseProperties.h \
    3D/GCTransform.h \
    3D/GCRenderTarget.h \
    3D/GCTextureRenderTarget.h \
    3D/GCScreenRenderTarget.h \
    3D/GCGeometry.h \
    3D/GCCamera.h \
    3D/GCRenderable.h \
    3D/GCScene.h \
    3D/GCViewport.h \
    3D/GCShadingGroup.h \
    3D/GCShader.h \
    3D/Manipulators/GCManipulator.h \
    3D/Manipulators/GCButtonManipulator.h \
    3D/Manipulators/GCDistanceManipulator.h \
    3D/Manipulators/GCTranslateManipulator.h \
    3D/GCShaderDataBindings.h \
    3D/GCTexture.h \
    GCComposeTransform.h \
    GCComposeVector3.h

INCLUDEPATH += $$ROOT/shift/GraphicsCore $$ROOT/Eks/EksCore $$ROOT/Eks/EksScript $$ROOT/Eks/Eks3D/include $$ROOT/Shift

LIBS += -lshift -lEksCore -lEks3D -lEksScript

SOURCES += \
    GraphicsCore.cpp \
    GCShiftRenderModel.cpp \
    GCAbstractNodeDelegate.cpp \
    GCNodeController.cpp \
    GCBaseProperties.cpp \
    3D/GCTransform.cpp \
    3D/GCRenderTarget.cpp \
    3D/GCTextureRenderTarget.cpp \
    3D/GCScreenRenderTarget.cpp \
    3D/GCGeometry.cpp \
    3D/GCCamera.cpp \
    3D/GCRenderable.cpp \
    3D/GCScene.cpp \
    3D/GCViewport.cpp \
    3D/GCShadingGroup.cpp \
    3D/GCShader.cpp \
    3D/Manipulators/GCManipulator.cpp \
    3D/Manipulators/GCButtonManipulator.cpp \
    3D/Manipulators/GCDistanceManipulator.cpp \
    3D/Manipulators/GCTranslateManipulator.cpp \
    3D/GCShaderDataBindings.cpp \
    3D/GCTexture.cpp \
    GCComposeTransform.cpp \
    GCComposeVector3.cpp






