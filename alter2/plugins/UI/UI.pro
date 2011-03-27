#-------------------------------------------------
#
# Project created by QtCreator 2010-10-30T17:13:16
#
#-------------------------------------------------

TARGET = UI
TEMPLATE = lib

DEFINES += UI_BUILD

QMAKE_CXXFLAGS += -Wall

SOURCES += UIPlugin.cpp \
    UISurface.cpp \
    UIWorkspace.cpp \
    UIVerticalLayout.cpp \
    UIHorizontalLayout.cpp \
    UIStackedLayout.cpp \
    UILayout.cpp \
    UIDatabaseDebugSurface.cpp \
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
    UISurfacePrivate.h \
    UIProfilerSurface.h

DESTDIR += ../../../bin

INCLUDEPATH += ../../ ../../../EksCore ../../../Alter2/plugins/ShiftAlter ../../../Shift

LIBS += -L../../../bin  -lEksCore -lalter -lShiftAlter -lShift
