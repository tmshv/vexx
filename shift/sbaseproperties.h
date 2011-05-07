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

#define DEFINE_POD_PROPERTY(EXPORT_MODE, name, type, defaultDefault) \
class EXPORT_MODE name : public SProperty \
  { \
public: \
  class InstanceInformation : public SProperty::InstanceInformation { public: \
    InstanceInformation() : _defaultValue(defaultDefault) { } \
    InstanceInformation(const SPropertyInformation *info, \
     const QString &name, xsize index, SProperty SPropertyContainer::* location, \
     ComputeFunction computeFn, SProperty SPropertyContainer::* *affects, bool entityChild, bool extra) \
      : SProperty::InstanceInformation(info, name, index, location, computeFn, affects, entityChild, extra), \
      _defaultValue(defaultDefault) { } \
    XProperties: XRORefProperty(type, defaultValue); \
    void initiateFromDefinition(const type &def) { SProperty::InstanceInformation::initiateFromDefinition(); _defaultValue = def; } \
    virtual void initiateProperty(SProperty *propertyToInitiate) const \
      { static_cast<name*>(propertyToInitiate)->_value = defaultValue(); } }; \
  S_PROPERTY(name, SProperty, savePOD, loadPOD, assignPOD, 0); \
  class Change : public SProperty::DataChange \
    { \
    S_CHANGE( Change, SChange, Type); \
  public: \
    Change(const type &b, const type &a, name *prop); \
    const type &before() const {return _before;} \
    const type &after() const {return _after;} \
  private: \
    type _before; \
    type _after; \
    bool apply(int mode, SObservers &obs); \
    }; \
  name(); \
  name(const type &def); \
  name &operator=(const type &in); \
  void assign(const type &in); \
const type &value() const {preGet(); return _value;} \
  const type &operator()() {preGet(); return _value;} \
protected: \
  type _value; \
private: \
  static void assignPOD(const SProperty *, SProperty * ); \
  static void savePOD(const SProperty *p, SPropertyData &data, SPropertyData::Mode mode); \
  static void loadPOD(SProperty *p, const SPropertyData &data, xuint32 v, SPropertyData::Mode mode, SLoader &l); \
  };

#define IMPLEMENT_POD_PROPERTY(name, type) \
name::Change::Change(const type &b, const type &a, name *prop) \
  : SProperty::DataChange(prop), _before(b), _after(a) \
  { } \
bool name::Change::apply(int mode, SObservers &obs) \
  { \
  if(mode&Forward) \
    { \
    ((name*)property())->_value = after(); \
    } \
  else if(mode&Backward) \
    { \
    ((name*)property())->_value = before(); \
    } \
  if(mode&Inform) \
    { \
    xAssert(property()->entity()); \
    obs.clear(); \
    property()->entity()->informDataObservers(mode, this, obs); \
    } \
  return true; \
  } \
name::name() \
  { \
  } \
name::name(const type &def) : _value(def) \
  { \
  } \
name &name::operator=(const type &in) \
  { \
  void *changeMemory = getChange< Change >(); \
  Change *change = new(changeMemory) Change(_value, in, this); \
  database()->submitChange(change); \
  postSet(); \
  return *this; \
  } \
void name::assign(const type &in) \
  { \
  void *changeMemory = getChange< Change >(); \
  Change *change = new(changeMemory) Change(_value, in, this); \
  database()->submitChange(change); \
  postSet(); \
  } \
void name::savePOD(const SProperty *p, SPropertyData &data, SPropertyData::Mode mode) { \
  SProperty::save(p, data, mode); \
  const name *ptr = p->castTo<const name>(); \
  xAssert(ptr); \
  if(ptr) { \
    QByteArray arr; \
    if(mode == SPropertyData::Binary) { \
      QDataStream str(&arr, QIODevice::WriteOnly); \
      str << ptr->_value; } \
    else { \
      QTextStream str(&arr, QIODevice::WriteOnly); \
      str << ptr->_value; } \
    data.setValue(arr); } } \
void name::loadPOD(SProperty *p, const SPropertyData &data, xuint32 v, SPropertyData::Mode mode, SLoader &l) { \
  SProperty::load(p, data, v, mode, l); \
  name *ptr = p->castTo<name>(); \
  xAssert(ptr); \
  if(ptr) { \
    QByteArray arr = data.value(); \
    if(mode == SPropertyData::Binary) { \
      QDataStream str(&arr, QIODevice::ReadOnly); \
      str >> ptr->_value; } \
    else { \
      QTextStream str(&arr, QIODevice::ReadOnly); \
      str >> ptr->_value; } } }

DEFINE_POD_PROPERTY(SHIFT_EXPORT, BoolProperty, int, 0);
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
DEFINE_POD_PROPERTY(SHIFT_EXPORT, StringProperty, XString, "");
DEFINE_POD_PROPERTY(SHIFT_EXPORT, ColourProperty, XColour, XColour(0.0f, 0.0f, 0.0f, 1.0f));
DEFINE_POD_PROPERTY(SHIFT_EXPORT, LongStringProperty, XString, "");

class SHIFT_EXPORT Pointer : public SProperty
  {
  S_PROPERTY(Pointer, SProperty, save, load, blankAssign, 0);

public:
  SProperty *pointed() const { preGet(); return input(); }
  SProperty *operator()() const { preGet(); return input(); }

  void setPointed(SProperty *prop);
  Pointer &operator=(SProperty *prop) { setPointed(prop); return *this; }

  static void assignPtr(const SProperty *, SProperty * );
  };

class SHIFT_EXPORT PointerArray : public STypedPropertyArray<Pointer>
  {
  S_PROPERTY_CONTAINER(PointerArray, STypedPropertyArray<Pointer>, 0);
public:
  void addPointer(SProperty *);
  void removePointer(SProperty *);
  bool hasPointer(SProperty *) const;
  };

#endif // SBASEPROPERTIES_H
