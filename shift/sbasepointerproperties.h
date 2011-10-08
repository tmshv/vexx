#ifndef SBASEPOINTERPROPERTIES_H
#define SBASEPOINTERPROPERTIES_H

#include "sproperty.h"
#include "sdatabase.h"

class SHIFT_EXPORT Pointer : public SProperty
  {
  S_PROPERTY(Pointer, SProperty, 0);

public:
  typedef SProperty Type;

  const SProperty *pointed() const { preGet(); return input(); }
  const SProperty *operator()() const { preGet(); return pointed(); }

  void setPointed(const SProperty *prop);
  Pointer &operator=(const SProperty *prop) { setPointed(prop); return *this; }
  };

template <typename T> class TypedPointer : public Pointer
  {
public:
  typedef T Type;

  T *pointed() { return input() ? input()->castTo<T>() : 0; }
  T *operator()() { return pointed(); }

  const T *pointed() const { return input() ? input()->castTo<T>() : 0; }
  const T *operator()() const { return pointed(); }

  void setPointed(const T *prop) { Pointer::setPointed(prop); }
  Pointer &operator=(const T *prop) { setPointed(prop); return *this; }
  };

template <typename PTR> class TypedPointerArray : public STypedPropertyArray<PTR>
  {
public:
  PTR* addPointer(typename PTR::Type *prop)
    {
    SDatabase* db = SProperty::database();
    xAssert(db);

    SBlock b(db);
    PTR *p = STypedPropertyArray<PTR>::add();
    xAssert(p);
    p->setPointed(prop);

    return p;
    }

  void removePointer(typename PTR::Type *ptr)
    {
    SProperty *c = SPropertyContainer::firstChild();
    while(c)
      {
      Pointer *cT = c->castTo<Pointer>();
      if(cT)
        {
        if(cT->pointed() == ptr)
          {
          STypedPropertyArray<PTR>::remove(c);
          return;
          }
        }
      c = c->nextSibling();
      }
    }

  bool hasPointer(typename PTR::Type *ptr)
    {
    SProperty *child = SPropertyContainer::firstChild();
    while(child)
      {
      if(ptr == child->input())
        {
        return true;
        }
      child = child->nextSibling();
      }
    return false;
    }
  };

#define S_TYPED_POINTER_TYPE(name, type) \
  class name : public TypedPointer<type> { \
    S_PROPERTY(name, Pointer, 0); }; \
    S_IMPLEMENT_INLINE_PROPERTY(name) \
  inline SPropertyInformation *name::createTypeInformation() { \
    return SPropertyInformation::create<name>(#name); }

#define S_TYPED_POINTER_ARRAY_TYPE(name, type) \
  class name : public TypedPointerArray<type> { \
  S_PROPERTY_CONTAINER(PointerArray, STypedPropertyArray<type>, 0); }; \
  S_IMPLEMENT_INLINE_PROPERTY(name) \
  inline SPropertyInformation *name::createTypeInformation() { \
    return SPropertyInformation::create<name>(#name); }


S_TYPED_POINTER_ARRAY_TYPE(PointerArray, Pointer)
#endif // SBASEPOINTERPROPERTIES_H
