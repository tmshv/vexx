# -------------------------------------------------
# Project created by QtCreator 2010-06-09T07:53:26
# -------------------------------------------------

TARGET = alter
TEMPLATE = lib

include("../EksCore/GeneralOptions.pri")

QT += xml
INCLUDEPATH += $$ROOT/EksCore/
LIBS += -lEksCore

SOURCES += acore.cpp \
    aabstractplugin.cpp

HEADERS += acore.h \
    aglobal.h \
    aabstractplugin.h \
    aplugin.h

macx:LIBS += -framework \
    CoreFoundation

