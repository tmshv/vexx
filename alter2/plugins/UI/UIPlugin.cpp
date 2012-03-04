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
#include "styperegistry.h"
#include "aplugin.h"
#include "UISurfacePrivate.h"
#include "UIDatabaseDebugSurface.h"
#include "UIGraphDebugSurface.h"
#include "UIProfilerSurface.h"
#include "UIEventLoggerSurface.h"

ALTER_PLUGIN(UIPlugin);

class UIPluginPrivate : public QMainWindow
  {
public:
  UIPluginPrivate() : _plugin(0)
    {
    setDockNestingEnabled(true);
    }

  virtual void closeEvent(QCloseEvent *event)
    {
    saveStateToDB();

    emit _plugin->aboutToClose();
    event->accept();
    }

  void saveStateToDB()
    {
    APlugin<SPlugin> shift(_plugin, "db");
    if(shift.isValid())
      {
      QByteArray geometryData = saveGeometry();
      shift->setSetting<QByteArray>("ui", "geometry", geometryData);

      QByteArray layoutData = saveState();
      shift->setSetting<QByteArray>("ui", "layout", layoutData);
      }
    }

  void restoreStateFromDB()
    {
    APlugin<SPlugin> shift(_plugin, "db");
    if(shift.isValid())
      {
      const QByteArray &geometry = shift->setting<QByteArray>("ui", "geometry");
      restoreGeometry(geometry);

      const QByteArray &layout = shift->setting<QByteArray>("ui", "layout");
      restoreState(layout);
      }
    }

  XList <UISurface*> _surfaces;
  QDialog *_preferences;
  QTabWidget *_preferenceTabs;
  UIPlugin *_plugin;
  };

UIPlugin::UIPlugin() : _priv(new UIPluginPrivate())
  {
  _priv->_plugin = this;
  }

void UIPlugin::load()
  {
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

    STypeRegistry::addType(UIWorkspace::staticTypeInformation());
    STypeRegistry::addType(UILayout::staticTypeInformation());
    STypeRegistry::addType(UIStackedLayout::staticTypeInformation());
    STypeRegistry::addType(UIVerticalLayout::staticTypeInformation());
    STypeRegistry::addType(UIHorizontalLayout::staticTypeInformation());

    if(core()->isDebugUIEnabled())
      {
      addSurface(new UIDatabaseDebugSurface(&db));
      // UNUSED, QML. addSurface(new UIGraphDebugSurface(&db));
      addSurface(new UIEventLoggerSurface());

#ifdef X_PROFILING_ENABLED
      addSurface(new UIProfilerSurface);
#endif
      }

    }
  }

void UIPlugin::show()
  {
  _priv->restoreStateFromDB();
  _priv->show();
  }

void UIPlugin::unload()
  {
  foreach(UISurface *s, _priv->_surfaces)
    {
    s->privateData()->setPlugin(0);
    delete s;
    }

  delete _priv;
  }

void UIPlugin::addDock(QString name, QWidget *w)
  {
  UISurface *surf = new UISurface(name, w, UISurface::Dock);
  addSurface(surf);
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
      QDockWidget* dock = new QDockWidget(surface->name(), _priv);

      dock->setWidget(surface->widget());
      dock->setObjectName(surface->name());

      surface->privateData()->setDock(dock);
      _priv->addDockWidget(Qt::LeftDockWidgetArea, dock);
      }

    if(surface->type() == UISurface::PropertiesPage)
      {
      _priv->_preferenceTabs->addTab(surface->widget(), surface->name());
      }

    _priv->_surfaces << surface;
    }
  }

void UIPlugin::addQMLSurface(const QString &)
  {
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
