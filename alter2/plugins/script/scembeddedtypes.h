#if 0 //ndef SCEMBEDDEDTYPES_H
#define SCEMBEDDEDTYPES_H

#include "QObject"
#include "scshiftproperty.h"
#include "scshiftpropertycontainer.h"
#include "scshiftentity.h"
#include "scshiftdatabase.h"
#include "styperegistry.h"

class QScriptEngine;

class ScEmbeddedTypes : public QObject, STypeRegistry::Observer
  {
  Q_OBJECT
  XROProperty(QScriptEngine *, engine);

public:
  ScEmbeddedTypes(QScriptEngine *);
  virtual ~ScEmbeddedTypes();

  static QScriptValue packValue(SProperty *);

signals:
  void typeAdded(QString t);
  void typeRemoved(QString t);

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
