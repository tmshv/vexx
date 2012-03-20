#ifndef SPROPERTYINFORMATIONAPIUTILITIES_H
#define SPROPERTYINFORMATIONAPIUTILITIES_H

#include "spropertyinformation.h"
#include "XInterface.h"

template <typename T, typename PARENT> XInterfaceBase *initiateAPIInterfaceWithParent(const SPropertyInformation *info)
  {
  const XInterface<PARENT> *parentTempl = XInterface<PARENT>::lookup();
  XInterface<T> *templ = XInterface<T>::createWithParent(info->typeName(), parentTempl);

  return templ;
  }

template <typename T> XInterfaceBase *initiateAPIInterface(const SPropertyInformation *info)
  {
  XInterface<T> *templ = XInterface<T>::create(info->typeName());

  return templ;
  }

template <typename T> XInterface<T> *SPropertyInformation::apiInterface()
  {
  return static_cast<XInterface<T>*>(_apiInterface);
  }

#endif // SPROPERTYINFORMATIONAPIUTILITIES_H
