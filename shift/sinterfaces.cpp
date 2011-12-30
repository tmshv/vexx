#include "sinterfaces.h"
#include "sbaseproperties.h"
#include "QColor"

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

SBasicColourInterface::SBasicColourInterface() : SPropertyColourInterface(true)
  {
  }

XColour SBasicColourInterface::colour(const SProperty *t) const
  {
  xuint32 h = qHash(t->typeInformation()->typeName());

  float lightness = (float)(h & 0xFF) / 0xFF;
  float saturation = (float)((h >> 8) & 0xFF) / 0xFF;
  float hue = (float)((h >> 16) & 0xFFF) / 0xFFF; // skip last nibble

  int hueI = hue * 360.0f; // 0.0 -> 360.0
  int satI = ((saturation / 2.0f) + 0.5f) * 255.0f; // 0.5 -> 1.0
  int ligI = ((lightness / 2.0f) + 0.25f) * 255.0f; // 0.25 -> 0.75

  QColor col = QColor::fromHsl(hueI, satI, ligI);

  return XColour(col);
  }
