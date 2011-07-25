# -------------------------------------------------
# Project created by QtCreator 2010-04-21T15:51:26
# -------------------------------------------------
QT += opengl
TARGET = Eks3D
TEMPLATE = lib
DEFINES += EKS3D_BUILD
SOURCES += ../src/XDoodad.cpp \
    ../src/XScene.cpp \
    ../src/XCuboid.cpp \
    ../src/XTransform.cpp \
    ../src/XAbstractRenderer.cpp \
    ../src/XFrameEvent.cpp \
    ../src/XTransformEvent.cpp \
    ../src/XCamera.cpp \
    ../src/XAbstractGeometryCache.cpp \
    ../src/XPrimitives.cpp \
    ../src/XAbstractShader.cpp \
    ../src/glew.c \
    ../src/XAbstractTexture.cpp
HEADERS += ../include/XDoodad.h \
    ../include/X3DGlobal.h \
    ../include/XScene.h \
    ../include/XCuboid.h \
    ../include/XTransform.h \
    ../include/XAbstractRenderer.h \
    ../include/XFrameEvent.h \
    ../include/XTransformEvent.h \
    ../include/XCamera.h \
    ../include/XAbstractGeometryCache.h \
    ../include/XPrimitives.h \
    ../include/XAbstractShader.h \
    ../include/XAbstractTexture.h
INCLUDEPATH += ../include/ \
    ../../EksCore/
LIBS += -L../../bin/ \
    -lEksCore
DESTDIR = ../../bin
