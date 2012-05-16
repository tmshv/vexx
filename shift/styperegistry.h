#ifndef STYPEREGISTRY_H
#define STYPEREGISTRY_H

#include "sglobal.h"
#include "XSet"

class SPropertyGroup;
class SPropertyInformation;

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

  static const QVector <const SPropertyGroup *> &groups();
  static const QVector <const SPropertyInformation *> &types();
  static void addPropertyGroup(SPropertyGroup &);

  static const SPropertyInformation *findType(const QString &);


private:
  STypeRegistry();
  X_DISABLE_COPY(STypeRegistry);

  static void addType(const SPropertyInformation *);
  static void internalAddType(const SPropertyInformation *);

  static void initiateInternalTypes();
  };

#endif // STYPEREGISTRY_H
