# -------------------------------------------------
# Project created by QtCreator 2010-06-09T09:04:56
# -------------------------------------------------
TARGET = shift
TEMPLATE = lib
DESTDIR += ../bin
INCLUDEPATH += ../alter2/ \
    ../EksCore \
    ../EksGui
LIBS += -L../bin \
    -lEksCore \
    -lEksGui
DEFINES += SHIFT_BUILD
SOURCES += \
    sentity.cpp \
    sproperty.cpp \
    sdatabase.cpp \
    schange.cpp \
    sobserver.cpp \
    spropertydata.cpp \
    sentityui.cpp \
    spropertydefaultui.cpp \
    sreferenceentity.cpp \
    spropertycontainer.cpp \
    spropertyarray.cpp \
    sbaseproperties.cpp \
    spropertyinformation.cpp \
    sarrayproperty.cpp \
    sdatabasemodel.cpp \
    sdynamicpropertyinformation.cpp
HEADERS += \
    sglobal.h \
    sentity.h \
    sproperty.h \
    sdatabase.h \
    schange.h \
    sobserver.h \
    spropertydata.h \
    sentityui.h \
    spropertydefaultui.h \
    sreferenceentity.h \
    spropertycontainer.h \
    spropertyarray.h \
    sbaseproperties.h \
    spropertyinformation.h \
    sloader.h \
    sarrayproperty.h \
    sdatabasemodel.h \
    sdynamicpropertyinformation.h
DESTDIR = ../bin

QMAKE_CXXFLAGS += -Wall
