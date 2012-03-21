#include "XScriptEngine.h"
#include "v8.h"

class XScriptEngine::Impl
  {
public:
  Impl() : globalTemplate(v8::ObjectTemplate::New())
    {
    }

  static XScriptEngine::Impl *val(XScriptEngine *e)
    {
    return e->_impl;
    }

  v8::Handle<v8::ObjectTemplate> globalTemplate;
  };

struct StaticEngine
  {
  v8::HandleScope scope;
  };
StaticEngine *g_engine = 0;


void fatal(const char* location, const char* message)
  {
  qFatal("%s: %s", location, message);
  }

void XScriptEngine::initiate()
  {
  if(g_engine)
    {
    return;
    }

  v8::V8::SetFatalErrorHandler(fatal);

  g_engine = new StaticEngine();
  }

void XScriptEngine::terminate()
  {
  delete g_engine;
  }

XScriptEngine::XScriptEngine() : _impl(new XScriptEngine::Impl)
  {
  }

XScriptEngine::~XScriptEngine()
  {
  v8::V8::LowMemoryNotification();
  }

void XScriptEngine::adjustAmountOfExternalAllocatedMemory(int in)
  {
  v8::V8::AdjustAmountOfExternalAllocatedMemory(in);
  }

v8::Handle<v8::ObjectTemplate> getGlobalTemplate(XScriptEngine *e)
  {
  XScriptEngine::Impl* impl = XScriptEngine::Impl::val(e);
  return impl->globalTemplate;
  }
