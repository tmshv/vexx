#ifndef SINTERFACE_H
#define SINTERFACE_H

#include "sproperty.h"

#define S_INTERFACE_FACTORY_TYPE(interfaceType) public: \
  typedef interfaceType InterfaceType; \
  private:

class SInterfaceBase;

class SInterfaceBaseFactory
  {
  S_INTERFACE_FACTORY_TYPE(SInterfaceBase);
XProperties:
  XROProperty(xsize, referenceCount);
  XROProperty(bool, deleteOnNoReferences);

public:
  SInterfaceBaseFactory(bool del) : _referenceCount(0), _deleteOnNoReferences(del) { }
  virtual SInterfaceBase *classInterface(SProperty *prop) { return 0; }

private:
  friend class SPropertyInformation;
  };

#define S_INTERFACE_TYPE(typeId) public: \
  enum { InterfaceType = SInterfaceTypes::typeId }; \
  virtual xuint32 interfaceTypeId() const { return InterfaceType; } \
  private:

class SInterfaceBase : public SProperty::UserData
  {
  S_INTERFACE_TYPE(Invalid)
  };


class SStaticInterfaceBase : public SInterfaceBase, public SInterfaceBaseFactory
  {
public:
  SStaticInterfaceBase(bool deleteOnNoReferences) : SInterfaceBaseFactory(deleteOnNoReferences) { }
  virtual SInterfaceBase *classInterface(SProperty *) { return this; }
  };

#endif // SINTERFACE_H
