#-------------------------------------------------
#
# Project created by QtCreator 2010-10-30T13:02:35
#
#-------------------------------------------------

TARGET = IneAlter
TEMPLATE = lib
DESTDIR = ../../../bin

DEFINES += INEALTER_LIBRARY

SOURCES += \
    InePlugin.cpp \
    IneProperties.cpp \
    IneFixtureManager.cpp \
    IneRoot.cpp \
    IneFixture.cpp \
    IneFixtureEditDialog.cpp

HEADERS += \
    InePlugin.h \
    IneProperties.h \
    IneFixtureManager.h \
    IneRoot.h \
    IneFixture.h \
    IneFixtureEditDialog.h

INCLUDEPATH += ../../ ../../../Ine/ ../../../EksCore ../UI ../../../shift ../ShiftAlter

LIBS += -L../../../bin -lalter -lIne -lEksCore -lEnttecUSBProDriver -lUI -lshift -lShiftAlter

FORMS +=
