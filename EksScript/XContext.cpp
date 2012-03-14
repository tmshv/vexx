#include "XContext.h"
#include "XScriptObjectV8Internals.h"

class XContext::Impl
  {
public:
  Impl(v8::Persistent<v8::Context> c) : _context(c), _scope(_context)
    {
    }

  v8::Persistent<v8::Context> _context;
  v8::Context::Scope _scope;
  };

XContext::XContext(XEngine *e)
    : _engine(e)
  {
  _impl = new Impl(v8::Context::New(NULL, getGlobalTemplate(_engine)));
  _impl->_context->AllowCodeGenerationFromStrings(false);
  }

XContext::~XContext()
  {
  // Dispose the persistent context.
  _impl->_context.Dispose();
  delete _impl;
  }

void XContext::set(const char* in, const XInterfaceObject& obj)
  {
    v8::Handle<v8::String> propName = v8::String::New(in);
  _impl->_context->Global()->Set(propName, getV8Internal(obj));
  }

void XContext::addInterface(const XInterfaceBase *i)
  {
  i->addClassTo(i->typeName(), fromObjectHandle(_impl->_context->Global()));
  }
