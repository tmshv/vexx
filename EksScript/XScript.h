#ifndef XSCRIPT_H
#define XSCRIPT_H

#include "XScriptGlobal.h"
#include "v8.h"

class EKSSCRIPT_EXPORT XScript
  {
public:
  XScript(const char *data);

  void run();

private:
  v8::Handle<v8::Script> _script;
  };


#endif // XSCRIPT_H
