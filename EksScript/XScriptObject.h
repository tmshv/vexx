#ifndef XSCRIPTOBJECT_H
#define XSCRIPTOBJECT_H

#include "XScriptGlobal.h"

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

  void *toExternal() const;
  double toNumber() const;
  xint64 toInteger() const;
  bool toBoolean() const;
  QString toString() const;

private:
  void *_object;
  };

#endif // XSCRIPTOBJECT_H
