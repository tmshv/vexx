QT += opengl
SOURCES += main.cpp
HEADERS += main.h
LIBS += -L../../bin \
    -ljLib \
    -lphysiCal
INCLUDEPATH += ../../jLib/include/ \
    ../include
QMAKE_CXXFLAGS += -msse
OTHER_FILES += postFrag3.txt \
    postFrag2.txt \
    postFrag1.txt
