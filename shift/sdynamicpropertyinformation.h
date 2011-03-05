#ifndef SDYNAMICTYPEINFORMATION_H
#define SDYNAMICTYPEINFORMATION_H

#include "sglobal.h"
#include "spropertyinformation.h"

class SDatabase;
class QString;
class SPropertyInformation;

class SHIFT_EXPORT SDynamicPropertyInformation
{
  SDynamicPropertyInformation(const QString &name, SPropertyType id, xuint32 version);

  void setParentType(SPropertyInformation *);
  void setParentType(const QString &name);
  void setParentType(SPropertyType id);

  void setSaveFunction(SPropertyInformation::SaveFunction fn);
  void setLoadFunction(SPropertyInformation::LoadFunction fn);
  void setAssignFunction(SPropertyInformation::AssignFunction fn);

  void addProperty(SPropertyType type, const QString &name, SPropertyInformation::ComputeFunction computeFn=0, const XList<xsize> &affects=XList<xsize>());
  void addProperty(const QString& type, const QString &name, SPropertyInformation::ComputeFunction computeFn=0, const XList<xsize> &affects=XList<xsize>());
  
  void addType(SDatabase *db);
};

#endif // SDYNAMICTYPEINFORMATION_H
