#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T15:56:15
#
#-------------------------------------------------
TARGET = ShiftMathsCore
TEMPLATE = lib

include("../../EksCore/GeneralOptions.pri")

HEADERS += \
    mcglobal.h \
    mathscore.h \
    mcmathsoperation.h \
    mcimage.h \
    mcsource.h

INCLUDEPATH += $$ROOT/shift/MathsCore $$ROOT/EksCore $$ROOT/Shift

LIBS += -lshift -lEksCore

SOURCES += \
    mathscore.cpp \
    mcmathsoperation.cpp \
    mcimage.cpp \
    mcsource.cpp




