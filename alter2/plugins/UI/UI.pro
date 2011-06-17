#-------------------------------------------------
#
# Project created by QtCreator 2010-10-30T17:13:16
#
#-------------------------------------------------

TARGET = UI
TEMPLATE = lib

include("../../../EksCore/GeneralOptions.pri")

SOURCES += UIPlugin.cpp \
    UISurface.cpp \
    UIWorkspace.cpp \
    UIVerticalLayout.cpp \
    UIHorizontalLayout.cpp \
    UIStackedLayout.cpp \
    UILayout.cpp \
    UIDatabaseDebugSurface.cpp \
    UIGraphDebugSurface.cpp \
    UISurfacePrivate.cpp \
    UIProfilerSurface.cpp

HEADERS += UIPlugin.h \
    UIGlobal.h \
    UISurface.h \
    UIWorkspace.h \
    UIVerticalLayout.h \
    UIHorizontalLayout.h \
    UIStackedLayout.h \
    UILayout.h \
    UIDatabaseDebugSurface.h \
    UIGraphDebugSurface.h \
    UISurfacePrivate.h \
    UIProfilerSurface.h


INCLUDEPATH += ../../ $$ROOT/EksCore $$ROOT/Eks3D/include $$ROOT/Alter2/plugins/ShiftAlter $$ROOT/Shift $$ROOT/Shift/GraphicsCore

LIBS += -lEksCore -lEks3D -lalter -lShiftAlter -lShift -lShiftGraphicsCore

