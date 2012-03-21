#ifndef XSCRIPTSOURCE_H
#define XSCRIPTSOURCE_H

#include "XScriptGlobal.h"

class XScriptValue;

class EKSSCRIPT_EXPORT XScriptSource
  {
public:
  XScriptSource(const QString &data);

  XScriptValue run(bool *error=0);

private:
  void *_impl;
  };


#endif // XSCRIPTSOURCE_H
