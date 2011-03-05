#-------------------------------------------------
#
# Project created by QtCreator 2010-10-30T11:52:10
#
#-------------------------------------------------
TARGET = EnttecUSBProDriver
TEMPLATE = lib

DEFINES += ENTTEXUSBPRO_BUILD

SOURCES += EnttecUSBProDriver.cpp \
    EnttecUSBProInterface.cpp \
    pro_driver.c

HEADERS += EnttecUSBProDriver.h\
    EnttecUSBProInterface.h \
    EnttecUSBProGlobal.h \
    pro_driver.h \
    FTD2XX.H

INCLUDEPATH += ../ ../../EksCore

LIBS += -L../../bin -lEksCore -lIne ftd2xx.lib

DESTDIR = ../../bin
