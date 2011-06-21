#include "spropertyinformation.h"
#include "spropertycontainer.h"

SPropertyInstanceInformation::SPropertyInstanceInformation()
  : _childInformation(0), _name(""), _location(0), _compute(0), _queueCompute(defaultQueue),
    _affects(0), _index(X_SIZE_SENTINEL), _entityChild(false), _extra(false), _dynamic(false)
  {
  }

SPropertyInformation::SPropertyInformation(CreateFunction createFn,
                     CreateInstanceInformationFunction cIIF,
                     SaveFunction saveFn,
                     LoadFunction loadFn,
                     AssignFunction assignFn,
                     xuint32 version,
                     const QString &typeName,
                     xuint32 typeId,
                     const SPropertyInformation *parent,
                     xsize size,
                     xsize instanceInfoSize)
    : _create(createFn), _createInstanceInformation(cIIF), _save(saveFn), _load(loadFn), _assign(assignFn),
    _version(version), _typeName(typeName), _typeId(typeId), _parentTypeInformation(parent),
    _propertyOffset(0), _size(size), _instanceInformationSize(instanceInfoSize), _instances(0), _dynamic(false)
  {
  if(_parentTypeInformation)
    {
    _propertyOffset = _parentTypeInformation->completeChildCount();
    }
  }

SPropertyType g_maxDynamicCount = 0x7FFFFFFF;
SPropertyInformation::SPropertyInformation(CreateFunction createFn,
                     CreateInstanceInformationFunction cIIF,
                     SaveFunction saveFn,
                     LoadFunction loadFn,
                     AssignFunction assignFn,
                     xuint32 version,
                     const QString &typeName,
                     const SPropertyInformation *parent,
                     xsize size,
                     xsize instanceInfoSize)
    : _create(createFn), _createInstanceInformation(cIIF), _save(saveFn), _load(loadFn), _assign(assignFn),
    _version(version), _typeName(typeName), _typeId(g_maxDynamicCount++), _parentTypeInformation(parent),
    _propertyOffset(0), _size(size), _instanceInformationSize(instanceInfoSize),
    _instances(0), _dynamic(false)
  {
  if(_parentTypeInformation)
    {
    _propertyOffset = _parentTypeInformation->completeChildCount();
    }
  }

SPropertyInformation::~SPropertyInformation()
  {
  for(xsize i=0; i<childCount(); ++i)
    {
    delete [] child(i)->affects();
    delete child(i);
    }
  }

const SPropertyInstanceInformation *SPropertyInformation::completeChild(xsize index) const
  {
  const SPropertyInformation *metaData = this;
  xAssert(index < metaData->completeChildCount());
  while(metaData && index < metaData->propertyOffset())
    {
    metaData = metaData->parentTypeInformation();
    }

  if(metaData)
    {
    xAssert(index >= metaData->propertyOffset() && index < metaData->completeChildCount());
    return metaData->child(index - metaData->propertyOffset());
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

void SPropertyInstanceInformation::setComputable(ComputeFunction computeFn, SProperty SPropertyContainer::* *affects)
  {
  _compute = computeFn;
  _affects = affects;
  }

void SPropertyInstanceInformation::initiate(const SPropertyInformation *info,
                 const QString &name,
                 xsize index,
                 SProperty SPropertyContainer::* location)
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
    if(siblingInfo->affects())
      {
      xsize i=0;
      while(siblingInfo->affects()[i])
        {
        SProperty *thisProp = (SProperty*)&(cont->*(siblingInfo->affects()[i]));
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

bool SPropertyInformation::inheritsFromType(SPropertyType match) const
  {
  const SPropertyInformation *type = this;
  while(type)
    {
    if(type->typeId() == match)
      {
      return true;
      }
    type = type->parentTypeInformation();
    }
  return false;
  }

xsize SPropertyInformation::completeChildCount() const
  {
  return propertyOffset() + children().size();
  }

void SPropertyInformation::reference() const
  {
  ++((SPropertyInformation*)this)->_instances;
  }

void SPropertyInformation::dereference() const
  {
  --((SPropertyInformation*)this)->_instances;
  }
