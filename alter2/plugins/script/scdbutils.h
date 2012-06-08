#ifndef SCDBUTILS_H
#define SCDBUTILS_H

#include "XConvert.h"
#include "spropertyinformation.h"
#include "styperegistry.h"

class XScriptArguments;
class XScriptValue;

class ScDbUtils
  {
public:
  static XInterfaceBase* createInterface();


private:
  static XScriptValue registerTypeJS(XScriptArguments const &args);

  static XScriptValue registerType(const XScriptObject &args);
  //XScriptValue registerExporterFn(const XScriptValue &args);
  //XScriptValue registerExporterFnJS(XScriptArguments const &args);

  //void registerExporterFnDart(XScriptDartArguments args);
  };

X_SCRIPTABLE_TYPE(ScDbUtils)

#endif // SCDBUTILS_H
