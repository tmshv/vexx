win32 {
  debug:LIBS += -L$$ROOT/EksScript/v8/debuglib/
  release:LIBS += -L$$ROOT/EksScript/v8/releaselib/

  LIBS += -lpreparser_lib -lv8_base -lv8_nosnapshot -lv8_snapshot -lws2_32 -lwinmm
}
