#ifndef SPROPERTYINFORMATIONAPIUTILITIES_H
#define SPROPERTYINFORMATIONAPIUTILITIES_H

#include "spropertyinformation.h"
#include "spropertygroup.h"
#include "XInterface.h"
#include "sproperty.h"

#define S_IMPLEMENT_PROPERTY(myName, grp) \
  static const SPropertyInformation *_##myName##StaticTypeInformation = myName :: bootstrapStaticTypeInformation();\
  const SPropertyInformation *myName::staticTypeInformation() { return _##myName##StaticTypeInformation; } \
  const SPropertyInformation *myName::bootstrapStaticTypeInformation() \
  { return SPropertyInformation::bootstrapTypeInformation<myName>(&_##myName##StaticTypeInformation, \
  #myName, myName::ParentType::bootstrapStaticTypeInformation(), grp::propertyGroup()); }

#define S_IMPLEMENT_ABSTRACT_PROPERTY(myName, grp) \
  S_IMPLEMENT_PROPERTY(myName, grp)

namespace
{
template <typename PropType> struct ApiHelper
  {
public:
  static void create(SPropertyInformation *info)
    {
    const XInterface<PropType::ParentType> *parentTempl =
        PropType::ParentType::staticTypeInformation()->apiInterface<PropType::ParentType>();
    const XInterface<SProperty> *baseTempl = SProperty::staticTypeInformation()->apiInterface<SProperty>();

    XInterface<PropType> *templ = XInterface<PropType>::createWithParent(info->typeName(), parentTempl, baseTempl);
    info->setApiInterface(templ);
    }
  };

template <> struct ApiHelper<SProperty>
  {
public:
  static void create(SPropertyInformation *info)
    {
    XInterface<SProperty> *templ = XInterface<SProperty>::create(info->typeName());
    info->setApiInterface(templ);
    }
  };

template<typename T, bool Abstract = T::IsAbstract> struct PropertyHelper;

template<typename T> struct PropertyHelper<T, true>
  {
  static void create(SProperty *)
    {
    xAssertFail();
    }
  static void destroy(SProperty *)
    {
    xAssertFail();
    }
  };

template<typename T> struct PropertyHelper<T, false>
  {
  static void create(SProperty *ptr)
    {
    new(ptr) T();
    }
  static void destroy(SProperty *ptr)
    {
    ((T*)ptr)->~T();
    }
  };

template <typename T> struct InstanceInformationHelper
  {
  static void create(SPropertyInstanceInformation *allocation)
    {
    new(allocation) T::InstanceInformation;
    }
  static void destroy(SPropertyInstanceInformation *allocation)
    {
    ((T*)allocation)->~T();
    }
  };
}

template <typename PropType> void SPropertyInformation::initiate(SPropertyInformation *info, const char *typeName)
  {
  // update copy constructor too
  info->setCreateProperty(PropertyHelper<PropType>::create);
  info->setDestroyProperty(PropertyHelper<PropType>::destroy);
  info->setCreateInstanceInformation(InstanceInformationHelper<PropType>::create);
  info->setDestroyInstanceInformation(InstanceInformationHelper<PropType>::destroy);
  info->setSave(PropType::saveProperty);
  info->setLoad(PropType::loadProperty);
  info->setShouldSave(PropType::shouldSaveProperty);
  info->setShouldSaveValue(PropType::shouldSavePropertyValue);
  info->setAssign(PropType::assignProperty);
  info->setPostCreate(0);
  info->setPostChildSet(PropType::postChildSet);
  info->setVersion(PropType::Version);
  info->setSize(sizeof(PropType));
  info->setInstanceInformationSize(sizeof(typename PropType::InstanceInformation));

  info->_createTypeInformation = PropType::createTypeInformation;
  info->_instances = 0;
  info->_extendedParent = 0;

  info->_typeName = typeName;

  ApiHelper<PropType>::create(info);
  }

template <typename T>
const SPropertyInformation *SPropertyInformation::bootstrapTypeInformation(const SPropertyInformation **info,
                                                                           const char *name,
                                                                           const SPropertyInformation *parent,
                                                                           SPropertyGroup& group)
  {
  if(*info)
    {
    return *info;
    }

  *info = SPropertyInformation::createTypeInformation<T>(name, parent);
  group.registerPropertyInformation(*info);
  return *info;
  }

template <typename T> XInterface<T> *SPropertyInformation::apiInterface()
  {
  return static_cast<XInterface<T>*>(_apiInterface);
  }

template <typename T> const XInterface<T> *SPropertyInformation::apiInterface() const
  {
  return static_cast<const XInterface<T>*>(_apiInterface);
  }

namespace XScriptConvert
{
namespace internal
{
template <> struct SHIFT_EXPORT JSToNative<SPropertyInformation>
  {
  typedef const SPropertyInformation *ResultType;

  ResultType operator()(XScriptValue const &h) const;
  };

template <> struct SHIFT_EXPORT NativeToJS<SPropertyInformation>
  {
  XScriptValue operator()(const SPropertyInformation *x) const;
  XScriptValue operator()(const SPropertyInformation &x) const;
  };
}
}

template <typename T>
  static const SPropertyInformation *SPropertyInformation::createTypeInformation(const char *name,
                                                            const SPropertyInformation *parentType)
  {
  SProfileFunction

  typedef void (*FnType)(SPropertyInformation *, const char *);
  FnType fn = SPropertyInformation::initiate<T>;

  return createTypeInformationInternal(name, parentType, fn);
  }

#endif // SPROPERTYINFORMATIONAPIUTILITIES_H
