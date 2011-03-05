# -------------------------------------------------
# Project created by QtCreator 2009-11-18T16:34:55
# -------------------------------------------------
QT += opengl \
    xml
TARGET = calc
TEMPLATE = lib
DEFINES += CALCV2_LIBRARY \
    SYNAPSE_RENDER_DEBUG \
    SYNAPSE_GLSL_INLINE

# DEFINES += SYNAPSE_GL_VIEWPORT
SOURCES += nodeEntity.cpp \
    main.cpp \
    viewerDock.cpp \
    viewer.cpp \
    nodeDB.cpp \
    nodeToolBar.cpp \
    nodeRenderer.cpp \
    exportWidget.cpp \
    timelineWidget.cpp \
    colourSampleWidget.cpp \
    helpSystem.cpp \
    scriptEdit.cpp \
    nodeEditDialog.cpp \
    nodeEntityManipulator.cpp \
    groupEditDialog.cpp \
    renderDebugger.cpp \
    colourPickerWidget.cpp \
    PreferencesDialog.cpp \
    synapseSDK.cpp
HEADERS += nodeEntity.h \
    viewerDock.h \
    viewer.h \
    nodeDB.h \
    nodeToolBar.h \
    nodeRenderer.h \
    exportWidget.h \
    timelineWidget.h \
    colourSampleWidget.h \
    utils.h \
    helpSystem.h \
    scriptEdit.h \
    nodeEditDialog.h \
    nodeEntityManipulator.h \
    groupEditDialog.h \
    renderDebugger.h \
    colourPickerWidget.h \
    PreferencesDialog.h \
    synapseSDK.h
INCLUDEPATH += ../alter/SDK
INCLUDEPATH += ../jLib/include \
    ./pie_menu/
LIBS += -L../bin/ \
    -lalter \
    -ljLib \
    -lEksCore \
    -lEksGui
DESTDIR = ../bin/plugins
INCLUDEPATH += ./node_ui/include \
    ./node_ui/nodes/ \
    ../jLib/external/include \
    ./node_ui/ \
    ../alter/pluginSource/basicViewport \
    ../EksCore/ \
    ../EksGui/ \
    ./
include( ./node_ui/nodeUI.pri )
include( ./nodes.pri )
include( ./pie_menu/qtpiemenu.pri )
QMAKE_CXXFLAGS += -msse
OTHER_FILES += nodes/basic.xml \
    nodes/mask.xml
FORMS += exportWidget.ui \
    nodeEditDialog.ui \
    groupEditDialog.ui \
    PreferencesDialog.ui
RESOURCES += calcIcons.qrc
