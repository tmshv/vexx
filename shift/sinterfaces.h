#ifndef SINTERFACES_H
#define SINTERFACES_H

#include "XVector3D"
#include "sinterface.h"
#include "XColour"

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

class SPropertyColourInterface : public SStaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(SPropertyColourInterface, PropertyColourInterface);

public:
  SPropertyColourInterface(bool a) : SStaticInterfaceBase(a) { }
  virtual XColour colour(const SProperty *) const = 0;
  };

class SBasicColourInterface : public SPropertyColourInterface
  {
public:
  SBasicColourInterface();
  virtual XColour colour(const SProperty *) const;
  };


#endif // SINTERFACES_H
