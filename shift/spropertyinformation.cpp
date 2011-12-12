#include "spropertyinformation.h"
#include "spropertycontainer.h"

SPropertyInstanceInformation::SPropertyInstanceInformation()
  : _childInformation(0), _name(""), _location(0), _compute(0), _computeLockedToMainThread(false),
    _queueCompute(defaultQueue), _affects(0), _index(X_SIZE_SENTINEL), _entityChild(false),
    _extra(false), _dynamic(false)
  {
  }

SPropertyInstanceInformation::~SPropertyInstanceInformation()
  {
  }

SPropertyInformation::SPropertyInformation()
    : _create(0), _createInstanceInformation(0), _save(0), _load(0), _assign(0), _postCreate(0),
    _version(0), _parentTypeInformation(0), _size(0), _instanceInformationSize(0),
    _dynamic(false), _instances(0), _extendedParent(0)
  {
  }

SPropertyInformation::~SPropertyInformation()
  {
  foreach(SInterfaceBaseFactory *f, _interfaceFactories)
    {
    xAssert(f->_referenceCount > 0);
    --f->_referenceCount;
    if(f->_referenceCount == 0 && f->_deleteOnNoReferences)
      {
      delete f;
      }
    }

  for(xsize i=0; i<childCount(); ++i)
    {
    delete [] child(i)->affects();
    delete child(i);
    }
  }

SPropertyInstanceInformation *SPropertyInformation::child(SProperty SPropertyContainer::* ptr)
  {
  SPropertyContainer *u = 0;
  SProperty *offset = &(u->*ptr);
  xptrdiff location = reinterpret_cast<xsize>(offset);
  xAssert(location > 0);
  return child(location);
  }

const SPropertyInstanceInformation *SPropertyInformation::child(SProperty SPropertyContainer::* ptr) const
  {
  SPropertyContainer *u = 0;
  SProperty *offset = &(u->*ptr);
  xptrdiff location = reinterpret_cast<xsize>(offset);
  xAssert(location > 0);
  return child(location);
  }

SPropertyInstanceInformation *SPropertyInformation::child(xsize location)
  {
  foreach(SPropertyInstanceInformation *i, _children)
    {
    if(i->location() == location)
      {
      return i;
      }
    }
  return 0;
  }

const SPropertyInstanceInformation *SPropertyInformation::child(xsize location) const
  {
  foreach(const SPropertyInstanceInformation *i, _children)
    {
    if(i->location() == location)
      {
      return i;
      }
    }
  return 0;
  }

const SPropertyInstanceInformation *SPropertyInformation::childFromIndex(xsize index) const
  {
  xAssert(index < childCount());
  return _children[index];
  }

SPropertyInstanceInformation *SPropertyInformation::childFromIndex(xsize index)
  {
  xAssert(index < childCount());
  return _children[index];
  }

const SPropertyInstanceInformation *SPropertyInformation::childFromName(const QString &in) const
  {
  foreach(SPropertyInstanceInformation *i, _children)
    {
    if(i->name() == in)
      {
      return i;
      }
    }
  return 0;
  }

SPropertyInstanceInformation *SPropertyInformation::childFromName(const QString &in)
  {
  foreach(SPropertyInstanceInformation *i, _children)
    {
    if(i->name() == in)
      {
      return i;
      }
    }
  return 0;
  }

SPropertyInformation::DataKey g_maxKey = 0;
SPropertyInformation::DataKey SPropertyInformation::newDataKey()
  {
  return g_maxKey++;
  }

SPropertyInstanceInformation::DataKey g_maxChildKey = 0;
SPropertyInstanceInformation::DataKey SPropertyInstanceInformation::newDataKey()
  {
  return g_maxChildKey++;
  }

void SPropertyInstanceInformation::setAffects(const SPropertyInstanceInformation *info)
  {
  xAssert(!_affects);
  xAssert(info);

  _affects = new xsize[2];
  _affects[0] = info->location();
  _affects[1] = 0;
  }

void SPropertyInstanceInformation::setAffects(xsize *affects)
  {
  xAssert(!_affects);
  _affects = affects;
  }

void SPropertyInstanceInformation::setDefaultInput(const SPropertyInstanceInformation *)
  {
  xAssertFail();
  }

void SPropertyInstanceInformation::initiate(const SPropertyInformation *info,
                 const QString &name,
                 xsize index,
                 xsize location)
  {
  _childInformation = info;
  _name = name;
  _location = location;
  _index = index;
  }

void SPropertyInstanceInformation::setData(DataKey k, const QVariant &v)
  {
  xAssert(k < g_maxChildKey);
  _data[k].setValue(v);
  }

void SPropertyInstanceInformation::defaultQueue(const SPropertyInstanceInformation *info, const SPropertyContainer *cont, SProperty **jobs, xsize &numJobs)
  {
  SProfileFunction
  for(SProperty *prop=cont->firstChild(); prop; prop=prop->nextSibling())
    {
    const SPropertyInstanceInformation *siblingInfo = prop->instanceInformation();
    if(siblingInfo->affects() && !siblingInfo->computeLockedToMainThread())
      {
      const SPropertyInformation *contInfo = cont->typeInformation();
      xsize i=0;
      while(siblingInfo->affects()[i])
        {
        const SPropertyInstanceInformation *affectsInst = contInfo->child(siblingInfo->affects()[i]);
        const SProperty *thisProp = affectsInst->locateProperty(cont);
        const SPropertyInstanceInformation *thisInfo = thisProp->instanceInformation();
        if(thisInfo == info)
          {
          jobs[numJobs++] = prop;
          }
        ++i;
        }
      }
    }
  }

bool SPropertyInformation::inheritsFromType(const SPropertyInformation *match) const
  {
  const SPropertyInformation *type = this;
  while(type)
    {
    if(type == match)
      {
      return true;
      }
    type = type->parentTypeInformation();
    }
  return false;
  }

void SPropertyInformation::reference() const
  {
  ++((SPropertyInformation*)this)->_instances;
  }

void SPropertyInformation::dereference() const
  {
  --((SPropertyInformation*)this)->_instances;
  }
