#include "styperegistry.h"
#include "sentity.h"
#include "sarrayproperty.h"
#include "sreferenceentity.h"
#include "sdatabase.h"

static XSet <const SPropertyInformation *> _types;

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
  return _types;
  }

void STypeRegistry::addType(const SPropertyInformation *t)
  {
  xAssert(!_types.contains(t));
  if(!_types.contains(t))
    {
    _types.insert(t);
    }
  }

const SPropertyInformation *STypeRegistry::findType(const QString &in)
  {
  SProfileFunction
  foreach(const SPropertyInformation *info, _types)
    {
    if(info->typeName() == in)
      {
      return info;
      }
    }
  return 0;
  }
