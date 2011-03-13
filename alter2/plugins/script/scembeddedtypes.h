#ifndef SCEMBEDDEDTYPES_H
#define SCEMBEDDEDTYPES_H

#include "scshiftdynamicpropertyinformation.h"

class QScriptEngine;

class ScEmbeddedTypes
  {
public:
  ScEmbeddedTypes(QScriptEngine *);

private:
  ScShiftDynamicPropertyInformation _dynamicPropertyInformation;
  };

#endif // SCEMBEDDEDTYPES_H
