#ifndef XSCRIPTVALUE_H
#define XSCRIPTVALUE_H

#include "XScriptGlobal.h"

class XScriptObject;
class QVariant;

class EKSSCRIPT_EXPORT XScriptValue
  {
public:
  XScriptValue();
  XScriptValue(bool x);
  XScriptValue(xuint32 x);
  XScriptValue(xint32 x);
  XScriptValue(xuint64 x);
  XScriptValue(xint64 x);
  XScriptValue(double x);
  XScriptValue(float x);
  XScriptValue(const QString &str);
  XScriptValue(const XScriptObject &str);
  XScriptValue(const QVariant& val);
  XScriptValue(void* val);
  ~XScriptValue();

  XScriptValue(const XScriptValue&);
  XScriptValue& operator=(const XScriptValue&);

  void clear();

  bool isValid() const;
  bool isObject() const;
  bool isArray() const;

  xsize length() const;
  XScriptValue at(xsize id);
  void set(xsize id, const XScriptValue &);

  void *toExternal() const;
  double toNumber() const;
  xint64 toInteger() const;
  bool toBoolean() const;
  QString toString() const;
  QVariant toVariant(int typeHint=0) const;

  static XScriptValue newArray();

private:
  void *_object;
  };

class EKSSCRIPT_EXPORT XPersistentScriptValue : public XScriptValue
  {
public:
  XPersistentScriptValue() : XScriptValue()
    {
    }

  XPersistentScriptValue(bool x) : XScriptValue(x)
    {
    }

  XPersistentScriptValue(xuint32 x) : XScriptValue(x)
    {
    }

  XPersistentScriptValue(xint32 x) : XScriptValue(x)
    {
    }

  XPersistentScriptValue(xuint64 x) : XScriptValue(x)
    {
    }

  XPersistentScriptValue(xint64 x) : XScriptValue(x)
    {
    }

  XPersistentScriptValue(double x) : XScriptValue(x)
    {
    }

  XPersistentScriptValue(float x) : XScriptValue(x)
    {
    }

  XPersistentScriptValue(const QString &str) : XScriptValue(str)
    {
    }

  XPersistentScriptValue(const XScriptObject &str) : XScriptValue(str)
    {
    }

  void dispose();
  };

#endif // XSCRIPTVALUE_H
