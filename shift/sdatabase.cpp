#include "sdatabase.h"
#include "sentity.h"
#include "schange.h"
#include "QFile"
#include "QRegExp"
#include "QDebug"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SDatabase)

SPropertyInformation *SDatabase::createTypeInformation()
  {
  SPropertyInformation* info = SPropertyInformation::create<SDatabase>("SDatabase");
  info->add(&SDatabase::majorVersion, "majorVersion")->setDefault(0);
  info->add(&SDatabase::minorVersion, "minorVersion")->setDefault(0);
  info->add(&SDatabase::revision, "revision")->setDefault(0);
  return info;
  }

SDatabase::SDatabase() : _blockLevel(0)
  {
  _database = this;
  _info = staticTypeInformation();
  _instanceInfo = &_instanceInfoData;
  }

SDatabase::~SDatabase()
  {
  // clear out our child elements before the allocator is destroyed.
  SProperty *prop = _child;
  while(prop)
    {
    SProperty *next = prop->_nextSibling;
    if(prop->index() >= _containedProperties)
      {
      database()->deleteDynamicProperty(prop);
      }
    prop = next;
    }
  _child = 0;

  foreach(SChange *ch, _done)
    {
    destoryChangeMemory(ch);
    }

  this->~SEntity();
  xAssert(_memory.empty());
  }

SProperty *SDatabase::createDynamicProperty(const SPropertyInformation *type)
  {
  SProfileFunction
  xAssert(type);

  SProperty *prop = (SProperty*)_memory.alloc(type->dynamicSize());
  type->create()(prop, type, (SPropertyInstanceInformation**)&prop->_instanceInfo);
  prop->_database = this;
  prop->_info = type;

  initiateProperty(prop);
  return prop;
  }

void SDatabase::deleteDynamicProperty(SProperty *prop)
  {
  uninitiateProperty(prop);

  SEntity *ent = prop->castTo<SEntity>();
  if(ent)
    {
    if(ent->parentEntity())
      {
      ent->parentEntity()->children.internalRemoveProperty(ent);
      }
    }
  prop->~SProperty();
  _memory.free(prop);
  }

void SDatabase::initiateInheritedDatabaseType(const SPropertyInformation *info)
  {
  _info = info;
  initiatePropertyFromMetaData(this, info, false);
  }

void SDatabase::initiatePropertyFromMetaData(SPropertyContainer *container, const SPropertyInformation *mD, bool includeParents)
  {
  xAssert(mD);

  for(xsize i=0, s=mD->childCount(); i<s; ++i)
    {
    // no contained properties with duplicated names...
    const SPropertyInstanceInformation *child = mD->child(i);

    // extract the properties location from the meta data.
    const SProperty SPropertyContainer::* prop(child->location());
    SProperty *thisProp = (SProperty*)&(container->*prop);

    if(child->extra())
      {
      child->childInformation()->create()(thisProp, child->childInformation(), 0);
      }

    xAssert(thisProp->_parent == 0);
    xAssert(thisProp->_entity == 0);
    xAssert(thisProp->_nextSibling == 0);

    thisProp->_info = child->childInformation();
    thisProp->_instanceInfo = child;

    container->internalInsertProperty(true, thisProp, X_SIZE_SENTINEL);
    initiateProperty(thisProp);
    child->initiateProperty(thisProp);
    }
  }

void SDatabase::uninitiatePropertyFromMetaData(SPropertyContainer *container, const SPropertyInformation *mD)
  {
  xAssert(mD);

  for(xsize i=0; i<mD->childCount(); ++i)
    {
    // no contained properties with duplicated names...
    const SPropertyInstanceInformation *child = mD->child(i);

    // extract the properties location from the meta data.
    const SProperty SPropertyContainer::* prop(child->location());
    SProperty *thisProp = (SProperty*)&(container->*prop);

    uninitiateProperty(thisProp);

    if(child->extra())
      {
      thisProp->~SProperty();
      }
    }
  }

void SDatabase::initiateProperty(SProperty *prop)
  {
  prop->typeInformation()->reference();

  SPropertyContainer *container = prop->castTo<SPropertyContainer>();
  if(container)
    {
    const SPropertyInformation *metaData = container->typeInformation();
    xAssert(metaData);

    initiatePropertyFromMetaData(container, metaData);
    }
  xAssert(prop->database());
  }

void SDatabase::uninitiateProperty(SProperty *prop)
  {
  prop->typeInformation()->dereference();

  SPropertyContainer *container = prop->castTo<SPropertyContainer>();
  if(container)
    {
    const SPropertyInformation *metaData = container->typeInformation();
    xAssert(metaData);

    uninitiatePropertyFromMetaData(container, metaData);
    }
  }

void SDatabase::beginBlock()
  {
  _blockLevel++;
  }

void SDatabase::endBlock()
  {
  xAssert(_blockLevel > 0);
  _blockLevel--;
  if(_blockLevel == 0)
    {
    inform();
    }
  }

QString SDatabase::pathSeparator()
  {
  return "/";
  }

QString SDatabase::propertySeparator()
  {
  return ":";
  }

SBlock::SBlock(SDatabase *db) : _db(db)
  {
  _db->beginBlock();
  }

SBlock::~SBlock()
  {
  _db->endBlock();
  }

void *SDatabase::allocateChangeMemory(xsize s)
  {
  SProfileFunction
  return _memory.alloc(s);
  }

void SDatabase::destoryChangeMemory(SChange *ptr)
  {
  SProfileFunction
  ptr->~SChange();
  _memory.free(ptr);
  }

void SDatabase::inform()
  {
  SProfileFunction
  foreach(SObserver *obs, _blockObservers)
    {
    obs->actOnChanges();
    }
  _blockObservers.clear();
  }
