#ifndef SBASEPROPERTIES_H
#define SBASEPROPERTIES_H

#include "XVector2D"
#include "XVector3D"
#include "XVector4D"
#include "XColour"
#include "XQuaternion"
#include "sproperty.h"
#include "sentity.h"
#include "schange.h"
#include "sobserver.h"
#include "sinterface.h"
#include "QByteArray"

template <typename T>
class SPODInterface
  {
  };

class SPropertyVariantInterface : public SStaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(SPropertyVariantInterface, PropertyVariantInterface);

public:
  SPropertyVariantInterface(bool d) : SStaticInterfaceBase(d) { }
  virtual QVariant asVariant(const SProperty *) const = 0;
  virtual void setVariant(SProperty *, const QVariant &) const = 0;
  };

template <typename PROP, typename POD> class PODPropertyVariantInterface : public SPropertyVariantInterface
  {
public:
  PODPropertyVariantInterface() : SPropertyVariantInterface(true) { }
  virtual QVariant asVariant(const SProperty *p) const
    {
    return QVariant::fromValue<POD>(p->uncheckedCastTo<PROP>()->value());
    }

  virtual void setVariant(SProperty *p, const QVariant &v) const
    {
    p->uncheckedCastTo<PROP>()->assign(v.value<POD>());
    }
  };

template <typename T, typename DERIVED> class SPODProperty : public SProperty
  {
XProperties:
  XRORefProperty(T, value);

public:
  class InstanceInformation : public SProperty::InstanceInformation
    {
  XProperties:
    XByRefProperty(T, defaultValue, setDefault);

  public:
    InstanceInformation(const T& d) : _defaultValue(d)
      {
      }

    virtual void initiateProperty(SProperty *propertyToInitiate) const
      {
      propertyToInitiate->uncheckedCastTo<DERIVED>()->_value = defaultValue();
      }
    };

  const T &operator()() const
    {
    return _value;
    }

  void assign(const T &in);

  static void saveProperty(const SProperty *p, SSaver &l )
    {
    SProperty::saveProperty(p, l);
    const DERIVED *ptr = p->uncheckedCastTo<DERIVED>();
    writeValue(l, ptr->_value);
    }

  static SProperty *loadProperty(SPropertyContainer *parent, SLoader &l)
    {
    SProperty *prop = SProperty::loadProperty(parent, l);
    DERIVED *ptr = prop->uncheckedCastTo<DERIVED>();
    readValue(l, ptr->_value);
    return prop;
    }

  static bool shouldSavePropertyValue(const SProperty *p)
    {
    const DERIVED *ptr = p->uncheckedCastTo<DERIVED>();

    bool def = ptr->value() == ptr->instanceInformation()->defaultValue();

    return !def && SProperty::shouldSavePropertyValue(p);
    }

private:
  class Change : public SProperty::DataChange
    {
    S_CHANGE(Change, SChange, Type);

  XProperties:
    XRORefProperty(T, before);
    XRORefProperty(T, after);

  public:
    Change(const T &b, const T &a, SPODProperty<T, DERIVED> *prop)
      : SProperty::DataChange(prop), _before(b), _after(a)
      { }

  private:
    bool apply(int mode)
      {
      if(mode&Forward)
        {
        property()->uncheckedCastTo<DERIVED>()->_value = after();
        property()->postSet();
        }
      else if(mode&Backward)
        {
        property()->uncheckedCastTo<DERIVED>()->_value = before();
        property()->postSet();
        }
      if(mode&Inform)
        {
        if(property()->entity())
          {
          property()->entity()->informDirtyObservers(property());
          }
        }
      return true;
      }
    };
  };

#define DEFINE_POD_PROPERTY(EXPORT_MODE, name, type, defaultDefault) \
class EXPORT_MODE name : public SPODProperty<type, name> { public: \
  S_PROPERTY(name, SProperty, 0); \
public: class InstanceInformation : public SPODProperty<type, name>::InstanceInformation \
    { public: \
    InstanceInformation() : SPODProperty<type, name>::InstanceInformation(defaultDefault) { } }; \
  name(); \
  name &operator=(const type &in) { \
    assign(in); \
    return *this; } \
  static void assignProperty(const SProperty *p, SProperty *l ); }; \
template <> class SPODInterface <type> { public: typedef name Type; \
  static void assign(name* s, const type& val) { s->assign(val); } \
  static const type& value(const name* s) { return s->value(); } };

