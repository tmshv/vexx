#-------------------------------------------------
#
# Project created by QtCreator 2010-10-31T08:46:00
#
#-------------------------------------------------

TARGET = SynapseCore
TEMPLATE = lib

include("../../EksCore/GeneralOptions.pri")

SOURCES += syplugin.cpp \
    synode.cpp \
    syimagebase.cpp \
    syimageinput.cpp \
    syimageoutput.cpp \
    sydocument.cpp \
    sysourcenode.cpp

HEADERS += syplugin.h \
    syglobal.h \
    synode.h \
    syimagebase.h \
    syimageinput.h \
    syimageoutput.h \
    sydocument.h \
    sysourcenode.h

INCLUDEPATH += $$ROOT/EksCore $$ROOT/Shift $$ROOT/Shift/GraphicsCore $$ROOT/Shift/MathsCore $$ROOT/alter2 $$ROOT/alter2/plugins/ShiftAlter

LIBS += -lshift -lEksCore -lalter -lShiftAlter -lShiftGraphicsCore -lShiftMathsCore
