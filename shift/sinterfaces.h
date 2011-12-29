#ifndef SINTERFACES_H
#define SINTERFACES_H

#include "XVector3D"
#include "sinterface.h"

class SPropertyVariantInterface : public SStaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(SPropertyVariantInterface, PropertyVariantInterface);

public:
  SPropertyVariantInterface(bool d) : SStaticInterfaceBase(d) { }
  virtual QString asString(const SProperty *) const = 0;
  virtual QVariant asVariant(const SProperty *) const = 0;
  virtual void setVariant(SProperty *, const QVariant &) const = 0;
  };

class SPropertyPositionInterface : public SStaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(SPropertyPositionInterface, PropertyPositionInterface);

public:
  SPropertyPositionInterface(bool a) : SStaticInterfaceBase(a) { }
  virtual XVector3D position(const SProperty *) const = 0;
  virtual void setPosition(SProperty *, const XVector3D &) const = 0;
  };

class SBasicPositionInterface : public SPropertyPositionInterface
  {
public:
  SBasicPositionInterface();

  virtual XVector3D position(const SProperty *) const;
  virtual void setPosition(SProperty *, const XVector3D &) const;
  };

#endif // SINTERFACES_H
