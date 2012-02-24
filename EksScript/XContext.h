#ifndef XCONTEXT_H
#define XCONTEXT_H

class XContext
  {
public:
  XContext(XEngine *e)
      : _engine(e),
      _context(v8::Context::New(NULL, _engine->_globalTemplate)),
      _scope(_context)
    {
    }

  ~XContext()
    {
    // Dispose the persistent context.
    _context.Dispose();
    }

  template <typename T>
  void addInterface(const XInterface<T>&)
    {
    typedef cvv8::ClassCreator<T> CC;
    CC& cc(CC::Instance());

    cc.AddClassTo( cvv8::TypeName<T>::Value, _context->Global() );
    }

  void set(const char* in, const XInterfaceObject& obj)
    {
      v8::Handle<v8::String> propName = v8::String::New(in);
    _context->Global()->Set(propName, obj._object);
    }

private:
  XEngine* _engine;
  v8::Persistent<v8::Context> _context;
  v8::Context::Scope _scope;
  };

#endif // XCONTEXT_H
