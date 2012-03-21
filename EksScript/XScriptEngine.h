#ifndef XSCRIPTENGINE_H
#define XSCRIPTENGINE_H

#include "XScriptGlobal.h"

class EKSSCRIPT_EXPORT XScriptEngine
  {
public:
  XScriptEngine();
  ~XScriptEngine();

  static void adjustAmountOfExternalAllocatedMemory(int in);

  class Impl;

private:
  Impl *_impl;
  };

#endif // XSCRIPTENGINE_H
