# -------------------------------------------------
# Project created by QtCreator 2010-04-23T12:10:30
# -------------------------------------------------

TARGET = QmlNodes
TEMPLATE = app

include("../../EksCore/GeneralOptions.pri")

QT += opengl \
    xml \
    network \
    webkit

SOURCES += main.cpp \
    qmlapplicationviewer/qmlapplicationviewer.cpp

LIBS += -lEksCore \
    -lEks3D \
    -lshift \
    -lalter \
    -lscript \
    -lShiftAlter \
    -lShiftGraphicsCore \
    -lUI

INCLUDEPATH += ../include \
    $$ROOT/EksCore \
    $$ROOT/Eks3D/include \
    $$ROOT/Shift \
    $$ROOT/Shift/GraphicsCore \
    $$ROOT/Alter2 \
    $$ROOT/EksCore \
    $$ROOT/Alter2/Plugins/UI \
    $$ROOT/Alter2/Plugins/Script \
    $$ROOT/Alter2/Plugins/ShiftAlter

OTHER_FILES += \
    qml/QmlNodes/main.qml \
    qml/QmlNodes/content/Node.qml \
    qmlapplicationviewer/qmlapplicationviewer.pri

HEADERS += \
    qmlapplicationviewer/qmlapplicationviewer.h





