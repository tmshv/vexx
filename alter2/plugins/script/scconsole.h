#ifndef SCCONSOLE_H
#define SCCONSOLE_H

#include "XInterface.h"

class ScConsole
  {
public:
  static XInterfaceBase* createInterface();

private:
  static XScriptValue logJS(XScriptArguments const &);
  static void logDart(XScriptDartArguments );
  };

X_SCRIPTABLE_TYPE(ScConsole)

#endif // SCCONSOLE_H
