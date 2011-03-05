#ifndef ENVIRONMENTENTITY_H
#define ENVIRONMENTENTITY_H

#include "XProperty"
#include "sentity.h"
#include "XEnvironment.h"
#if 0
class EnvironmentEntity : public SEntity
  {
  S_ENTITY(EnvironmentEntity, SEntity)

  XROProperty(XEnvironment *, environment);
  XROProperty(XEnvironment::ItemID, item);
  XROProperty(XEnvironment::ItemID, parent);
  XROProperty(xuint32, index);

public:
  enum
    {
    ID = SEntity::LastProperty,
    Position,
    Orientation,
    LastProperty,
    };

  EnvironmentEntity(XEnvironment *environment, XEnvironment::ItemID);

  void setItem(XEnvironment::ItemID item, XEnvironment::ItemID parent, xuint32 childIndex);

protected:
  virtual XVariant getData( SPropertyID id, SAttributeID attribute ) const;
  virtual bool setData( SPropertyID id, SAttributeID attribute, const XVariant &value );
  };
#endif
#endif // ENVIRONMENTENTITY_H
