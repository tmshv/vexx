#ifndef SENTITY_H
#define SENTITY_H

#include "sglobal.h"
#include "spropertycontainer.h"
#include "spropertyarray.h"
#include "sobserver.h"
#include "XWeakShared"

class SProperty;
class SDatabase;
class STreeObserver;
class SDataObserver;
class SConnectionObserver;

#define S_ENTITY(name, parent, version) S_PROPERTY_CONTAINER(name, parent, version)

#define S_ENTITY_DEFINITION S_PROPERTY_CONTAINER_DEFINITION
#define S_ENTITY_END_DEFINITION(name, parent) S_PROPERTY_CONTAINER_END_DEFINITION(name, parent, saveEntity, loadEntity)

#define S_ENTITY_EMPTY_DEFINITION(name, parent) S_PROPERTY_CONTAINER_EMPTY_DEFINITION(name, parent, saveEntity, loadEntity)

#define S_ENTITY_COMPUTATION(function, ...)


class SHIFT_EXPORT SEntity : public SPropertyContainer, public XWeakSharedData
  {
  S_ENTITY(SEntity, SPropertyContainer, 0);

public:
  SEntity();

  SPropertyArray children;

  void reparent(SEntity *);

  template <typename T>T *addChild(const QString& name)
    {
    T *ent = children.add<T>();
    xAssert(ent);
    ent->setName(name);
    return ent;
    }

  SEntity *parentEntity() const;
  SEntity *firstChildEntity() const;
  SEntity *nextSiblingEntity() const;

  virtual bool acceptsChild(const SEntity *) const;
  virtual bool acceptsParent(const SEntity *) const;

  void addDataObserver(SDataObserver *);
  void addTreeObserver(STreeObserver *);
  void addConnectionObserver(SConnectionObserver *);

  void removeDataObserver(SDataObserver *);
  void removeTreeObserver(STreeObserver *);
  void removeConnectionObserver(SConnectionObserver *);
  
  void informDataObservers(int m, const DataChange *event, SObservers &obs);
  void informTreeObservers(int m, const SChange *event, SObservers &obs);
  void informConnectionObservers(int m, const SChange *event, SObservers &obs);

protected:
  static void saveEntity(const SProperty *, SPropertyData &, SPropertyData::Mode);
  static void loadEntity(SProperty *, const SPropertyData &, xuint32, SPropertyData::Mode, SLoader &);

private:
  struct ObserverStruct
    {
    enum
      {
      Data,
      Tree,
      Connection
      };
    xuint8 mode;
    SObserver *observer;
    };
  XList<ObserverStruct> _observers;
  };

#endif // SENTITY_H
