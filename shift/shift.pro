# -------------------------------------------------
# Project created by QtCreator 2010-06-09T09:04:56
# -------------------------------------------------

TARGET = shift
TEMPLATE = lib

include("../EksCore/GeneralOptions.pri")

INCLUDEPATH += ../alter2/ \
    $$ROOT/EksCore \
    $$ROOT/EksGui

LIBS += -lEksCore \
    -lEksGui

SOURCES += \
    sentity.cpp \
    sproperty.cpp \
    sdatabase.cpp \
    schange.cpp \
    sobserver.cpp \
    sentityui.cpp \
    spropertydefaultui.cpp \
    sreferenceentity.cpp \
    spropertycontainer.cpp \
    spropertyarray.cpp \
    sbaseproperties.cpp \
    spropertyinformation.cpp \
    sarrayproperty.cpp \
    sdatabasemodel.cpp \
    sdynamicpropertyinformation.cpp \
    sprocessmanager.cpp \
    siterator.cpp \
    sxmlio.cpp \
    sbinaryio.cpp \
    styperegistry.cpp \
    sbasepointerproperties.cpp

HEADERS += \
    sglobal.h \
    sentity.h \
    sproperty.h \
    sdatabase.h \
    schange.h \
    sobserver.h \
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
    sdynamicpropertyinformation.h \
    sentityweakpointer.h \
    sprocessmanager.h \
    siterator.h \
    sxmlio.h \
    sbinaryio.h \
    styperegistry.h \
    sbasepointerproperties.h
