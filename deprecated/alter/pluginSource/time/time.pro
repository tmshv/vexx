# -------------------------------------------------
# Project created by QtCreator 2009-11-08T18:28:52
# -------------------------------------------------
TARGET = time
TEMPLATE = lib
DEFINES += TIME_LIBRARY
SOURCES += timeExt.cpp \
    timelineWidget.cpp \
    timeDock.cpp \
    timeEntity.cpp \
    timeToolBar.cpp
HEADERS += timeExt.h \
    timelineWidget.h \
    timeDock.h \
    timeEntity.h \
    timeGlobal.h \
    timeToolBar.h
INCLUDEPATH += ../../SDK \
    ../../../jLib/include/ \
    ../../../EksCore
LIBS += -L../../../bin \
    -lalter \
    -ljLib \
    -lEksCore
DESTDIR = ../../plugins/
FORMS += timeDock.ui
QMAKE_CXXFLAGS += -msse
RESOURCES += timeResources.qrc
