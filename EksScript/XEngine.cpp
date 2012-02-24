#include "XEngine.h"

XEngine::XEngine() : _globalTemplate(v8::ObjectTemplate::New())
  {
  }

XEngine::~XEngine()
  {
  v8::V8::LowMemoryNotification();
  }
