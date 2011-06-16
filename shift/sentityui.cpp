#include "QFormLayout"
#include "QWidget"
#include "spropertydefaultui.h"
#include "XString"
#include "sentityui.h"
#include "sentity.h"
#include "sproperty.h"

SEntityUI::SEntityUI(xuint32 options)
  {
  if((options&NoDefaultUIHandlers) != true)
    {
    setUIType(BoolProperty::Type, addWidgetCreator<SPropertyDefaultUI::Bool>() );

    setUIType(IntProperty::Type, addWidgetCreator<SPropertyDefaultUI::Int32>() );
    setUIType(UnsignedIntProperty::Type, addWidgetCreator<SPropertyDefaultUI::UInt32>() );
    setUIType(LongIntProperty::Type, addWidgetCreator<SPropertyDefaultUI::Int64>() );
    setUIType(LongUnsignedIntProperty::Type, addWidgetCreator<SPropertyDefaultUI::UInt64>() );

    setUIType(FloatProperty::Type, addWidgetCreator<SPropertyDefaultUI::Float>() );
    setUIType(DoubleProperty::Type, addWidgetCreator<SPropertyDefaultUI::Double>() );

    setUIType(StringProperty::Type, addWidgetCreator<SPropertyDefaultUI::String>() );

    setUIType(Vector2DProperty::Type, addWidgetCreator<SPropertyDefaultUI::Vector2D>() );
    setUIType(Vector2DProperty::Type, addWidgetCreator<SPropertyDefaultUI::Vector3D>() );
    setUIType(ColourProperty::Type, addWidgetCreator<SPropertyDefaultUI::Colour>() );
    }
  }

xuint64 SEntityUI::widgetType(const SProperty *property) const
  {
  xuint32 actualType(property->type());
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

void SEntityUI::setUIType(XMetaType::Type type, int widgetType)
  {
  if(widgetType >= 0 && widgetType < _types.size())
    {
    _uiTypes[type] = widgetType;
    }
  }
