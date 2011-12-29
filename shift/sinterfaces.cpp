#include "sinterfaces.h"
#include "sbaseproperties.h"

#define PositionName "__position"

SBasicPositionInterface::SBasicPositionInterface() : SPropertyPositionInterface(true)
  {
  }

XVector3D SBasicPositionInterface::position(const SProperty *p) const
  {
  const SEntity *ent = p->uncheckedCastTo<SEntity>();
  const Vector3DProperty *prop = ent->findChild<Vector3DProperty>(PositionName);
  if(prop)
    {
    return prop->value();
    }
  xAssert(!ent->findChild("position"));
  return XVector3D::Zero();
  }

void SBasicPositionInterface::setPosition(SProperty *p, const XVector3D &val) const
  {
  SEntity *ent = p->uncheckedCastTo<SEntity>();
  Vector3DProperty *prop = ent->uncheckedCastTo<SEntity>()->findChild<Vector3DProperty>(PositionName);
  if(!prop)
    {
    prop = ent->addProperty<Vector3DProperty>(PositionName);
    }
  xAssert(prop);

  prop->assign(val);
  }
