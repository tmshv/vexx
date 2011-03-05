#ifndef SPROPERTYARRAY_H
#define SPROPERTYARRAY_H

#include "spropertycontainer.h"

class SHIFT_EXPORT SPropertyArray : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(SPropertyArray, SPropertyContainer);

public:
  SPropertyArray();
  virtual ~SPropertyArray();

  template <typename T> T *add()
    {
    return addProperty(T::Type, X_SIZE_SENTINEL)->castTo<T>();
    }

  SProperty *operator[](xsize i) { return at(i); }
  SProperty *at(xsize i);
  void clear();

  void remove(SProperty *);
  };

template <typename T> class STypedPropertyArray : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(STypedPropertyArray, SPropertyContainer);

public:
  T *add()
    {
    return addProperty(T::Type)->castTo<T>();
    }

  void remove(SProperty *prop)
    {
    removeProperty(prop);
    }

  void resize(xsize s)
    {
    xsize sz = size();
    xsize dif = s - sz;

    if(dif > 0)
      {
      for(xsize x=0; x<s; ++x)
        {
        addProperty(false, internalCreateHelper(T::Type, database()));
        }
      }
    else if(dif < 0)
      {
      for(xsize x=s; x<sz; ++x)
        {
        // could be optimised lots...
        removeProperty(at(x));
        }
      }
    }

  T *operator[](xsize i) { return at(i); }
  T *at(xsize i)
    {
    SProperty *c = firstChild();
    while(c && i)
      {
      --i;
      c = c->nextSibling();
      }

    return c;
    }
  };

template <typename T>const SPropertyInformation *STypedPropertyArray<T>::staticTypeInformation()
  {
  const SPropertyInformation* parentInfo = SPropertyContainer::staticTypeInformation();
  static SPropertyInformation metaData =
    {
    STypedPropertyArray<T>::createSTypedPropertyArray,
    saveContainer,
    loadContainer,
    assignContainer,
    0,
    "STypedPropertyArray<T>",
    Type,
    parentInfo,
    parentInfo->propertyOffset + parentInfo->propertyCount,
    0,
    0
    };
  return &metaData;
  }

#endif // SPROPERTYARRAY_H
