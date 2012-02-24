#ifndef XENGINE_H
#define XENGINE_H

#include "XScriptGlobal.h"
#include "v8.h"

class EKSSCRIPT_EXPORT XEngine
  {
public:
  XEngine();
  ~XEngine();

private:
  v8::HandleScope _scope;
  v8::Handle<v8::ObjectTemplate> _globalTemplate;
  friend class XContext;
  };

#endif // XENGINE_H
