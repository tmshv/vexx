include($$PWD"/SIMD.pri")
include($$PWD"/C++0x.pri")

QMAKE_CXXFLAGS += -Wall

ROOT = $$PWD"/../"
BINFOLDER = $$ROOT"/bin/"

export (ROOT)

DEFINES += QT_NO_STL

DESTDIR = $$BINFOLDER

LIBS += -L$$BINFOLDER

DEFINES += $$upper($$TARGET)_BUILD
