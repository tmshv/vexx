#ifndef SINTERFACE_H
#define SINTERFACE_H

#define S_INTERFACE_FACTORY_TYPE(interfaceType) public: \
  typedef interfaceType InterfaceType; \
  private:

class SInterfaceBase;
class SProperty;

class SInterfaceBaseFactory
  {
  S_INTERFACE_FACTORY_TYPE(SInterfaceBase);
public:

  virtual SInterfaceBase *classInterface(SProperty *prop) { return 0; }
  };

#define S_INTERFACE_TYPE(typeId) public: \
  enum { InterfaceType = SInterfaceTypes::typeId }; \
  virtual xuint32 interfaceTypeId() const { return InterfaceType; } \
  private:

class SInterfaceBase
  {
  S_INTERFACE_TYPE(Invalid)
  };

#endif // SINTERFACE_H
