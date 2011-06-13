#include "item.h"
#include "sproperty.h"
#include "sdatabase.h"

void severityCompute(const SPropertyInstanceInformation *, SPropertyContainer *container)
  {
  Item *item = container->uncheckedCastTo<Item>();

  item->severity = item->priority() * item->links.size();
  }

S_PROPERTY_CONTAINER_DEFINITION(Item, SEntity)
  S_COMPUTE_GROUP(severitySet)
    S_AFFECTS(severity)
  S_COMPUTE_GROUP_END()

  S_PROPERTY_DEFINITION(LongStringProperty, description, "")
  S_COMPUTABLE_PROPERTY_DEFINITION(FloatProperty, priority, 0, severitySet, 1.0f)
  S_COMPUTABLE_PROPERTY_DEFINITION(FloatProperty, severity, severityCompute, 0, 1.0f)
  S_COMPUTABLE_PROPERTY_DEFINITION(PointerArray, links, 0, severitySet)
S_ENTITY_END_DEFINITION(Item, SEntity)

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
