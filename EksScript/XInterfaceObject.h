#ifndef XINTERFACEOBJECT_H
#define XINTERFACEOBJECT_H

#include "XScriptGlobal.h"

template <typename T> class XInterface;
class XScriptFunction;
class XScriptObject;
class XPersistentScriptObject;

class EKSSCRIPT_EXPORT XInterfaceObject
  {
public:
  XInterfaceObject();
  XInterfaceObject(const XScriptObject &func);
  XInterfaceObject(const XScriptFunction &func);
  ~XInterfaceObject();

  XInterfaceObject(const XInterfaceObject&);
  XInterfaceObject& operator=(const XInterfaceObject&);

  xsize internalFieldCount() const;
  void *internalField(xsize idx) const;

  XScriptObject getPrototype() const;

  bool isValid() const;

  typedef void (*WeakDtor)(XPersistentScriptObject object,void* p);
  void makeWeak(void *data, WeakDtor cb);

  template <typename T>
  static XInterfaceObject newInstance(XInterface<T>* i);

  template <typename T> T *castTo();
  template <typename T> const T *castTo() const;
private:
  void *_object;
  friend class XContext;
  };

#include "XInterface.h"

template <typename T> XInterfaceObject XInterfaceObject::newInstance(XInterface<T>* i)
  {
  return i->newInstance(0, NULL);
  }

template <typename T> T *XInterfaceObject::castTo()
  {
  return XScriptConvert::from<T>(*this);
  }

template <typename T> const T *XInterfaceObject::castTo() const
  {
  return XScriptConvert::from<T>(*this);
  }

#endif // XINTERFACEOBJECT_H
