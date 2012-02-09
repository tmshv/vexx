#include "scnodeitem.h"
#include "sentity.h"
#include "sinterfaces.h"
#include "UI/sentityui.h"
#include "QGraphicsProxyWidget"

ScNodeItem::ScNodeItem(QDeclarativeItem *parent) :
  ScPropertyItem(parent)
  {
  }

const QString &ScNodeItem::name() const
  {
  if(!_specialMode.isEmpty())
    {
    if(_specialMode == "input")
      {
      static QString inputs = "Inputs";
      return inputs;
      }
    if(_specialMode == "output")
      {
      static QString outputs = "Outputs";
      return outputs;
      }
    }
  return ScPropertyItem::name();
  }

void ScNodeItem::setName(const QString& x)
  {
  if(!_specialMode.isEmpty())
    {
    xAssertFail();
    return;
    }

  ScPropertyItem::setName(x);
  }

void ScNodeItem::setEntity(SEntity *ent, const QString &spMode)
  {
  setProperty(ent);
  _specialMode = spMode;

  const SPropertyPositionInterface *interface = property()->interface<SPropertyPositionInterface>();
  xAssert(interface);
  if(interface)
    {
    if(_specialMode.isEmpty())
      {
      XVector3D pos = interface->position(property());
      setPos(pos.x(), pos.y());
      }
    else if(_specialMode == "input")
      {
      XVector3D pos = interface->inputsPosition(property());
      setPos(pos.x(), pos.y());
      }
    else if(_specialMode == "output")
      {
      XVector3D pos = interface->outputsPosition(property());
      setPos(pos.x(), pos.y());
      }
    }
  }

void ScNodeItem::setPosition(float x, float y)
  {
  setPos(x, y);
  positionChanged();
  }

QVariantMap ScNodeItem::createPropertiesSection(QDeclarativeItem *parent, QColor labelColour, float expectedWidth, float expectedHeight)
  {
  SEntityUI ui;
  QWidget *widg = ui.createControlWidget(property()->entity(), 0);

  QPalette pal = widg->palette();
  pal.setColor(QPalette::Background, Qt::transparent);
  pal.setColor(QPalette::WindowText, labelColour);
  widg->setPalette(pal);

  QRect geo = widg->geometry();
  if(expectedWidth > 0)
    {
    geo.setWidth(expectedWidth);
    }
  if(expectedHeight > 0)
    {
    geo.setHeight(expectedHeight);
    }
  widg->setGeometry(geo);

  QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(parent);
  proxy->setWidget(widg);

  QSizeF s = proxy->size();

  QVariantMap r;
  r["width"] = s.width();
  r["height"] = s.height();
  return r;
  }

void ScNodeItem::positionChanged()
  {
  const SPropertyPositionInterface *interface = property()->interface<SPropertyPositionInterface>();
  xAssert(interface);
  if(interface)
    {
    if(_specialMode.isEmpty())
      {
      interface->setPosition(property(), XVector3D(x(), y(), 0.0f));
      }
    else if(_specialMode == "input")
      {
      interface->setInputsPosition(property(), XVector3D(x(), y(), 0.0f));
      }
    else if(_specialMode == "output")
      {
      interface->setOutputsPosition(property(), XVector3D(x(), y(), 0.0f));
      }
    }
  }

QString ScNodeItem::specialMode() const
  {
  return _specialMode;
  }
