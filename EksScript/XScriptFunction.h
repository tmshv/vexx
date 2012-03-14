#ifndef XSCRIPTFUNCTION_H
#define XSCRIPTFUNCTION_H

#include "XScriptGlobal.h"

class XInterfaceObject;
class XScriptObject;
class XScriptArguments;

class EKSSCRIPT_EXPORT XScriptFunction
  {
public:
  XScriptFunction();
  ~XScriptFunction();

  XScriptFunction(const XScriptFunction&);
  XScriptFunction& operator=(const XScriptFunction&);

  XScriptObject call(const XInterfaceObject &thisObject, int argc, XScriptObject args[]) const;
  XScriptObject callAsConstructor(const XScriptArguments&);

private:
  void *_func;
  };

class EKSSCRIPT_EXPORT XAccessorInfo
  {
public:
  XInterfaceObject calleeThis() const;

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
  XInterfaceObject calleeThis() const;
  xsize length() const;
  XScriptObject at(xsize i) const;

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
