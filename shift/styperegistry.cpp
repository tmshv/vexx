#include "styperegistry.h"
#include "sentity.h"
#include "sarrayproperty.h"
#include "sreferenceentity.h"
#include "sdatabase.h"

static XHash <SPropertyType, const SPropertyInformation *> _types;

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

const XHash <SPropertyType, const SPropertyInformation *> &STypeRegistry::types()
  {
  return _types;
  }

void STypeRegistry::addType(const SPropertyInformation *t)
  {
  xAssert(!_types.values().contains(t));
  if(!_types.values().contains(t))
    {
    _types.insert(t->typeId(), t);
    }
  }

const SPropertyInformation *STypeRegistry::findType(xuint32 i)
  {
  SProfileFunction
  if(_types.contains(i))
    {
    return _types.value(i);
    }
  return 0;
  }

const SPropertyInformation *STypeRegistry::findType(const QString &in)
  {
  SProfileFunction
  QList <xuint32> keys(_types.keys());
  for(int i=0, s=keys.size(); i<s; ++i)
    {
    SPropertyType key = keys[i];
    if(_types[key]->typeName() == in)
      {
      return _types.value(key);
      }
    }
  return 0;
  }
