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

#define DEFINE_POD_PROPERTY(name, type) \
class SHIFT_EXPORT name : public SProperty \
  { \
  S_PROPERTY(name, SProperty, savePOD, loadPOD, assignPOD); \
public: \
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

DEFINE_POD_PROPERTY(BoolProperty, int);
DEFINE_POD_PROPERTY(IntProperty, xint32);
DEFINE_POD_PROPERTY(LongIntProperty, xint64);
DEFINE_POD_PROPERTY(UnsignedIntProperty, xuint32);
DEFINE_POD_PROPERTY(LongUnsignedIntProperty, xuint64);
DEFINE_POD_PROPERTY(FloatProperty, float);
DEFINE_POD_PROPERTY(DoubleProperty, double);
DEFINE_POD_PROPERTY(Vector2DProperty, XVector2D);
DEFINE_POD_PROPERTY(Vector3DProperty, XVector3D);
DEFINE_POD_PROPERTY(Vector4DProperty, XVector4D);
DEFINE_POD_PROPERTY(QuaternionProperty, XQuaternion);
DEFINE_POD_PROPERTY(StringProperty, XString);
DEFINE_POD_PROPERTY(ColourProperty, XColour);
DEFINE_POD_PROPERTY(LongStringProperty, XString);

class SHIFT_EXPORT Pointer : public SProperty
  {
  S_PROPERTY(Pointer, SProperty, save, load, blankAssign);

public:
  SProperty *pointed() const { preGet(); return input(); }
  SProperty *operator()() const { preGet(); return input(); }

  void setPointed(SProperty *prop);
  Pointer &operator=(SProperty *prop) { setPointed(prop); return *this; }

  static void assignPtr(const SProperty *, SProperty * );
  };

class SHIFT_EXPORT PointerArray : public STypedPropertyArray<Pointer>
  {
  S_PROPERTY_CONTAINER(PointerArray, STypedPropertyArray<Pointer>);
public:
  void addPointer(SProperty *);
  void removePointer(SProperty *);
  bool hasPointer(SProperty *) const;
  };

#endif // SBASEPROPERTIES_H
