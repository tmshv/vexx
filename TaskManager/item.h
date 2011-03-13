#ifndef ITEM_H
#define ITEM_H

#include "sentity.h"
#include "sbaseproperties.h"
#include "XList"
#include "taskglobal.h"

class Item;
typedef XList<Item*> Items;

class Item : public SEntity
  {
  S_ENTITY(Item, SEntity, 0);

public:
  Item();

  LongStringProperty description;
  FloatProperty priority;
  FloatProperty severity;
  PointerArray links;

  void addDependancy(Item *);
  void removeDependancy(Item *);

private:
  virtual bool acceptsChild(const SEntity *) const;
  };

#endif // TASK_H
