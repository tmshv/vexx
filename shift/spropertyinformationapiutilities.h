#ifndef SPROPERTYINFORMATIONAPIUTILITIES_H
#define SPROPERTYINFORMATIONAPIUTILITIES_H

#include "spropertyinformation.h"
#include "XInterface.h"

template <typename T, typename PARENT> XInterfaceBase *initiateApiInterfaceWithParent(const SPropertyInformation *info)
  {
  const XInterface<PARENT> *parentTempl = PARENT::staticTypeInformation()->apiInterface<PARENT>();
  XInterface<T> *templ = XInterface<T>::createWithParent(info->typeName(), parentTempl);

  return templ;
  }

template <typename T> XInterfaceBase *initiateApiInterface(const SPropertyInformation *info)
  {
  XInterface<T> *templ = XInterface<T>::create(info->typeName());

  return templ;
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
  XScriptValue operator()(SPropertyInformation const &x) const;
  };
}
}

#endif // SPROPERTYINFORMATIONAPIUTILITIES_H
