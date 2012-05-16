#-------------------------------------------------
#
# Project created by QtCreator 2010-10-30T17:13:16
#
#-------------------------------------------------

TARGET = UI
TEMPLATE = lib

include("../../../Eks/EksCore/GeneralOptions.pri")

SOURCES += UIPlugin.cpp \
    UISurface.cpp \
    UIDatabaseDebugSurface.cpp \
    UIGraphDebugSurface.cpp \
    UISurfacePrivate.cpp \
    UIProfilerSurface.cpp \
    UIEventLoggerSurface.cpp

HEADERS += UIPlugin.h \
    UIGlobal.h \
    UISurface.h \
    UIDatabaseDebugSurface.h \
    UIGraphDebugSurface.h \
    UISurfacePrivate.h \
    UIProfilerSurface.h \
    UIEventLoggerSurface.h


INCLUDEPATH += ../../ $$ROOT/Eks/EksCore ../../ $$ROOT/Eks/EksScript $$ROOT/Eks/Eks3D/include $$ROOT/Alter2/plugins/ShiftAlter $$ROOT/Shift $$ROOT/Shift/GraphicsCore

LIBS += -lEksCore -lEksScript -lEks3D -lalter -lShiftAlter -lShift -lShiftGraphicsCore



