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

  SProperty::staticTypeInformation()->addStaticInterface(&_property);
  _propertyContainer.initiate();
  _entity.initiate();
  _database.initiate();
  _floatArrayProperty.initiate();
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

  ScScriptInterface *interface = prop->interface<ScScriptInterface>();

  if(interface)
    {
    QScriptClass* classType = interface->scriptClass();

    return _types->engine()->newObject(classType, _types->engine()->newVariant(qVariantFromValue(prop)));
    }
  else
    {
    return QScriptValue();
    }
  }
