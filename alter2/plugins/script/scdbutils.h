#ifndef SCDBUTILS_H
#define SCDBUTILS_H

#include "XConvert.h"
#include "spropertyinformation.h"
#include "styperegistry.h"

class XScriptArguments;
class XScriptValue;

XScriptValue registerTypeFn(XScriptArguments const &args);
XScriptValue registerExporterFn(XScriptArguments const &args);

#endif // SCDBUTILS_H
