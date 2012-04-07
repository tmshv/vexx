# -------------------------------------------------
# Project created by QtCreator 2010-06-09T07:53:26
# -------------------------------------------------

TARGET = alter
TEMPLATE = lib

include("../Eks/EksCore/GeneralOptions.pri")

QT += xml network
INCLUDEPATH += $$ROOT/Eks/EksCore/
LIBS += -lEksCore

SOURCES += acore.cpp \
    aabstractplugin.cpp \
    adebuginterface.cpp

HEADERS += acore.h \
    aglobal.h \
    aabstractplugin.h \
    aplugin.h \
    adebuginterface.h

macx:LIBS += -framework \
    CoreFoundation

