#ifndef SBASEPOINTERPROPERTIES_H
#define SBASEPOINTERPROPERTIES_H

#include "sproperty.h"
#include "sdatabase.h"
#include "sinterfaces.h"

class SHIFT_EXPORT Pointer : public SProperty
  {
  S_PROPERTY(Pointer, SProperty, 0);

public:
  typedef SProperty Type;

  const SProperty *pointed() const { preGet(); return input(); }
  const SProperty *operator()() const { preGet(); return pointed(); }

  void setPointed(const SProperty *prop);
  Pointer &operator=(const SProperty *prop) { setPointed(prop); return *this; }

  const SPropertyInformation *pointedToType() const;
  static const SPropertyInformation *pointedToType(const SPropertyInformation *);

  static void assignPointerInformation(SPropertyInformation *newInfo, const SPropertyInformation *pointedToInfo);
  };

template <typename T> class TypedPointer : public Pointer
  {
public:
  typedef T Type;

  // pre gets here to make sure we clear any dirty flags that have flowed in through
  // dependencies
  T *pointed() { preGet(); return input() ? input()->castTo<T>() : 0; }
  T *operator()() { preGet(); return pointed(); }

  const T *pointed() const { preGet(); return input() ? input()->castTo<T>() : 0; }
  const T *operator()() const { preGet(); return pointed(); }

  void setPointed(const T *prop) { Pointer::setPointed(prop); }
  Pointer &operator=(const T *prop) { setPointed(prop); return *this; }
  };

template <typename PTR> class TypedPointerArray : public STypedPropertyArray<PTR>
  {
public:
  PTR* addPointer(const typename PTR::Type *prop)
    {
    SHandler* db = SProperty::handler();
    xAssert(db);

    SBlock b(db);
    PTR *p = STypedPropertyArray<PTR>::add();
    xAssert(p);
    p->setPointed(prop);

    return p;
    }

  void removePointer(const typename PTR::Type *ptr)
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

  bool hasPointer(const typename PTR::Type *ptr)
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


class SPointerComputeLock
  {
public:
  SPointerComputeLock(Pointer *p) : _p(p) { }
  ~SPointerComputeLock() { _p->pointed(); }

private:
  Pointer *_p;
  };

S_PROPERTY_INTERFACE(Pointer)

#define S_TYPED_POINTER_TYPE(name, type) \
  class name : public TypedPointer<type> { \
    S_PROPERTY(name, Pointer, 0); }; \
    S_IMPLEMENT_INLINE_PROPERTY(name) \
  inline void name::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data) { \
  if(data.registerInterfaces) { \
    assignPointerInformation(info, type::staticTypeInformation()); } } \
  S_PROPERTY_INTERFACE(name)

template <typename T, typename TYPE> void createTypedPointerArray(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerInterfaces)
    {
    typedef TYPE::Type PtrType;

    class PointerArrayConnectionInterface : public SPropertyConnectionInterface
      {
    public:
      PointerArrayConnectionInterface() : SPropertyConnectionInterface(true) { }
      virtual void connect(SProperty *driven, const SProperty *driver) const
        {
        SBlock b(driven->database());

        T* arr = driven->castTo<T>();
        xAssert(arr);

        const PtrType* ptr = driver->castTo<PtrType>();
        if(ptr)
          {
          arr->addPointer(ptr);
          }
        else
          {
          arr->setInput(driver);
          }
        }
      };

    info->addStaticInterface(new PointerArrayConnectionInterface);

    XInterface<T>* api = info->apiInterface<T>();


    XInterfaceBase::Function fn;
    fn = XScript::MethodToInCa<TypedPointerArray<TYPE>, TYPE *(const PtrType *), &T::addPointer>::Call;
    api->addFunction("addPointer", fn);

    fn = XScript::MethodToInCa<TypedPointerArray<TYPE>, bool (const PtrType *), &T::hasPointer>::Call;
    api->addFunction("hasPointer", fn);

    fn = XScript::MethodToInCa<TypedPointerArray<TYPE>, void (const PtrType *), &T::removePointer>::Call;
    api->addFunction("removePointer", fn);
    }
  }

#define S_TYPED_POINTER_ARRAY_TYPE(name, type) \
  class name : public TypedPointerArray<type> { \
  S_PROPERTY_CONTAINER(PointerArray, SPropertyContainer, 0); }; \
  S_IMPLEMENT_INLINE_PROPERTY(name) \
  inline void name::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data) { createTypedPointerArray<name, type>(info, data); } \
  S_PROPERTY_INTERFACE(name) \
  S_PROPERTY_INTERFACE(TypedPointerArray<type>)

S_TYPED_POINTER_ARRAY_TYPE(PointerArray, Pointer)

#endif // SBASEPOINTERPROPERTIES_H
