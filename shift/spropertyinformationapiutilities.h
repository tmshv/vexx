#ifndef SPROPERTYINFORMATIONAPIUTILITIES_H
#define SPROPERTYINFORMATIONAPIUTILITIES_H

#include "spropertyinformation.h"
#include "XInterface.h"

template <typename T> XInterfaceBase *initiateAPIInterface(const SPropertyInformation *info)
  {
  if(info->parentTypeInformation())
    {
    //XInterface<T> *bTempl = XInterface<T>::createWithParent("Inheritor", aTempl);
    }

  return 0;
  }

template <typename T> XInterface<T> *SPropertyInformation::apiInterface()
  {
  }

#endif // SPROPERTYINFORMATIONAPIUTILITIES_H
