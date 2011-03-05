#include "IneFixture.h"

IneFixture::IneFixture()
  {
  _manufacturer = "None";
  }

SPropertyID IneFixture::addParameter(QString n)
  {
  Parameter param;
  param.name = n;
  param.value = 0.0f;
  _params << param;
  return LastProperty + _params.size() - 1;
  }

void IneFixture::removeParameter(QString n)
  {
  for(int i=0, s=_params.size(); i<s; ++i)
    {
    if(_params[i].name == n)
      {
      _params.removeAt(i);
      --i;
      --s;
      }
    }
  }

xuint32 IneFixture::numberOfProperties() const
  {
  return LastProperty + _params.size();
  }

xuint32 IneFixture::numberOfAttributes(SPropertyID p) const
  {
  if(p < SEntity::LastProperty)
    {
    return SEntity::numberOfAttributes(p);
    }
  if(p == Manufacturer)
    {
    return 2;
    }
  return LastAttribute;
  }

QString IneFixture::manufacturer() const
  {
  return _manufacturer;
  }

void IneFixture::serialise(IneFixtureInstance *instance, QByteArray &data)
  {
  }

XVariant IneFixture::getData( SPropertyID p, SAttributeID a ) const
  {
  if(p < SEntity::LastProperty)
    {
    return SEntity::getData( p, a );
    }

  if(p == Manufacturer)
    {
    if(a == SProperty::Name)
      {
      return "Manufacturer";
      }
    else if(a == SProperty::Value)
      {
      return _manufacturer;
      }
    }
  else if(p < (xuint32)LastProperty+_params.size())
    {
    const Parameter &param(_params[p-LastProperty]);
    if(a == SProperty::Name)
      {
      return param.name;
      }
    else if(a == SProperty::Value)
      {
      return param.value;
      }
    else if(a == Format)
      {
      return param.format;
      }
    else if(a == Offset)
      {
      return param.offset;
      }
    }

  return XVariant();
  }

bool IneFixture::setData( SPropertyID p, SAttributeID a, const XVariant &value )
  {
  if(p < SEntity::LastProperty)
    {
    return SEntity::setData( p, a, value );
    }

  if(p == Manufacturer)
    {
    if(a == SProperty::Value)
      {
      _manufacturer = value.toString();
      return true;
      }
    }
  else if(p < (xuint32)LastProperty+_params.size())
    {
    Parameter &param(_params[p-LastProperty]);
    if(a == SProperty::Name)
      {
      param.name = value.toString();
      return true;
      }
    else if(a == SProperty::Value)
      {
      param.value = value;
      return true;
      }
    else if(a == Format)
      {
      param.format = value.toUnsignedInt();
      return true;
      }
    else if(a == Offset)
      {
      param.offset = value.toUnsignedInt();
      return true;
      }
    }

  return false;
  }
