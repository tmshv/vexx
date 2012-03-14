#ifndef XSCRIPTOBJECT_H
#define XSCRIPTOBJECT_H

#include "XScriptGlobal.h"

class XInterfaceObject;

class EKSSCRIPT_EXPORT XScriptObject
  {
public:
  XScriptObject();
  XScriptObject(bool x);
  XScriptObject(xuint32 x);
  XScriptObject(xint32 x);
  XScriptObject(xuint64 x);
  XScriptObject(xint64 x);
  XScriptObject(double x);
  XScriptObject(float x);
  XScriptObject(const QString &str);
  XScriptObject(const XInterfaceObject &str);
  ~XScriptObject();

  XScriptObject(const XScriptObject&);
  XScriptObject& operator=(const XScriptObject&);

  void clear();

  bool isValid() const;
  bool isObject() const;

  void *toExternal() const;
  double toNumber() const;
  xint64 toInteger() const;
  bool toBoolean() const;
  QString toString() const;

private:
  void *_object;
  };

class EKSSCRIPT_EXPORT XPersistentScriptObject : public XScriptObject
  {
public:
  XPersistentScriptObject() : XScriptObject()
    {
    }

  XPersistentScriptObject(bool x) : XScriptObject(x)
    {
    }

  XPersistentScriptObject(xuint32 x) : XScriptObject(x)
    {
    }

  XPersistentScriptObject(xint32 x) : XScriptObject(x)
    {
    }

  XPersistentScriptObject(xuint64 x) : XScriptObject(x)
    {
    }

  XPersistentScriptObject(xint64 x) : XScriptObject(x)
    {
    }

  XPersistentScriptObject(double x) : XScriptObject(x)
    {
    }

  XPersistentScriptObject(float x) : XScriptObject(x)
    {
    }

  XPersistentScriptObject(const QString &str) : XScriptObject(str)
    {
    }

  XPersistentScriptObject(const XInterfaceObject &str) : XScriptObject(str)
    {
    }

  void dispose();
  };

#endif // XSCRIPTOBJECT_H
