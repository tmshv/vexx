#ifndef XSCRIPTCONTEXT_H
#define XSCRIPTCONTEXT_H

#include "XScriptGlobal.h"
#include "XScriptEngine.h"
#include "XScriptObject.h"

class EKSSCRIPT_EXPORT XScriptContext
  {
public:
  XScriptContext(XScriptEngine *e);
  ~XScriptContext();

  typedef XScriptValue (*Function)( XScriptArguments const & argv );

  void set(const QString& in, const XScriptObject& obj);
  void set(const QString& in, Function fn);

  void addInterface(const XInterfaceBase *i);

private:
  XScriptEngine* _engine;
  class Impl;
  Impl *_impl;
  };

#endif // XSCRIPTCONTEXT_H
