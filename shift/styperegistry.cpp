#include "styperegistry.h"
#include "sentity.h"
#include "sarrayproperty.h"
#include "sreferenceentity.h"
#include "sbasepointerproperties.h"
#include "sdatabase.h"

struct TypeData
  {
  XSet <const SPropertyInformation *> types;
  XList <STypeRegistry::Observer *> observers;
  };

static TypeData _types;

STypeRegistry::STypeRegistry()
  {
  }

void STypeRegistry::initiate()
  {
  addType(SProperty::staticTypeInformation());
  addType(SPropertyContainer::staticTypeInformation());
  addType(SPropertyArray::staticTypeInformation());

  addType(SEntity::staticTypeInformation());
  addType(SDatabase::staticTypeInformation());
  addType(SReferenceEntity::staticTypeInformation());

  addType(BoolProperty::staticTypeInformation());
  addType(IntProperty::staticTypeInformation());
  addType(LongIntProperty::staticTypeInformation());
  addType(UnsignedIntProperty::staticTypeInformation());
  addType(LongUnsignedIntProperty::staticTypeInformation());
  addType(FloatProperty::staticTypeInformation());
  addType(DoubleProperty::staticTypeInformation());
  addType(Vector2DProperty::staticTypeInformation());
  addType(Vector3DProperty::staticTypeInformation());
  addType(Vector4DProperty::staticTypeInformation());
  addType(QuaternionProperty::staticTypeInformation());
  addType(StringProperty::staticTypeInformation());
  addType(ColourProperty::staticTypeInformation());
  addType(ByteArrayProperty::staticTypeInformation());

  addType(Pointer::staticTypeInformation());
  addType(PointerArray::staticTypeInformation());

  addType(SFloatArrayProperty::staticTypeInformation());
  addType(SUIntArrayProperty::staticTypeInformation());
  }

const XSet <const SPropertyInformation *> &STypeRegistry::types()
  {
  return _types.types;
  }

void STypeRegistry::addType(const SPropertyInformation *t)
  {
  internalAddType(t);
  foreach(Observer *o, _types.observers)
    {
    o->typeAdded(t);
    }
  }

void STypeRegistry::addTypeObserver(Observer *o)
  {
  _types.observers << o;
  }

void STypeRegistry::removeTypeObserver(Observer *o)
  {
  _types.observers.removeAll(o);
  }

void STypeRegistry::internalAddType(const SPropertyInformation *t)
  {
  if(!_types.types.contains(t))
    {
    _types.types.insert(t);
    }
  }

const SPropertyInformation *STypeRegistry::findType(const QString &in)
  {
  SProfileFunction
  foreach(const SPropertyInformation *info, _types.types)
    {
    if(info->typeName() == in)
      {
      return info;
      }
    }
  return 0;
  }
