#include "spropertyinformation.h"

SPropertyInstanceInformation::SPropertyInstanceInformation(const SPropertyInformation *info,
                 const QString &name,
                 SProperty SPropertyContainer::* location,
                 ComputeFunction computeFn,
                 SProperty SPropertyContainer::* *affects)
    : _childInformation(info), _name(name), _location(location), _compute(computeFn), _affects(affects)
  {
  }

SPropertyInformation::SPropertyInformation(CreateFunction createFn,
                     SaveFunction saveFn,
                     LoadFunction loadFn,
                     AssignFunction assignFn,
                     xuint32 version,
                     const QString &typeName,
                     xuint32 typeId,
                     const SPropertyInformation *parent,
                     const XList<SPropertyInstanceInformation*> children,
                     xsize propertyOffset,
                     xsize size,
                     bool dynamic)
    : _create(createFn), _save(saveFn), _load(loadFn), _assign(assignFn), _version(version), _typeName(typeName),
    _typeId(typeId), _parentTypeInformation(parent), _children(children), _propertyOffset(propertyOffset),
    _size(size), _instances(0), _dynamic(dynamic)
  {
  }

SPropertyInformation::~SPropertyInformation()
  {
  if(dynamic())
    {
    for(xsize i=0; i<childCount(); ++i)
      {
      delete child(i)->affects();
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
