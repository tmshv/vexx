#ifndef SCEMBEDDEDTYPES_H
#define SCEMBEDDEDTYPES_H

#include "scshiftproperty.h"
#include "scshiftpropertycontainer.h"
#include "scshiftentity.h"
#include "scshiftdatabase.h"
#include "styperegistry.h"

class QScriptEngine;

class ScEmbeddedTypes : STypeRegistry::Observer
  {
  XROProperty(QScriptEngine *, engine);
public:
  ScEmbeddedTypes(QScriptEngine *);
  virtual ~ScEmbeddedTypes();

  static QScriptValue packValue(SProperty *);

private:
  virtual void typeAdded(const SPropertyInformation *);
  virtual void typeRemoved(const SPropertyInformation *);

  void ensureTypeHierarchyAdded(const SPropertyInformation *);

  ScShiftProperty _property;
  ScShiftPropertyContainer _propertyContainer;
  ScShiftEntity _entity;
  ScShiftDatabase _database;

  static ScEmbeddedTypes *_types;
  };

#endif // SCEMBEDDEDTYPES_H
