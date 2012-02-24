#ifndef XCONTEXT_H
#define XCONTEXT_H

#include "XScriptGlobal.h"
#include "XEngine.h"
#include "XInterfaceObject.h"

class EKSSCRIPT_EXPORT XContext
  {
public:
  XContext(XEngine *e);
  ~XContext();

  void set(const char* in, const XInterfaceObject& obj);

  template <typename T>
  void addInterface(const XInterface<T>&)
    {
    typedef cvv8::ClassCreator<T> CC;
    CC& cc(CC::Instance());

    cc.AddClassTo( cvv8::TypeName<T>::Value, _context->Global() );
    }

private:
  XEngine* _engine;
  v8::Persistent<v8::Context> _context;
  v8::Context::Scope _scope;
  };

#endif // XCONTEXT_H
