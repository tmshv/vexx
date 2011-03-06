#ifndef SDYNAMICTYPEINFORMATION_H
#define SDYNAMICTYPEINFORMATION_H

#include "sglobal.h"
#include "spropertyinformation.h"
#include "XProperty"
#undef properties

class SDatabase;
class QString;
class SPropertyInformation;

class SHIFT_EXPORT SDynamicPropertyInformation
  {
public:
  class Property
    {
  XProperties:
    XProperty(QString, typeName, setTypeName);
    XProperty(QString, name, setName);
    XProperty(SPropertyInformation::ComputeFunction, computeFunction, setComputeFunction);
    XRefProperty(XList<xsize>, affects);

  public:
    /*
    template <typename T> Property(const QString &name,  computeFn=0, const XList<xsize> &affects=XList<xsize>())
      {

      }

    Property(SPropertyType type, const QString &name, SPropertyInformation::ComputeFunction computeFn=0, const XList<xsize> &affects=XList<xsize>());
    */
    Property(const QString& type, const QString &name, SPropertyInformation::ComputeFunction computeFn=0, const XList<xsize> &affects=XList<xsize>());

    };

XProperties:
  XProperty(QString, typeName, setTypeName);
  XProperty(QString, parentTypeName, setParentTypeName);
  XProperty(xuint32, version, setVersion);

  XProperty(SPropertyInformation::SaveFunction, saveFunction, setSaveFunction);
  XProperty(SPropertyInformation::LoadFunction, loadFunction, setLoadFunction);
  XProperty(SPropertyInformation::AssignFunction, assignFunction, setAssignFunction);

  XRefProperty(XList<Property>, properties)


public:
  SDynamicPropertyInformation(const QString &name="", xuint32 version=0);

  bool isValid() const;
  void setParentTypeName(SPropertyInformation *);
  void addType(SDatabase *db);
  };

#endif // SDYNAMICTYPEINFORMATION_H
