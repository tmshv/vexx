#include "QFormLayout"
#include "QWidget"
#include "spropertydefaultui.h"
#include "QString"
#include "sentityui.h"
#include "sentity.h"
#include "sproperty.h"

SEntityUI::SEntityUI(xuint32 options)
  {
  if((options&NoDefaultUIHandlers) != true)
    {
    setUIType(BoolProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::Bool>() );

    setUIType(IntProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::Int32>() );
    setUIType(UnsignedIntProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::UInt32>() );
    setUIType(LongIntProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::Int64>() );
    setUIType(LongUnsignedIntProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::UInt64>() );

    setUIType(FloatProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::Float>() );
    setUIType(DoubleProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::Double>() );

    setUIType(StringProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::String>() );

    setUIType(Vector2DProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::Vector2D>() );
    setUIType(Vector2DProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::Vector3D>() );
    setUIType(ColourProperty::staticTypeInformation(), addWidgetCreator<SPropertyDefaultUI::Colour>() );
    }
  }

xuint64 SEntityUI::widgetType(const SProperty *property) const
  {
  const SPropertyInformation *actualType(property->staticTypeInformation());
  if(_uiTypes.contains(actualType))
    {
    return _uiTypes[actualType];
    }
  return X_UINT64_SENTINEL;
  }

QWidget *SEntityUI::createControlWidget(SEntity *ent, QWidget *parent) const
  {
  QWidget *canvas(new QWidget(parent));
  QFormLayout *layout(new QFormLayout(canvas));

  SProperty *child = ent->firstChild();
  while(child)
    {
    QWidget *widget = createControlWidget(child);
    if(widget)
      {
      layout->addRow(child->name(), widget);
      }
    child = child->nextSibling();
    }
  return canvas;
  }

QWidget *SEntityUI::createControlWidget(SProperty *property, QWidget *parent) const
  {
  xuint64 type = widgetType(property);
  if(type < (xuint64)_types.size())
    {
    bool readOnly = false;
    // todo, make read only customisable..
    return _types[type](property, readOnly, parent);
    }
  return 0;
  }

bool SEntityUI::canCreateWidget(SProperty *prop) const
  {
  xuint64 type = widgetType(prop);
  return type < (xuint64)_types.size();
  }

int SEntityUI::addWidgetCreator(WidgetCreator fn)
  {
  _types << fn;
  return _types.size()-1;
  }

void SEntityUI::setUIType(const SPropertyInformation *type, int widgetType)
  {
  if(widgetType >= 0 && widgetType < _types.size())
    {
    _uiTypes[type] = widgetType;
    }
  }
