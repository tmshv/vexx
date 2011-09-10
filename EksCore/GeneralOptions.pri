include($$PWD"/SIMD.pri")
include($$PWD"/C++0x.pri")

win32-g++ {
  QMAKE_CXXFLAGS += -Wall
}

win32-msvc2008 {
  QMAKE_CXXFLAGS += /MDd
}

ROOT = $$PWD"/../"
BINFOLDER = $$ROOT"/bin/"

export (ROOT)

DEFINES += QT_NO_STL

DESTDIR = $$BINFOLDER

LIBS += -L$$BINFOLDER

DEFINES += $$upper($$TARGET)_BUILD
