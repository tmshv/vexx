#ifndef XSCRIPTFUNCTION_H
#define XSCRIPTFUNCTION_H

#include "XScriptGlobal.h"

class XScriptObject;
class XScriptValue;
class XScriptArguments;

class EKSSCRIPT_EXPORT XScriptFunction
  {
public:
  XScriptFunction();
  ~XScriptFunction();

  XScriptFunction(const XScriptFunction&);
  XScriptFunction& operator=(const XScriptFunction&);

  XScriptValue callWithTryCatch(const XScriptObject &thisObject, int argc, const XScriptValue args[], bool *error) const;
  XScriptValue call(const XScriptObject &thisObject, int argc, const XScriptValue args[]) const;
  XScriptValue callAsConstructor(const XScriptArguments&);

private:
  void *_func;
  };

class EKSSCRIPT_EXPORT XAccessorInfo
  {
public:
  XScriptObject calleeThis() const;

private:
  XAccessorInfo();
  ~XAccessorInfo();
  XAccessorInfo(const XAccessorInfo&);
  XAccessorInfo& operator=(const XAccessorInfo&);

  void* _info;
  };

class EKSSCRIPT_EXPORT XScriptArguments
  {
public:
  bool isConstructCall() const;
  XScriptFunction callee() const;
  XScriptObject calleeThis() const;
  xsize length() const;
  XScriptValue at(xsize i) const;

private:
  XScriptArguments();
  ~XScriptArguments();
  XScriptArguments(const XScriptArguments&);
  XScriptArguments& operator=(const XScriptArguments&);

  void* _args;
  void* _values;
  int _length;
  bool _isConstructCall;
  };

#endif // XSCRIPTFUNCTION_H
