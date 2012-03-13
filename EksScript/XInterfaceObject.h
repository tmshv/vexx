#ifndef XINTERFACEOBJECT_H
#define XINTERFACEOBJECT_H

#include "XScriptGlobal.h"
#include "XInterface.h"

template <typename T> class XInterface;

class EKSSCRIPT_EXPORT XInterfaceObject
  {
public:
  XInterfaceObject();

  template <typename T>
  static XInterfaceObject newInstance(XInterface<T>* i)
    {
    XInterfaceObject ret;
    ret._object = i->newInstance(0, NULL);
    return ret;
    }

  template <typename T> T *castTo()
    {
    return XScriptConvert::from<T>(_object);
    }

  template <typename T> const T *castTo() const
    {
    return XScriptConvert::from<T>(_object);
    }

private:
  v8::Handle<v8::Object> _object;
  friend class XContext;
  };

#endif // XINTERFACEOBJECT_H
