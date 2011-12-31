#include "sbasepointerproperties.h"

S_IMPLEMENT_PROPERTY(Pointer)

SPropertyInformation *Pointer::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<Pointer>("Pointer");

  assignPointerInformation(info, SProperty::staticTypeInformation());

  return info;
  }

void Pointer::setPointed(const SProperty *prop)
  {
  if(hasInput())
    {
    input()->disconnect(this);
    }

  if(prop)
    {
    prop->connect(this);
    }
  }

SPropertyInformation::DataKey g_pointerTypeKey = SPropertyInformation::newDataKey();
const SPropertyInformation *Pointer::pointedToType() const
  {
  return pointedToType(typeInformation());
  }

const SPropertyInformation *Pointer::pointedToType(const SPropertyInformation *info)
  {
  xAssert(info);

  QVariant var = info->data()[g_pointerTypeKey];
  if(!var.isValid())
    {
    var = pointedToType(info->parentTypeInformation());
    }
  xAssert(var.isValid());

  const SPropertyInformation *pointedTo = var.value<const SPropertyInformation*>();
  xAssert(pointedTo);

  return pointedTo;
  }

void Pointer::assignPointerInformation(SPropertyInformation *newInfo, const SPropertyInformation *pointedToInfo)
  {
  newInfo->setData(g_pointerTypeKey, QVariant::fromValue(pointedToInfo));
  }
