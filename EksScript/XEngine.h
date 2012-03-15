#ifndef XENGINE_H
#define XENGINE_H

#include "XScriptGlobal.h"

class EKSSCRIPT_EXPORT XEngine
  {
public:
  XEngine();
  ~XEngine();

  static void adjustAmountOfExternalAllocatedMemory(int in);

  class Impl;

  class Unlocker
    {
    //v8::Unlocker
    };

private:
  Impl *_impl;
  };

#endif // XENGINE_H
