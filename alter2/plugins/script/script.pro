# -------------------------------------------------
# Project created by QtCreator 2010-06-09T14:26:27
# -------------------------------------------------

TARGET = script
TEMPLATE = lib

include("../../../Eks/EksCore/GeneralOptions.pri")

QT += declarative opengl
INCLUDEPATH += ../../ \
    $$ROOT/Eks/EksCore \
    $$ROOT/Eks/EksScript \
    $$ROOT/shift \
    $$ROOT/alter2/plugins/UI/ \
    $$ROOT/alter2/plugins/ShiftAlter/

LIBS += -lalter \
    -lEksCore \
    -lEksScript \
    -lUI \
    -lshift \
    -lShiftAlter

SOURCES += scplugin.cpp \
    scsurface.cpp \
    scembeddedtypes.cpp \
    QMLExtensions/scpath.cpp \
    QMLExtensions/scellipse.cpp \
    QMLExtensions/scbrush.cpp \
    QMLExtensions/scnodedisplay.cpp \
    QMLExtensions/scnodeitem.cpp \
    QMLExtensions/scpropertyitem.cpp \
    QMLExtensions/scpropertydisplay.cpp \
    QMLExtensions/scconnectoritem.cpp \
    QMLExtensions/scmousearea.cpp \
    scdeclarativesurface.cpp \
    scdbutils.cpp

HEADERS += scplugin.h \
    scglobal.h \
    scsurface.h \
    scembeddedtypes.h \
    QMLExtensions/scpath.h \
    QMLExtensions/scellipse.h \
    QMLExtensions/scbrush.h \
    QMLExtensions/scnodedisplay.h \
    QMLExtensions/scnodeitem.h \
    QMLExtensions/scpropertyitem.h \
    QMLExtensions/scpropertydisplay.h \
    QMLExtensions/scconnectoritem.h \
    QMLExtensions/scmousearea.h \
    scdeclarativesurface.h \
    scdbutils.h

RESOURCES += \
    ScResources.qrc

OTHER_FILES += \
    JSPartExporter.js \
    CoreStartup.js \
    StartupUI.js \
    NodeCanvas.qml \
    NodeCanvasContents/Node.qml \
    NodeCanvasContents/Property.qml \
    NodeCanvasContents/Input.qml \
    NodeCanvasContents/PropertyList.qml \
    NodeCanvasContents/PropertyInterface.qml \
    NodeCanvasContents/NodeHeader.qml \
    NodeCanvasContents/DynamicInput.qml \
    NodeCanvasContents/PathHolder.qml \
    NodeCanvasContents/PathItem.qml \
    NodeCanvasContextMenu.qml \
    CreateMenuContents/Menu.qml \
    NodeCanvasContents/Properties.qml \
    jsEntities/StandardSurface.js \
    jsEntities/FlatSurface.js






