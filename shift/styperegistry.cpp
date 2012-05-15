#include "styperegistry.h"
#include "sentity.h"
#include "sarrayproperty.h"
#include "sreferenceentity.h"
#include "sbasepointerproperties.h"
#include "sdatabase.h"
#include "XAllocatorBase"
#include "sobserver.h"
#include "XBucketAllocator"

struct TypeData
  {
  XSet <const SPropertyInformation *> types;
  XList <STypeRegistry::Observer *> observers;
  XAllocatorBase *allocator;
  };

static TypeData _internalTypes;

STypeRegistry::STypeRegistry()
  {
  }

void STypeRegistry::initiate()
  {
  XScriptEngine::initiate();

  _internalTypes.allocator = new XBucketAllocator();

  addType(SProperty::staticTypeInformation());
  addType(SPropertyContainer::staticTypeInformation());
  addType(SPropertyArray::staticTypeInformation());

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
  addType(StringPropertyBase::staticTypeInformation());
  addType(FilenameProperty::staticTypeInformation());
  addType(StringProperty::staticTypeInformation());
  addType(ColourProperty::staticTypeInformation());
  addType(ByteArrayProperty::staticTypeInformation());

  addType(StringArrayProperty::staticTypeInformation());

  addType(Pointer::staticTypeInformation());
  addType(PointerArray::staticTypeInformation());

  addType(SFloatArrayProperty::staticTypeInformation());
  addType(SUIntArrayProperty::staticTypeInformation());

  addType(SFloatArrayProperty::staticTypeInformation());
  addType(SUIntArrayProperty::staticTypeInformation());
  addType(SVector2ArrayProperty::staticTypeInformation());
  addType(SVector3ArrayProperty::staticTypeInformation());
  addType(SVector4ArrayProperty::staticTypeInformation());

  addType(SEntity::staticTypeInformation());
  addType(SDatabase::staticTypeInformation());
  addType(SReferenceEntity::staticTypeInformation());


  SEntity::staticTypeInformation()->addStaticInterface(new SBasicPositionInterface);
  SProperty::staticTypeInformation()->addStaticInterface(new SBasicColourInterface);

  XInterface<STreeObserver> *treeObs = XInterface<STreeObserver>::create("_STreeObserver");
  treeObs->seal();
  }

void STypeRegistry::terminate()
{
  // script engine needs to access type info.
  XScriptEngine::terminate();

  delete _internalTypes.allocator;
  _internalTypes.allocator = 0;
  }

XAllocatorBase *STypeRegistry::allocator()
  {
  xAssert(_internalTypes.allocator);
  return _internalTypes.allocator;
  }

const XSet <const SPropertyInformation *> &STypeRegistry::types()
  {
  return _internalTypes.types;
  }

void STypeRegistry::addType(const SPropertyInformation *t)
  {
  internalAddType(t);
  foreach(Observer *o, _internalTypes.observers)
    {
    o->typeAdded(t);
    }
  }

void STypeRegistry::addTypeObserver(Observer *o)
  {
  _internalTypes.observers << o;
  }

void STypeRegistry::removeTypeObserver(Observer *o)
  {
  _internalTypes.observers.removeAll(o);
  }

void STypeRegistry::internalAddType(const SPropertyInformation *t)
  {
  xAssert(t);
  if(!_internalTypes.types.contains(t))
    {
    _internalTypes.types.insert(t);
    }
  }

const SPropertyInformation *STypeRegistry::findType(const QString &in)
  {
  SProfileFunction
  foreach(const SPropertyInformation *info, _internalTypes.types)
    {
    if(info->typeName() == in)
      {
      return info;
      }
    }
  return 0;
  }
