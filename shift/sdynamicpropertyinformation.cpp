#include "sdynamicpropertyinformation.h"

SDynamicPropertyInformation::Property::Property(const QString& type,
                                                const QString &name,
                                                SPropertyInstanceInformation::ComputeFunction computeFn,
                                                const XList<xsize> &affects)
    : _typeName(type), _name(name), _computeFunction(computeFn), _affects(affects)
  {
  }

SDynamicPropertyInformation::SDynamicPropertyInformation(const QString &name, xuint32 version)
    : _typeName(name), _parentTypeName("SProperty"), _version(version)
  {
  }

bool SDynamicPropertyInformation::isValid() const
  {
  return _typeName != 0;
  }

void SDynamicPropertyInformation::setParentTypeName(SPropertyInformation *info)
  {
  _parentTypeName = info->typeName();
  }

void SDynamicPropertyInformation::addType(SDatabase *db)
  {
  }
