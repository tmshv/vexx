#ifndef INEPLUGIN_H
#define INEPLUGIN_H

#include "aabstractplugin.h"
#include "IneManager.h"

class IneProperties;
class IneFixtureManager;
class EnttecUSBProDriver;
class IneRoot;
class SReferenceEntity;

class InePlugin : public AAbstractPlugin, public IneManager
  {
  X_OBJECT( InePlugin, AAbstractPlugin, 201 )
public:
  InePlugin();
  ~InePlugin();

  virtual void refreshInterfaces();
  virtual void addDriver(IneDriver *);
  virtual void removeDriver(IneDriver *);

  virtual const XList<IneInterface*> &interfaces();
  virtual const XList<IneDriver*> &drivers();

private:
  void updateFromDB();

  virtual void load();
  virtual void unload();
  virtual void pluginAdded(xuint32 type);

  virtual void error(const QString &);
  virtual void addUniverse(IneInterface *);
  virtual void removeUniverse(IneInterface *);

  XList<IneDriver*> _drivers;
  XList<IneInterface*> _interfaces;
  IneProperties *_properties;
  IneFixtureManager *_fixtureManager;
  EnttecUSBProDriver *_enttecDriver;
  IneRoot *_ineRoot;
  SReferenceEntity *_fixtureRoot;
  };

#endif // INEPLUGIN_H
