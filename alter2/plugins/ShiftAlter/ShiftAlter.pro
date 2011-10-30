#-------------------------------------------------
#
# Project created by QtCreator 2010-10-31T08:46:00
#
#-------------------------------------------------

TARGET = ShiftAlter
TEMPLATE = lib

include("../../../EksCore/GeneralOptions.pri")


SOURCES += splugin.cpp \
    sappdatabase.cpp \
    saparteditor.cpp \
    saparteditorinterface.cpp

HEADERS += splugin.h \
    saglobal.h \
    sappdatabase.h \
    saparteditor.h \
    saparteditorinterface.h

INCLUDEPATH += $$ROOT/EksCore $$ROOT/Shift ../../


LIBS += -lshift -lEksCore -lalter

FORMS +=

RESOURCES += \
    Resources.qrc












