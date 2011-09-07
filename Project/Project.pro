TARGET = Project
TEMPLATE = app

include("../EksCore/GeneralOptions.pri")

QT += opengl \
    xml \
    network \
    script \
    webkit

SOURCES += main.cpp

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
    startup.js \
    Project.js


