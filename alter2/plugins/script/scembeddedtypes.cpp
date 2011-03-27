#include "scembeddedtypes.h"
#include "sproperty.h"
#include "sentity.h"
#include "sdatabase.h"

ScEmbeddedTypes *ScEmbeddedTypes::_types = 0;

ScEmbeddedTypes::ScEmbeddedTypes(QScriptEngine *eng) :
    _engine(eng),
    _dynamicPropertyInformation(eng),
    _property(eng),
    _propertyContainer(eng),
    _entity(eng),
    _database(eng)
  {
  xAssert(_types == 0);
  _types = this;

  _property.initiate();
  _propertyContainer.initiate();
  _entity.initiate();
  _database.initiate();
  }

ScEmbeddedTypes::~ScEmbeddedTypes()
  {
  _types = 0;
  }

QScriptValue ScEmbeddedTypes::packValue(SProperty *prop)
  {
  ScProfileFunction
  if(!prop)
    {
    return QScriptValue();
    }

  QScriptClass* classType = &_types->_property;

  if(prop->inheritsFromType<SDatabase>())
    {
    classType = &_types->_database;
    }
  else if(prop->inheritsFromType<SEntity>())
    {
    classType = &_types->_entity;
    }
  else if(prop->inheritsFromType<SPropertyContainer>())
    {
    classType = &_types->_propertyContainer;
    }

  return _types->engine()->newObject(classType, _types->engine()->newVariant(qVariantFromValue(prop)));
  }
