#ifndef SENTITYWEAKPOINTER_H
#define SENTITYWEAKPOINTER_H

#include "XWeakShared"
#include "sentity.h"
#include "sdatabase.h"

#define S_ENTITY_WEAK_POINTER_CHECK \
if(data() == 0 || (data()->parent() == 0 && !data()->inheritsFromType<SDatabase>())) { xAssertFail(); return 0; }

class SEntityWeakPointer : private XWeakSharedPointer<SEntity>
  {
public:
  explicit SEntityWeakPointer(SEntity *ptr=0) : XWeakSharedPointer<SEntity>(ptr)
    {
    }

  SEntity *operator=(SEntity *ptr)
    {
    XWeakSharedPointer<SEntity>::assign(ptr);
    return ptr;
    }

  void assign(SEntity *ptr)
    {
    XWeakSharedPointer<SEntity>::assign(ptr);
    }

  const SEntity *operator->() const
    {
    S_ENTITY_WEAK_POINTER_CHECK
    return data();
    }

  SEntity *operator->()
    {
    S_ENTITY_WEAK_POINTER_CHECK
    return data();
    }

  const SEntity *entity() const
    {
    S_ENTITY_WEAK_POINTER_CHECK
    return data();
    }

  SEntity *entity()
    {
    S_ENTITY_WEAK_POINTER_CHECK
    return data();
    }

  bool isValid() const
    {
    return data() != 0;
    }

  operator SEntity*()
    {
    return data();
    }

  operator const SEntity*() const
    {
    return data();
    }
  };

inline bool operator!=(SProperty *prop, const SEntityWeakPointer &ptr)
  {
  return prop != ptr.entity();
  }

#endif // SENTITYWEAKPOINTER_H
