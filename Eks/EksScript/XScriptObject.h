#ifndef XScriptObject_H
#define XScriptObject_H

#include "XScriptGlobal.h"

class XInterfaceBase;
template <typename T> class XInterface;
class XScriptFunction;
class XScriptValue;
class XPersistentScriptValue;

class EKSSCRIPT_EXPORT XScriptObject
  {
public:
  XScriptObject();
  XScriptObject(const XScriptValue &func);
  XScriptObject(const XScriptFunction &func);
  ~XScriptObject();

  XScriptObject(const XScriptObject&);
  XScriptObject& operator=(const XScriptObject&);

  xsize internalFieldCount() const;
  void *internalField(xsize idx) const;

  XInterfaceBase *getInterface() const;
  XScriptValue getPrototype() const;
  void setPrototype(XScriptValue proto);

  XScriptValue get(const QString &) const;
  void set(const QString &, const XScriptValue &);

  bool isValid() const;

  template <typename T>
  static XScriptObject newInstance(XInterface<T>* i);

  template <typename T> T *castTo();
  template <typename T> const T *castTo() const;

  static XScriptObject newObject();

private:
  void *_object;
  friend class XScriptContext;
  };

#include "XInterface.h"

template <typename T> XScriptObject XScriptObject::newInstance(XInterface<T>* i)
  {
  return i->newInstance(0, NULL);
  }

template <typename T> T *XScriptObject::castTo()
  {
  return XScriptConvert::from<T>(*this);
  }

template <typename T> const T *XScriptObject::castTo() const
  {
  return XScriptConvert::from<T>(*this);
  }

Q_DECLARE_METATYPE(XScriptObject);
Q_DECLARE_METATYPE(XScriptObject*);

#endif // XScriptObject_H
