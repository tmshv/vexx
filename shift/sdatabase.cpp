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
  type->createProperty()(prop, type, (SPropertyInstanceInformation**)&prop->_instanceInfo);
  prop->_database = this;
  prop->_info = type;

  initiateProperty(prop);
  postInitiateProperty(prop);
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
      childInformation->createProperty()(thisProp, child->childInformation(), 0);
      }

    xAssert(thisProp->_parent == 0);
    xAssert(thisProp->_entity == 0);
    xAssert(thisProp->_nextSibling == 0);

    thisProp->_info = childInformation;
    thisProp->_instanceInfo = child;
    container->internalInsertProperty(true, thisProp, X_SIZE_SENTINEL);
    initiateProperty(thisProp);
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

  xAssert(prop->database());
  }

void SDatabase::postInitiateProperty(SProperty *prop)
  {
  SPropertyContainer *container = prop->castTo<SPropertyContainer>();
  if(container)
    {
    const SPropertyInformation *metaData = container->typeInformation();
    xAssert(metaData);

    for(xsize i=0, s=metaData->childCount(); i<s; ++i)
      {
      const SPropertyInstanceInformation *child = metaData->childFromIndex(i);

      SProperty *thisProp = child->locateProperty(container);
      postInitiateProperty(thisProp);
      }
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
  _blockSize << _done.size();
  }

void SDatabase::endBlock(bool cancel)
  {
  xAssert(_blockLevel > 0);
  _blockLevel--;

  xsize previousPoint = _blockSize.last();
  _blockSize.pop_back();
  if(cancel)
    {
    undoTo(previousPoint);
    }

  // wrap everything into one inform block
  if(_blockLevel == 0)
    {
    inform();
    }
  }

void SDatabase::undoTo(xsize p)
  {
  xAssert(p <= (xsize)_done.size());
  for(xsize i=((xsize)_done.size()-1); i>=p; --i)
    {
    SChange *c = _done[i];

    bool result = c->unApply() && c->inform();
    xAssert(result);

    // todo dont need this here, when undo fully implemented.B
    _done.pop_back();
    }
  }

QChar SDatabase::pathSeparator()
  {
  return QChar('/');
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
