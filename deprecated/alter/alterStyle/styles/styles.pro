HEADERS = norwegianwoodstyle.h \
    widgetgallery.h \
    ADarkStyle.h
SOURCES = main.cpp \
    norwegianwoodstyle.cpp \
    widgetgallery.cpp \
    ADarkStyle.cpp
RESOURCES = styles.qrc \
    resources.qrc
INCLUDEPATH += ../../../EksCore
LIBS += -L../../../bin \
    -lEksCore
