# -------------------------------------------------
# Project created by QtCreator 2009-12-28T11:58:19
# -------------------------------------------------
QT += opengl
TARGET = simulation
TEMPLATE = lib
DEFINES += SIMULATION_LIBRARY
SOURCES += simulation.cpp \
    simulationToolBar.cpp \
    simulationManager.cpp \
    abstractSimulator.cpp \
    frameLogDock.cpp
HEADERS += simulation.h \
    simulationGlobal.h \
    simulationToolBar.h \
    simulationManager.h \
    abstractSimulator.h \
    frameLogDock.h
INCLUDEPATH += ../../SDK/ \
    ../../../jLib/include \
    ../../../EksCore \
    ../../pluginSource/time \
    ../../pluginSource/basicViewport
LIBS += -L../../../bin/ \
    -L../../plugins/ \
    -lalter \
    -ljLib \
    -ltime \
    -lEksCore \
    -lbasicViewport
DESTDIR += ../../plugins
RESOURCES += resources.qrc
