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

  v8::HandleScope scope;
  v8::Handle<v8::ObjectTemplate> globalTemplate;
  };

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
