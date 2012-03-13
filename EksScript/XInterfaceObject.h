#ifndef XINTERFACEOBJECT_H
#define XINTERFACEOBJECT_H

#include "XScriptGlobal.h"
#include "XInterface.h"

class EKSSCRIPT_EXPORT XScriptObject
  {
  XScriptObject();
  XScriptObject(bool x);
  XScriptObject(xuint32 x);
  XScriptObject(xint32 x);
  XScriptObject(xuint64 x);
  XScriptObject(xint64 x);
  XScriptObject(double x);
  XScriptObject(float x);

private:
  v8::Handle<v8::Value> _object;
  };

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
