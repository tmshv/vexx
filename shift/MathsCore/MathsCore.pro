#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T15:56:15
#
#-------------------------------------------------
TARGET = ShiftMathsCore
TEMPLATE = lib

include("../../Eks/EksCore/GeneralOptions.pri")

HEADERS += \
    mcglobal.h \
    mcmathsoperation.h \
    mcimage.h \
    mcsource.h \
    mcsimpleadd.h \
    mcsimplemultiply.h \
    mcsimple.h

INCLUDEPATH += $$ROOT/shift/MathsCore $$ROOT/Eks/EksCore $$ROOT/Eks/EksScript $$ROOT/Shift

LIBS += -lshift -lEksCore -lEksScript

SOURCES += \
    mathscore.cpp \
    mcmathsoperation.cpp \
    mcimage.cpp \
    mcsource.cpp \
    mcsimpleadd.cpp \
    mcsimplemultiply.cpp \
    mcsimple.cpp




