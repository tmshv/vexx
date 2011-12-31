# -------------------------------------------------
# Project created by QtCreator 2010-06-09T14:26:27
# -------------------------------------------------

TARGET = script
TEMPLATE = lib

include("../../../EksCore/GeneralOptions.pri")

QT += script scripttools declarative
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
    scio.cpp

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
    scio.h

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
    NodeCanvasContents/PropertyList.qml






