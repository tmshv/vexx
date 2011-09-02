#include "item.h"
#include "sproperty.h"
#include "sdatabase.h"
#include "styperegistry.h"

void severityCompute(const SPropertyInstanceInformation *, SPropertyContainer *container)
  {
  Item *item = container->uncheckedCastTo<Item>();

  item->severity = item->priority() * item->links.size();
  }

S_IMPLEMENT_PROPERTY(Item)

SPropertyInformation *Item::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<Item>("Item");

  info->add(&Item::description, "description");

  FloatProperty::InstanceInformation* sevInfo = info->add(&Item::severity, "severity");
  sevInfo->setDefault(1.0f);
  sevInfo->setCompute(severityCompute);

  FloatProperty::InstanceInformation* priInfo = info->add(&Item::priority, "priority");
  priInfo->setDefault(1.0f);
  priInfo->setAffects(sevInfo);

  PointerArray::InstanceInformation* linksInfo = info->add(&Item::links, "links");
  linksInfo->setAffects(sevInfo);

  return info;
  }


Item::Item() : priority(0)
  {
  }

void Item::addDependancy(Item *item)
  {
  xAssert(item);
  links.addPointer(item);
  }

void Item::removeDependancy(Item *item)
  {
  xAssert(item);
  links.removePointer(item);
  }

bool Item::acceptsChild(const SEntity *ent) const
  {
  return ent->castTo<Item>() != 0;
  }