#define IMPLEMENT_POD_PROPERTY(name, type) \
  S_IMPLEMENT_PROPERTY(name) \
  SPropertyInformation *name::createTypeInformation() { \
    SPropertyInformation *info = SPropertyInformation::create<name>(#name); \
    info->addStaticInterface(new PODPropertyVariantInterface<name, type>()); \
    return info; } \
  name::name() { }

namespace
{
QTextStream &operator<<(QTextStream &s, xuint8 v)
  {
  return s << (xuint32)v;
  }

QTextStream &operator>>(QTextStream &s, xuint8 &v)
  {
  xuint32 t;
  s >> t;
  v = (xuint8)t;
  return s;
  }
}

DEFINE_POD_PROPERTY(SHIFT_EXPORT, BoolProperty, xuint8, 0);
DEFINE_POD_PROPERTY(SHIFT_EXPORT, IntProperty, xint32, 0);
DEFINE_POD_PROPERTY(SHIFT_EXPORT, LongIntProperty, xint64, 0);
DEFINE_POD_PROPERTY(SHIFT_EXPORT, UnsignedIntProperty, xuint32, 0);
DEFINE_POD_PROPERTY(SHIFT_EXPORT, LongUnsignedIntProperty, xuint64, 0);
DEFINE_POD_PROPERTY(SHIFT_EXPORT, FloatProperty, float, 0.0f);
DEFINE_POD_PROPERTY(SHIFT_EXPORT, DoubleProperty, double, 0.0);
DEFINE_POD_PROPERTY(SHIFT_EXPORT, Vector2DProperty, XVector2D, XVector2D(0.0f, 0.0f));
DEFINE_POD_PROPERTY(SHIFT_EXPORT, Vector3DProperty, XVector3D, XVector3D(0.0f, 0.0f, 0.0f));
DEFINE_POD_PROPERTY(SHIFT_EXPORT, Vector4DProperty, XVector4D, XVector4D(0.0f, 0.0f, 0.0f, 0.0f));
DEFINE_POD_PROPERTY(SHIFT_EXPORT, QuaternionProperty, XQuaternion, XQuaternion());
DEFINE_POD_PROPERTY(SHIFT_EXPORT, StringProperty, QString, "");
DEFINE_POD_PROPERTY(SHIFT_EXPORT, ColourProperty, XColour, XColour(0.0f, 0.0f, 0.0f, 1.0f));
DEFINE_POD_PROPERTY(SHIFT_EXPORT, ByteArrayProperty, QByteArray, QByteArray());

#define EnumProperty IntProperty

template <typename Derived> QTextStream & operator <<(QTextStream &str, const Eigen::PlainObjectBase <Derived> &data)
  {
  xsize width = data.cols();
  xsize height = data.rows();
  str << width << " " << height << " ";
  for (xsize i = 0; i < height; ++i)
    {
    for(xsize j = 0; j < width; ++j)
      {
      str << data(i, j);
      if((i < height-1) && (j < width-1)) // while not last element
        {
        str << " "; // separate each element with space
        }
      }
    }
  return str;
  }

template <typename Derived> QDataStream & operator <<(QDataStream &str, const Eigen::PlainObjectBase <Derived> &data)
  {
  xsize width = data.cols();
  xsize height = data.rows();
  str << (quint64) width << (quint64) height;
  for (xsize i = 0; i < height; ++i)
    {
    for(xsize j = 0; j < width; ++j)
      {
      str << data(i, j);
      }
    }
  return str;
  }

template <typename Derived> QTextStream & operator >>(QTextStream &str, Eigen::PlainObjectBase <Derived> &data)
  {
  xsize width;
  xsize height;

  str >> width >> height; // first element in str is size of str
  data.resize(width, height);

  for(xsize i = 0; i < height; ++i )
    {
    for(xsize j = 0; j < width; j++)
      {
      typename Derived::Scalar tVal;
      str >> tVal;
      data(i, j) = tVal;
      }
    }
  return str;
  }

template <typename Derived> QDataStream & operator >>(QDataStream &str, Eigen::PlainObjectBase <Derived> &data)
  {
  quint64 width;
  quint64 height;

  str >> width >> height; // first element in str is size of str
  data.resize(width, height);
  for(xsize i = 0; i < height; ++i )
    {
    for(xsize j = 0; j < width; j++)
      {
      typename Derived::Scalar tVal;
      str >> tVal;
      data(i, j) = tVal;
      }
    }
  return str;
  }

// specific pod interface for bool because it is actually a uint8.
template <> class SPODInterface <bool> { public: typedef BoolProperty Type; \
  static void assign(BoolProperty* s, const bool &val) { s->assign(val); } \
  static const xuint8 &value(const BoolProperty* s) { return s->value(); } };



#include "sdatabase.h"

template <typename T, typename DERIVED> void SPODProperty<T, DERIVED>::assign(const T &in)
  {
  database()->doChange<Change>(_value, in, this);
  }

#endif // SBASEPROPERTIES_H
