#include "IneProperties.h"
#include "InePlugin.h"
#include "QWidget"
#include "QVBoxLayout"
#include "QListWidget"
#include "QPushButton"
#include "IneInterface.h"
#include "IneDriver.h"

IneProperties::IneProperties(InePlugin *plugin) : UISurface("DMX Device Manager", new QWidget, PropertiesPage), _plugin(plugin)
  {
  QVBoxLayout *layout(new QVBoxLayout(widget()));
  layout->setContentsMargins(2,2,2,2);

  _list = new QListWidget(widget());
  layout->addWidget(_list);

  QPushButton *refresh(new QPushButton("Refresh", widget()));
  layout->addWidget(refresh);
  connect(refresh, SIGNAL(clicked()), this, SLOT(refresh()));
  }

void IneProperties::refresh()
  {
  _plugin->refreshInterfaces();

  _list->clear();
  foreach(IneInterface *i, _plugin->interfaces())
    {
    _list->addItem(QString("%1 [%2]").arg(i->name()).arg(i->driver()->name()));
    }
  }
