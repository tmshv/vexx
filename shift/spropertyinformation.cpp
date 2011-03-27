#include "spropertyinformation.h"

SPropertyInstanceInformation::SPropertyInstanceInformation(const SPropertyInformation *info,
                 const QString &name,
                 xsize index,
                 SProperty SPropertyContainer::* location,
                 ComputeFunction computeFn,
                 SProperty SPropertyContainer::* *affects,
                 bool entityChild,
                 bool extra)
    : _childInformation(info), _name(name), _location(location), _compute(computeFn),
      _affects(affects), _index(index), _entityChild(entityChild), _extra(extra), _dynamic(false)
  {
  xAssert(location != 0);
  }

SPropertyInstanceInformation::SPropertyInstanceInformation(bool dynamic)
  : _childInformation(0), _name(""), _location(0), _compute(0),
    _affects(0), _index(X_SIZE_SENTINEL), _entityChild(false), _extra(false), _dynamic(dynamic)
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
                     const XList<SPropertyInstanceInformation*> children,
                     xsize size,
                     xsize instanceInfoSize)
    : _create(createFn), _createInstanceInformation(cIIF), _save(saveFn), _load(loadFn), _assign(assignFn),
    _version(version), _typeName(typeName), _typeId(typeId), _parentTypeInformation(parent), _children(children),
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
                     const XList<SPropertyInstanceInformation*> children,
                     xsize size,
                     xsize instanceInfoSize)
    : _create(createFn), _createInstanceInformation(cIIF), _save(saveFn), _load(loadFn), _assign(assignFn),
    _version(version), _typeName(typeName), _typeId(g_maxDynamicCount++), _parentTypeInformation(parent),
    _children(children), _propertyOffset(0), _size(size), _instanceInformationSize(instanceInfoSize),
    _instances(0), _dynamic(false)
  {
  if(_parentTypeInformation)
    {
    _propertyOffset = _parentTypeInformation->completeChildCount();
    }
  }

SPropertyInformation::~SPropertyInformation()
  {
  if(dynamic())
    {
    for(xsize i=0; i<childCount(); ++i)
      {
      delete [] child(i)->affects();
      delete child(i);
      }
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

void SPropertyInstanceInformation::setData(DataKey k, const QVariant &v)
  {
  xAssert(k < g_maxChildKey);
  _data[k].setValue(v);
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
