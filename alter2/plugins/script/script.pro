# -------------------------------------------------
# Project created by QtCreator 2010-06-09T14:26:27
# -------------------------------------------------

TARGET = script
TEMPLATE = lib

include("../../../EksCore/GeneralOptions.pri")

QT += script scripttools declarative opengl
INCLUDEPATH += ../../ \
    $$ROOT/EksCore \
    $$ROOT/shift \
    $$ROOT/alter2/plugins/UI/ \
    $$ROOT/alter2/plugins/ShiftAlter/

LIBS += -lalter \
    -lEksCore \
    -lUI \
    -lshift \
    -lShiftAlter

SOURCES += scplugin.cpp \
    scsurface.cpp \
    scshiftproperty.cpp \
    scembeddedtypes.cpp \
    scshiftpropertycontainer.cpp \
    scshiftentity.cpp \
    scshiftdatabase.cpp \
    scshiftfloatarrayproperty.cpp \
    scio.cpp \
    QMLExtensions\scpath.cpp \
    QMLExtensions/scellipse.cpp \
    QMLExtensions/scbrush.cpp \
    QMLExtensions/scnodedisplay.cpp \
    QMLExtensions/scnodeitem.cpp \
    QMLExtensions/scpropertyitem.cpp \
    QMLExtensions/scpropertydisplay.cpp \
    QMLExtensions/scconnectoritem.cpp

HEADERS += scplugin.h \
    scglobal.h \
    scsurface.h \
    scshiftproperty.h \
    scembeddedtypes.h \
    scwrappedclass.h \
    scshiftpropertycontainer.h \
    scshiftentity.h \
    scshiftdatabase.h \
    scshiftfloatarrayproperty.h \
    scio.h \
    QMLExtensions\scpath.h \
    QMLExtensions/scellipse.h \
    QMLExtensions/scbrush.h \
    QMLExtensions/scnodedisplay.h \
    QMLExtensions/scnodeitem.h \
    QMLExtensions/scpropertyitem.h \
    QMLExtensions/scpropertydisplay.h \
    QMLExtensions/scconnectoritem.h

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
    QMLItemLookup.js \
    NodeCanvasContents/PropertyList.qml \
    NodeCanvasContents/PropertyInterface.qml \
    NodeCanvasContents/NodeHeader.qml \
    NodeCanvasContents/DynamicInput.qml






