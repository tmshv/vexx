#include "scembeddedtypes.h"
#include "spropertyinformation.h"
#include "sproperty.h"
#include "sentity.h"
#include "sdatabase.h"
#include "sarrayproperty.h"

ScEmbeddedTypes *ScEmbeddedTypes::_types = 0;

ScEmbeddedTypes::ScEmbeddedTypes(QScriptEngine *eng) :
    _engine(eng),
    _property(eng),
    _propertyContainer(eng),
    _entity(eng),
    _database(eng),
    _floatArrayProperty(eng)
  {
  xAssert(_types == 0);
  _types = this;

  _property.initiate();
  _propertyContainer.initiate();
  _entity.initiate();
  _database.initiate();
  _floatArrayProperty.initiate();

  SProperty::staticTypeInformation()->addStaticInterface(&_property);
  SPropertyContainer::staticTypeInformation()->addStaticInterface(&_propertyContainer);
  SEntity::staticTypeInformation()->addStaticInterface(&_entity);
  SDatabase::staticTypeInformation()->addStaticInterface(&_database);
  SFloatArrayProperty::staticTypeInformation()->addStaticInterface(&_floatArrayProperty);

  foreach(const SPropertyInformation *p, STypeRegistry::types())
    {
    ensureTypeHierarchyAdded(p);
    }

  STypeRegistry::addTypeObserver(this);
  }

void ScEmbeddedTypes::ensureTypeHierarchyAdded(const SPropertyInformation *p)
  {
  const SPropertyInformation *parent = p->parentTypeInformation();
  if(parent)
    {
    QScriptValue g = _engine->globalObject();
    if(!g.property(parent->typeName()).isValid())
      {
      ensureTypeHierarchyAdded(parent);
      }
    }

  typeAdded(p);
  }

void ScEmbeddedTypes::typeAdded(const SPropertyInformation *p)
  {
  QScriptValue g = _engine->globalObject();
  if(!g.property(p->typeName()).isValid())
    {
    QScriptValue v = _engine->newObject();
    v.setProperty("typeName", p->typeName());

    const SPropertyInformation *parentInfo = p->parentTypeInformation();
    if(parentInfo)
      {
      QScriptValue parent = g.property(parentInfo->typeName());
      xAssert(parent.isObject());
      if(parent.isObject())
        {
        v.setPrototype(parent);
        }
      }

    g.setProperty(p->typeName(), v);
    }
  }

void ScEmbeddedTypes::typeRemoved(const SPropertyInformation *)
  {
  }

ScEmbeddedTypes::~ScEmbeddedTypes()
  {
  _types = 0;
  STypeRegistry::removeTypeObserver(this);
  }

QScriptValue ScEmbeddedTypes::packValue(SProperty *prop)
  {
  ScProfileFunction
  if(!prop)
    {
    return QScriptValue();
    }

  ScScriptInterface *interface = prop->interface<ScScriptInterface>();
  xAssert(interface);

  QScriptClass* classType = interface->scriptClass();

  QScriptValue v = _types->engine()->newObject(classType, _types->engine()->newVariant(qVariantFromValue(prop)));

  QScriptValue proto = _types->engine()->globalObject().property(prop->typeInformation()->typeName());
  v.setPrototype(proto.isObject());
  v.setPrototype(proto);
  return v;
  }
