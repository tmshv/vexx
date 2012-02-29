#include "XContext.h"

XContext::XContext(XEngine *e)
    : _engine(e),
    _context(v8::Context::New(NULL, _engine->_globalTemplate)),
    _scope(_context)
  {
  _context->AllowCodeGenerationFromStrings(false);
  }

XContext::~XContext()
  {
  // Dispose the persistent context.
  _context.Dispose();
  }

void XContext::set(const char* in, const XInterfaceObject& obj)
  {
    v8::Handle<v8::String> propName = v8::String::New(in);
  _context->Global()->Set(propName, obj._object);
  }

void XContext::addInterface(const XInterfaceBase *i)
  {
  i->addClassTo(i->typeName(), _context->Global());
  }
