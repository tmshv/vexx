#ifndef XINTERFACEOBJECT_H
#define XINTERFACEOBJECT_H

class XInterfaceObject
  {
public:
  XInterfaceObject()
    {
    }

  template <typename T>
  static XInterfaceObject newInstance(XInterface<T>*, T** m)
    {
    typedef cvv8::ClassCreator<T> CC;
    XInterfaceObject ret;
    ret._object = CC::Instance().NewInstance( 0, NULL, *m );
    return ret;
    }

private:
  v8::Handle<v8::Object> _object;
  friend class XContext;
  };

#endif // XINTERFACEOBJECT_H
