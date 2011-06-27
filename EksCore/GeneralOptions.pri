include($$PWD"/SIMD.pri")
include($$PWD"/C++0x.pri")

QMAKE_CXXFLAGS += -Wall

ROOT = $$PWD"/../"
BINFOLDER = $$ROOT"/bin/"

export (ROOT)

DESTDIR = $$BINFOLDER

LIBS += -L$$BINFOLDER

DEFINES += $$upper($$TARGET)_BUILD
