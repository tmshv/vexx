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
  clear();
  _child = 0;

  foreach(SChange *ch, _done)
    {
    xDestroyAndFree(changeAllocator(), SChange, ch);
    }

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

void SDatabase::deleteProperty(SProperty *prop)
  {
  xAssert(!prop->_flags.hasFlag(PreGetting));
  uninitiateProperty(prop);

  prop->~SProperty();
  }

void SDatabase::deleteDynamicProperty(SProperty *prop)
  {
  deleteProperty(prop);
  _memory.free(prop);
  }

void SDatabase::initiateInheritedDatabaseType(const SPropertyInformation *info)
  {
  _info = info;
  initiatePropertyFromMetaData(this, info);
  }

void SDatabase::initiatePropertyFromMetaData(SPropertyContainer *container, const SPropertyInformation *mD)
  {
  xAssert(mD);

  for(xsize i=0, s=mD->childCount(); i<s; ++i)
    {
    // no contained properties with duplicated names...
    const SPropertyInstanceInformation *child = mD->childFromIndex(i);
    const SPropertyInformation *childInformation = child->childInformation();

    // extract the properties location from the meta data.
    SProperty *thisProp = child->locateProperty(container);

    if(child->extra())
      {
      childInformation->create()(thisProp, child->childInformation(), 0);
      }

    xAssert(thisProp->_parent == 0);
    xAssert(thisProp->_entity == 0);
    xAssert(thisProp->_nextSibling == 0);

    thisProp->_info = childInformation;
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
    const SPropertyInstanceInformation *child = mD->childFromIndex(i);

    // extract the properties location from the meta data.
    SProperty *thisProp = child->locateProperty(container);

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

  const SPropertyInstanceInformation *inst = prop->instanceInformation();
  xAssert(inst);
  inst->initiateProperty(prop);

  const SPropertyInformation *info = prop->typeInformation();
  while(info)
    {
    SPropertyInformation::PostCreateFunction postCreate = info->postCreate();
    if(postCreate)
      {
      postCreate(prop);
      }
    info = info->parentTypeInformation();
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

SBlock::SBlock(SDatabase *db) : _db(db)
  {
  _db->beginBlock();
  }

SBlock::~SBlock()
  {
  _db->endBlock();
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
