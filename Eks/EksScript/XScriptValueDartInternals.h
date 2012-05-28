#ifndef XSCRIPTVALUEDARTINTERNALS_H
#define XSCRIPTVALUEDARTINTERNALS_H

#include "XScriptGlobal.h"

#include "dart/dart_api.h"
#include "QDebug"

class XScriptValue;
class XScriptObject;
class XInterfaceBase;
class XScriptFunction;
class XScriptEngine;

inline void printError(Dart_Handle han)
  {
  const char *err = Dart_GetError(han);
  if(err)
    {
    qWarning() << err;
    }
  }

XScriptValue fromHandle(Dart_Handle);
XScriptObject fromObjectHandle(Dart_Handle);
XScriptFunction fromFunction(Dart_Handle);

#define CHECK_HANDLE(handle) if(Dart_IsError(handle)) { printError(handle); }

Dart_Handle getDartInternal(const XScriptValue&);
Dart_Handle getDartInternal(const XScriptObject &o);

#endif // XSCRIPTVALUEDARTINTERNALS_H
