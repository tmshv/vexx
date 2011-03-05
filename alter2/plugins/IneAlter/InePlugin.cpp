#include "InePlugin.h"
#include "IneInterface.h"
#include "EnttecUSBProDriver/EnttecUSBProDriver.h"
#include "IneDriver.h"
#include "QDebug"
#include "acore.h"
#include "UIPlugin.h"
#include "splugin.h"
#include "aplugin.h"
#include "IneProperties.h"
#include "IneFixtureManager.h"
#include "IneRoot.h"
#include "IneFixture.h"
#include "sreferenceEntity.h"

ALTER_PLUGIN( InePlugin );

InePlugin::InePlugin() : _properties(new IneProperties(this)), _fixtureManager(0), _ineRoot(0), _fixtureRoot(0)
  {
  }

InePlugin::~InePlugin()
  {
  delete _enttecDriver;
  delete _properties;
  delete _fixtureManager;
  }

void InePlugin::load()
  {
  setInterface( this );
  _enttecDriver = new EnttecUSBProDriver();
  addDriver(_enttecDriver);
  }

void InePlugin::unload()
  {
  APlugin<UIPlugin> ui(this, UIPlugin::Type);
  if(ui.isValid())
    {
    ui->removeSurface(_properties->name());
    }
  }

void InePlugin::pluginAdded(xuint32)
  {
  APlugin<UIPlugin> ui(this, UIPlugin::Type);
  APlugin<SPlugin> shift(this, SPlugin::Type);

  if(ui.isValid())
    {
    ui->addSurface(_properties);
    }

  updateFromDB();

  if(ui.isValid() && shift.isValid() && _fixtureRoot)
    {
    _fixtureManager = new IneFixtureManager(_fixtureRoot);
    ui->addSurface(_fixtureManager);
    }
  }

void InePlugin::updateFromDB()
  {
  APlugin<SPlugin> shift(this, SPlugin::Type);
  if(shift.isValid())
    {
    shift->db().addType<IneRoot>("IneRoot");
    shift->db().addType<IneFixture>("IneFixture");
    _ineRoot = shift->db().root()->resolvePath<IneRoot>("Ine");
    if(!_ineRoot)
      {
      _ineRoot = shift->db().createEntity<IneRoot>("Ine");
      }
    _fixtureRoot = shift->db().root()->resolvePath<SReferenceEntity>("Ine|Fixtures");
    if(!_fixtureRoot)
      {
      _fixtureRoot = shift->db().createEntity<SReferenceEntity>("Fixtures", _ineRoot);
      }
    if(_fixtureManager)
      {
      _fixtureManager->setRoot(_fixtureRoot);
      }
    }
  }

const XList<IneInterface*> &InePlugin::interfaces()
  {
  return _interfaces;
  }

const XList<IneDriver*> &InePlugin::drivers()
  {
  return _drivers;
  }

void InePlugin::refreshInterfaces()
  {
  foreach(IneInterface *in, _interfaces)
    {
    delete in;
    }
  _interfaces.clear();

  foreach(IneDriver *dr, _drivers)
    {
    dr->refreshUniverses(this);
    }
  }

void InePlugin::addDriver(IneDriver *in)
  {
  _drivers << in;
  }

void InePlugin::removeDriver(IneDriver *in)
  {
  _drivers.removeAll(in);
  }


void InePlugin::error(const QString &in)
  {
  qDebug() << in;
  }

void InePlugin::addUniverse(IneInterface *in)
  {
  _interfaces << in;
  }

void InePlugin::removeUniverse(IneInterface *in)
  {
  _interfaces.removeAll(in);
  }
