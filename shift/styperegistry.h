#ifndef STYPEREGISTRY_H
#define STYPEREGISTRY_H

#include "sglobal.h"
#include "XSet"
#include "spropertyinformation.h"

class SHIFT_EXPORT STypeRegistry
  {
public:
  class Observer
    {
  public:
    virtual void typeAdded(const SPropertyInformation *) = 0;
    virtual void typeRemoved(const SPropertyInformation *) = 0;
    };

  static void addTypeObserver(Observer *o);
  static void removeTypeObserver(Observer *o);

  static void initiate();
  static void terminate();

  static XAllocatorBase *allocator();

  static const XSet <const SPropertyInformation *> &types();
  static void addType(const SPropertyInformation *);

  static const SPropertyInformation *findType(const QString &);

  static void internalAddType(const SPropertyInformation *);

private:
  STypeRegistry();
  X_DISABLE_COPY(STypeRegistry)
  };

#endif // STYPEREGISTRY_H
