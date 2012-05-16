#include "styperegistry.h"
#include "sentity.h"
#include "sarrayproperty.h"
#include "sreferenceentity.h"
#include "sbasepointerproperties.h"
#include "sdatabase.h"
#include "XAllocatorBase"
#include "sobserver.h"
#include "XBucketAllocator"
#include "spropertygroup.h"
#include "spropertyinformation.h"

struct TypeData
  {
  QVector <const SPropertyGroup *> groups;
  QVector <const SPropertyInformation *> types;
  QList <STypeRegistry::Observer *> observers;

  XAllocatorBase *allocator;
  };

static TypeData _internalTypes;

STypeRegistry::STypeRegistry()
  {
  }

void STypeRegistry::initiateInternalTypes()
  {
  XScriptEngine::initiate();

  _internalTypes.allocator = new XBucketAllocator();
  }

void STypeRegistry::initiate()
  {
  addPropertyGroup(shiftPropertyGroup());
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
  if(!_internalTypes.allocator)
    {
    initiateInternalTypes();
    }

  xAssert(_internalTypes.allocator);
  return _internalTypes.allocator;
  }

void STypeRegistry::addPropertyGroup(SPropertyGroup &g)
  {
  _internalTypes.groups << &g;
  foreach(const SPropertyInformation *info, g.containedTypes())
    {
    addType(info);
    }

  g._added = true;
  }

const QVector <const SPropertyGroup *> &STypeRegistry::groups()
  {
  return _internalTypes.groups;
  }

const QVector <const SPropertyInformation *> &STypeRegistry::types()
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
  xAssert(!findType(t->typeName()));
  if(!_internalTypes.types.contains(t))
    {
    _internalTypes.types << t;
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
