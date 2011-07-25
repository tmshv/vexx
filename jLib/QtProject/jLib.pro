# -------------------------------------------------
# Project created by QtCreator 2009-04-19T14:51:36
# -------------------------------------------------
QT += opengl \
    network
TARGET = jLib
TEMPLATE = lib
DEFINES += JLIB_BUILD \
    GLEW_STATIC
QMAKE_CXXFLAGS += -Wextra
SOURCES += ../external/src/glew.c \
    ../src/jLib.glHeader.cpp
unix:INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += ../include \
    ../external/include \
    ../external/include/glew
unix:!macx { 
    INCLUDEPATH += /usr/include/OpenEXR
    LIBS += -lIlmImf
    DEFINES += JLIB_OPENEXR_SUPPORT
}
HEADERS += ../include/jLib.h \
    ../include/jLib.global.h \
    ../include/jLib.jptr.h \
    ../include/jLib.vector.h \
    ../include/jLib.string.h \
    ../include/jLib.floatLimits.h \
    ../include/jLib.informer.h \
    ../include/jLib.testEnvironment.h \
    ../include/jLib.glHeader.h \
    ../include/jLib.properties.h \
    ../include/jLib.variant.h \
    ../include/jLib.map.h \
    ../include/jLib.list.h
include(error.pri)
include(math.pri)
include(xml.pri)
include(file.pri)
include(graphics.pri)
include(jqt.pri)
SOURCES += ../src/jLib.memory.cpp \
    ../src/jLib.informer.cpp \
    ../src/jLib.variant.cpp
DESTDIR = ../../bin
QMAKE_CXXFLAGS += -msse
