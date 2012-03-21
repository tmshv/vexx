#ifndef XSCRIPTENGINE_H
#define XSCRIPTENGINE_H

#include "XScriptGlobal.h"

class EKSSCRIPT_EXPORT XScriptEngine
  {
public:
  XScriptEngine();
  ~XScriptEngine();

  static void adjustAmountOfExternalAllocatedMemory(int in);

  static void initiate();
  static void terminate();

  class Impl;

private:
  Impl *_impl;
  };

#endif // XSCRIPTENGINE_H
