#ifndef XCONTEXT_H
#define XCONTEXT_H

#include "XScriptGlobal.h"
#include "XEngine.h"
#include "XInterfaceObject.h"

class EKSSCRIPT_EXPORT XContext
  {
public:
  XContext(XEngine *e);
  ~XContext();

  void set(const char* in, const XInterfaceObject& obj);

  void addInterface(const XInterfaceBase *i);

private:
  XEngine* _engine;
  class Impl;
  Impl *_impl;
  };

#endif // XCONTEXT_H
