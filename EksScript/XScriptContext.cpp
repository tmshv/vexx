#include "XScriptContext.h"
#include "XScriptValueV8Internals.h"

class XScriptContext::Impl
  {
public:
  Impl(v8::Persistent<v8::Context> c) : _context(c), _scope(_context)
    {
    }

  v8::Persistent<v8::Context> _context;
  v8::Context::Scope _scope;
  };

XScriptContext::XScriptContext(XScriptEngine *e)
    : _engine(e)
  {
  _impl = new Impl(v8::Context::New(NULL, getGlobalTemplate(_engine)));
  _impl->_context->AllowCodeGenerationFromStrings(false);
  }

XScriptContext::~XScriptContext()
  {
  // Dispose the persistent context.
  _impl->_context.Dispose();
  delete _impl;
  }

void XScriptContext::set(const QString& in, const XScriptObject& obj)
  {
  XScriptValue propName = XScriptConvert::to(in);
  _impl->_context->Global()->Set(getV8Internal(propName), getV8Internal(obj));
  }

void XScriptContext::set(const QString &name, Function fn)
  {
  XScriptValue propName = XScriptConvert::to(name);
  v8::Handle<v8::FunctionTemplate> fnTmpl = ::v8::FunctionTemplate::New((v8::InvocationCallback)fn);

  _impl->_context->Global()->Set(getV8Internal(propName), fnTmpl->GetFunction());
  }


void XScriptContext::addInterface(const XInterfaceBase *i)
  {
  xAssert(i->isSealed());
  i->addClassTo(i->typeName(), fromObjectHandle(_impl->_context->Global()));
  }
