# -------------------------------------------------
# Project created by QtCreator 2010-04-23T12:10:30
# -------------------------------------------------

TARGET = Tang
TEMPLATE = app

include("../Eks/EksCore/GeneralOptions.pri")

QT += opengl \
    xml \
    network \
    webkit

SOURCES += main.cpp \
    viewport.cpp \
    webview.cpp \
    object.cpp \
    componentdocument.cpp \
    component.cpp \
    area.cpp \
    areadocument.cpp \
    tangmanager.cpp

LIBS += -lEksCore \
    -lEksScript \
    -lEks3D \
    -lshift \
    -lalter \
    -lscript \
    -lShiftAlter \
    -lShiftGraphicsCore \
    -lShiftMathsCore \
    -lShiftMeshCore \
    -lUI

INCLUDEPATH += ../include \
    $$ROOT/Eks/EksCore \
    $$ROOT/Eks/EksScript \
    $$ROOT/Eks/Eks3D/include \
    $$ROOT/Shift \
    $$ROOT/Shift/GraphicsCore \
    $$ROOT/Shift/MeshCore \
    $$ROOT/Alter2 \
    $$ROOT/EksCore \
    $$ROOT/Alter2/Plugins/UI \
    $$ROOT/Alter2/Plugins/Script \
    $$ROOT/Alter2/Plugins/ShiftAlter

HEADERS += \
    viewport.h \
    webview.h \
    object.h \
    componentdocument.h \
    component.h \
    area.h \
    areadocument.h \
    tangmanager.h

OTHER_FILES += \
    startupTang.js \
    Components/SteelPlate.json.js \
    Components/SteelPlate.json \
    ViewportContextMenu.js







