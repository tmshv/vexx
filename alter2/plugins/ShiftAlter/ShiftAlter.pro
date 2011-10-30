#-------------------------------------------------
#
# Project created by QtCreator 2010-10-31T08:46:00
#
#-------------------------------------------------

TARGET = ShiftAlter
TEMPLATE = lib

include("../../../EksCore/GeneralOptions.pri")


SOURCES += splugin.cpp \
    sappdatabase.cpp \
    saparteditor.cpp \
    saparteditorform.cpp

HEADERS += splugin.h \
    saglobal.h \
    sappdatabase.h \
    saparteditor.h \
    saparteditorform.h

INCLUDEPATH += $$ROOT/EksCore $$ROOT/Shift ../../


LIBS += -lshift -lEksCore -lalter

FORMS += \
    saparteditorform.ui






