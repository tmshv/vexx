#ifndef INEMANAGER_H
#define INEMANAGER_H

#include "XList"
#include "IneGlobal.h"

class IneInterface;
class IneDriver;
class QString;

class IneManager
  {
public:
  virtual void error(const QString &) = 0;

  virtual void refreshInterfaces() = 0;
  virtual void addDriver(IneDriver *) = 0;
  virtual void removeDriver(IneDriver *) = 0;

  virtual void addUniverse(IneInterface *) = 0;
  virtual void removeUniverse(IneInterface *) = 0;

  virtual const XList<IneInterface*> &interfaces() = 0;
  virtual const XList<IneDriver*> &drivers() = 0;
  };

#endif // INEMANAGER_H
