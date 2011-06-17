include($$PWD"/SIMD.pri")

QMAKE_CXXFLAGS += -Wall

ROOT = $$PWD"/../"
BINFOLDER = $$ROOT"/bin/"

export (ROOT)

DESTDIR = $$BINFOLDER

LIBS += -L$$BINFOLDER

DEFINES += $$upper($$TARGET)_BUILD
