win32-g++ {
  QMAKE_CXXFLAGS += -std=gnu++0x
  DEFINES += X_CPPOX_SUPPORT
}

win32-msvc2010 {
  DEFINES += X_CPPOX_SUPPORT X_HAS_LONG_LONG
}

HEADERS +=

SOURCES +=








