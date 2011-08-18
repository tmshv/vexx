#ifndef SCEMBEDDEDTYPES_H
#define SCEMBEDDEDTYPES_H

#include "scshiftproperty.h"
#include "scshiftpropertycontainer.h"
#include "scshiftentity.h"
#include "scshiftdatabase.h"
#include "scshiftfloatarrayproperty.h"

class QScriptEngine;

class ScEmbeddedTypes
  {
  XROProperty(QScriptEngine *, engine);
public:
  ScEmbeddedTypes(QScriptEngine *);
  ~ScEmbeddedTypes();

  static QScriptValue packValue(SProperty *);

private:
  ScShiftProperty _property;
  ScShiftPropertyContainer _propertyContainer;
  ScShiftEntity _entity;
  ScShiftDatabase _database;
  ScShiftFloatArrayProperty _floatArrayProperty;

  static ScEmbeddedTypes *_types;
  };

#endif // SCEMBEDDEDTYPES_H
