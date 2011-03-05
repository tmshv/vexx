# -------------------------------------------------
# Project created by QtCreator 2009-07-22T19:46:38
# -------------------------------------------------
TARGET = physiCal
TEMPLATE = lib
INCLUDEPATH += ../include \
    ../../jLib/include \
    ../../EksCore
LIBS += -L../../bin \
    -ljLib \
    -lEksCore
DEFINES += PHYSICAL_LIBRARY
DESTDIR = ../../bin/
include( shape.pri )
HEADERS += ../include/physiCalGlobal.h \
    ../include/physiCal.h \
    ../include/timer.h \
    ../include/collisionArray.h \
    ../include/system.h \
    ../include/frame.h \
    ../include/springMaterial.h \
    ../include/spring.h \
    ../include/boundingCube.h \
    ../include/octree.h \
    ../include/frameLog.h \
    ../include/object.h \
    ../include/particle.h \
    ../include/objectFace.h \
    ../include/objectRef.h \
    ../include/objectLeaf.h \
    ../include/pVector.h
SOURCES += ../src/timer.cpp \
    ../src/collisionArray.cpp \
    ../src/system.cpp \
    ../src/frame.cpp \
    ../src/springMaterial.cpp \
    ../src/spring.cpp \
    ../src/frameLog.cpp \
    ../src/object.cpp \
    ../src/particle.cpp \
    ../src/objectFace.cpp \
    ../src/objectRef.cpp \
    ../src/objectLeaf.cpp \
    ../src/pVector.cpp
QMAKE_CXXFLAGS += -msse
