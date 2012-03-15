#include "XEngine.h"
#include "v8.h"

class XEngine::Impl
  {
public:
  Impl() : globalTemplate(v8::ObjectTemplate::New())
    {
    }

  static XEngine::Impl *val(XEngine *e)
    {
    return e->_impl;
    }

  v8::HandleScope scope;
  v8::Handle<v8::ObjectTemplate> globalTemplate;
  };

XEngine::XEngine() : _impl(new XEngine::Impl)
  {
  }

XEngine::~XEngine()
  {
  v8::V8::LowMemoryNotification();
  }

void XEngine::adjustAmountOfExternalAllocatedMemory(int in)
  {
  v8::V8::AdjustAmountOfExternalAllocatedMemory(in);
  }

v8::Handle<v8::ObjectTemplate> getGlobalTemplate(XEngine *e)
  {
  XEngine::Impl* impl = XEngine::Impl::val(e);
  return impl->globalTemplate;
  }
