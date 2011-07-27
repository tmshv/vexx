TARGET = alter
TEMPLATE = app
SOURCES += app.cpp
LIBS += -L../../../bin \
    -lalter \
    -lEksCore \
    -lEksGui
INCLUDEPATH += ../../../alter/SDK \
               ../../../EksCore
DESTDIR = ../../../bin
QMAKE_CXXFLAGS += -msse
RESOURCES += ../../resources/branding.qrc
