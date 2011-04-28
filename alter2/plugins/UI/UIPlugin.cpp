#include "UIPlugin.h"
#include "UIVerticalLayout.h"
#include "UIHorizontalLayout.h"
#include "UIStackedLayout.h"
#include "UIWorkspace.h"
#include "UISurface.h"
#include "acore.h"
#include "QMainWindow"
#include "QDockWidget"
#include "QDialog"
#include "QVBoxLayout"
#include "QCloseEvent"
#include "QPushButton"
#include "splugin.h"
#include "aplugin.h"
#include "UISurfacePrivate.h"
#include "UIDatabaseDebugSurface.h"
#include "UIGraphDebugSurface.h"
#include "UIProfilerSurface.h"

ALTER_PLUGIN(UIPlugin);

class UIPluginPrivate : public QMainWindow
  {
public:
  UIPluginPrivate() : _plugin(0)
    {
    }

  virtual void closeEvent(QCloseEvent *event)
    {
    emit _plugin->aboutToClose();
    event->accept();
    }

  XList <UISurface*> _surfaces;
  QMainWindow *_mainWindow;
  QDialog *_preferences;
  QTabWidget *_preferenceTabs;
  UIDatabaseDebugSurface _dbDebug;
  UIGraphDebugSurface _graphDebug;
  UIProfilerSurface _profiler;
  UIPlugin *_plugin;
  };

UIPlugin::UIPlugin() : _priv(new UIPluginPrivate())
  {
  _priv->_plugin = this;
  }

void UIPlugin::load()
  {
  _priv->show();

  _priv->_preferences = new QDialog(_priv);
  _priv->_preferences->setModal(true);

  QVBoxLayout *prefLayout(new QVBoxLayout(_priv->_preferences));
  prefLayout->setContentsMargins(5,5,5,5);
  _priv->_preferenceTabs = new QTabWidget(_priv->_preferences);
  prefLayout->addWidget(_priv->_preferenceTabs);

  QHBoxLayout *prefSubLayout(new QHBoxLayout());
  prefLayout->addLayout(prefSubLayout);
  prefSubLayout->addStretch();
  QPushButton *close(new QPushButton("&Close", _priv->_preferences));
  QObject::connect(close, SIGNAL(clicked()), _priv->_preferences, SLOT(close()));
  prefSubLayout->addWidget(close);

  APlugin<SPlugin> shift(this, "db");
  if(shift.isValid())
    {
    SDatabase &db = shift->db();

    db.addType<UIWorkspace>();
    db.addType<UILayout>();
    db.addType<UIStackedLayout>();
    db.addType<UIVerticalLayout>();
    db.addType<UIHorizontalLayout>();

    if(core()->isDebugUIEnabled())
      {
      _priv->_dbDebug.setDatabase(&db);
      addSurface(&_priv->_dbDebug);
      _priv->_graphDebug.setEntity(&db);
      addSurface(&_priv->_graphDebug);
      }

#ifdef X_PROFILING_ENABLED
    addSurface(&_priv->_profiler);
#endif
    }
  }

void UIPlugin::unload()
  {
  foreach(UISurface *s, _priv->_surfaces)
    {
    s->privateData()->setPlugin(0);
    }

  delete _priv;
  }

void UIPlugin::addSurface(UISurface *surface)
  {
  if(!_priv->_surfaces.contains(surface))
    {
    if(!surface->privateData())
      {
      surface->setPrivateData(new UISurfacePrivate);
      surface->privateData()->setSurface(surface);
      }

    surface->privateData()->setPlugin(this);

    if(surface->type() == UISurface::Dock)
      {
      surface->privateData()->setDock(new QDockWidget(surface->name(), _priv));
      surface->privateData()->dock()->setWidget(surface->widget());
      _priv->addDockWidget(Qt::LeftDockWidgetArea, surface->privateData()->dock());
      }

    if(surface->type() == UISurface::PropertiesPage)
      {
      _priv->_preferenceTabs->addTab(surface->widget(), surface->name());
      }

    _priv->_surfaces << surface;
    }
  }

void UIPlugin::removeSurface(QString name)
  {
  foreach(UISurface *s, _priv->_surfaces)
    {
    if(s->name() == name)
      {
      removeSurface(s);
      }
    }
  }

void UIPlugin::removeSurface(UISurface *s)
  {
  _priv->_surfaces.removeAll(s);
  s->privateData()->setPlugin(0);
  }
