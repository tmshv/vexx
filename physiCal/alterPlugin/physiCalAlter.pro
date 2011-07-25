# -------------------------------------------------
# Project created by QtCreator 2009-12-28T12:23:51
# -------------------------------------------------
QT += opengl
TARGET = physiCalAlter
TEMPLATE = lib
DEFINES += PHYSICALALTER_LIBRARY
SOURCES += physicalalter.cpp \
    physicalAlterSimulation.cpp \
    objectEntity.cpp \
    materialEntity.cpp \
    groundObject.cpp \
    freezeBox.cpp
HEADERS += physicalalter.h \
    physicalAlterGlobal.h \
    physicalAlterSimulation.h \
    objectEntity.h \
    materialEntity.h \
    groundObject.h \
    freezeBox.h
INCLUDEPATH += ../../alter/SDK/ \
    ../../jLib/include \
    ../../alter/pluginSource/simulation/ \
    ../../alter/pluginSource/basicViewport/ \
    ../include \
    ../../EksCore
LIBS += -L../../bin/ \
    -L../../alter/plugins/ \
    -lalter \
    -ljLib \
    -lsimulation \
    -lphysiCal \
    -lbasicViewport \
    -lEksCore
DESTDIR += ../../alter/plugins
RESOURCES += physicalAlterResources.qrc
OTHER_FILES += simulationVert.txt \
    simulationFrag.txt
