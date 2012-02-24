#ifndef XSCRIPT_H
#define XSCRIPT_H

#include "XScriptGlobal.h"

class EKSSCRIPT_EXPORT XScript
  {
public:
  XScript(const char *data);

  void run();

private:
  void *_impl[1];
  };


#endif // XSCRIPT_H
