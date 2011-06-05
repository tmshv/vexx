#include "sbaseproperties.h"
#include "sdatabase.h"

IMPLEMENT_POD_PROPERTY(BoolProperty, int);
IMPLEMENT_POD_PROPERTY(IntProperty, xint32);
IMPLEMENT_POD_PROPERTY(LongIntProperty, xint64);
IMPLEMENT_POD_PROPERTY(UnsignedIntProperty, xuint32);
IMPLEMENT_POD_PROPERTY(LongUnsignedIntProperty, xuint64);
IMPLEMENT_POD_PROPERTY(FloatProperty, float);
IMPLEMENT_POD_PROPERTY(DoubleProperty, double);
IMPLEMENT_POD_PROPERTY(Vector2DProperty, XVector2D);
IMPLEMENT_POD_PROPERTY(Vector3DProperty, XVector3D);
IMPLEMENT_POD_PROPERTY(Vector4DProperty, XVector4D);
IMPLEMENT_POD_PROPERTY(QuaternionProperty, XQuaternion);
IMPLEMENT_POD_PROPERTY(StringProperty, XString);
IMPLEMENT_POD_PROPERTY(ColourProperty, XColour);
IMPLEMENT_POD_PROPERTY(LongStringProperty, XString);
IMPLEMENT_POD_PROPERTY(ByteArrayProperty, QByteArray);

void BoolProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  BoolProperty *to = t->uncheckedCastTo<BoolProperty>();

  const BoolProperty *boolProp = f->castTo<BoolProperty>();
  if(boolProp)
    {
    to->assign(boolProp->value());
    return;
    }

  const IntProperty *intProp = f->castTo<IntProperty>();
  if(intProp)
    {
    to->assign(intProp->value());
    return;
    }

  const LongIntProperty *longIntProp = f->castTo<LongIntProperty>();
  if(longIntProp)
    {
    to->assign(longIntProp->value());
    return;
    }

  const UnsignedIntProperty *uIntProp = f->castTo<UnsignedIntProperty>();
  if(uIntProp)
    {
    to->assign(uIntProp->value());
    return;
    }

  const LongUnsignedIntProperty *uLongIntProp = f->castTo<LongUnsignedIntProperty>();
  if(uLongIntProp)
    {
    to->assign(uLongIntProp->value());
    return;
    }

  const FloatProperty *floatProp = f->castTo<FloatProperty>();
  if(floatProp)
    {
    to->assign(floatProp->value());
    return;
    }

  const DoubleProperty *doubleProp = f->castTo<DoubleProperty>();
  if(doubleProp)
    {
    to->assign(doubleProp->value());
    return;
    }
  }

void IntProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  IntProperty *to = t->uncheckedCastTo<IntProperty>();

  const BoolProperty *boolProp = f->castTo<BoolProperty>();
  if(boolProp)
    {
    to->assign(boolProp->value());
    return;
    }

  const IntProperty *intProp = f->castTo<IntProperty>();
  if(intProp)
    {
    to->assign(intProp->value());
    return;
    }

  const LongIntProperty *longIntProp = f->castTo<LongIntProperty>();
  if(longIntProp)
    {
    to->assign(longIntProp->value());
    return;
    }

  const UnsignedIntProperty *uIntProp = f->castTo<UnsignedIntProperty>();
  if(uIntProp)
    {
    to->assign(uIntProp->value());
    return;
    }

  const LongUnsignedIntProperty *uLongIntProp = f->castTo<LongUnsignedIntProperty>();
  if(uLongIntProp)
    {
    to->assign(uLongIntProp->value());
    return;
    }

  const FloatProperty *floatProp = f->castTo<FloatProperty>();
  if(floatProp)
    {
    to->assign(floatProp->value());
    return;
    }

  const DoubleProperty *doubleProp = f->castTo<DoubleProperty>();
  if(doubleProp)
    {
    to->assign(doubleProp->value());
    return;
    }
  }

void LongIntProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  LongIntProperty *to = t->uncheckedCastTo<LongIntProperty>();

  const BoolProperty *boolProp = f->castTo<BoolProperty>();
  if(boolProp)
    {
    to->assign(boolProp->value());
    return;
    }

  const IntProperty *intProp = f->castTo<IntProperty>();
  if(intProp)
    {
    to->assign(intProp->value());
    return;
    }

  const LongIntProperty *longIntProp = f->castTo<LongIntProperty>();
  if(longIntProp)
    {
    to->assign(longIntProp->value());
    return;
    }

  const UnsignedIntProperty *uIntProp = f->castTo<UnsignedIntProperty>();
  if(uIntProp)
    {
    to->assign(uIntProp->value());
    return;
    }

  const LongUnsignedIntProperty *uLongIntProp = f->castTo<LongUnsignedIntProperty>();
  if(uLongIntProp)
    {
    to->assign(uLongIntProp->value());
    return;
    }

  const FloatProperty *floatProp = f->castTo<FloatProperty>();
  if(floatProp)
    {
    to->assign(floatProp->value());
    return;
    }

  const DoubleProperty *doubleProp = f->castTo<DoubleProperty>();
  if(doubleProp)
    {
    to->assign(doubleProp->value());
    return;
    }
  }

void UnsignedIntProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  UnsignedIntProperty *to = t->uncheckedCastTo<UnsignedIntProperty>();

  const BoolProperty *boolProp = f->castTo<BoolProperty>();
  if(boolProp)
    {
    to->assign(boolProp->value());
    return;
    }

  const IntProperty *intProp = f->castTo<IntProperty>();
  if(intProp)
    {
    to->assign(intProp->value());
    return;
    }

  const LongIntProperty *longIntProp = f->castTo<LongIntProperty>();
  if(longIntProp)
    {
    to->assign(longIntProp->value());
    return;
    }

  const UnsignedIntProperty *uIntProp = f->castTo<UnsignedIntProperty>();
  if(uIntProp)
    {
    to->assign(uIntProp->value());
    return;
    }

  const LongUnsignedIntProperty *uLongIntProp = f->castTo<LongUnsignedIntProperty>();
  if(uLongIntProp)
    {
    to->assign(uLongIntProp->value());
    return;
    }

  const FloatProperty *floatProp = f->castTo<FloatProperty>();
  if(floatProp)
    {
    to->assign(floatProp->value());
    return;
    }

  const DoubleProperty *doubleProp = f->castTo<DoubleProperty>();
  if(doubleProp)
    {
    to->assign(doubleProp->value());
    return;
    }
  }

void LongUnsignedIntProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  LongUnsignedIntProperty *to = t->uncheckedCastTo<LongUnsignedIntProperty>();

  const BoolProperty *boolProp = f->castTo<BoolProperty>();
  if(boolProp)
    {
    to->assign(boolProp->value());
    return;
    }

  const IntProperty *intProp = f->castTo<IntProperty>();
  if(intProp)
    {
    to->assign(intProp->value());
    return;
    }

  const LongIntProperty *longIntProp = f->castTo<LongIntProperty>();
  if(longIntProp)
    {
    to->assign(longIntProp->value());
    return;
    }

  const UnsignedIntProperty *uIntProp = f->castTo<UnsignedIntProperty>();
  if(uIntProp)
    {
    to->assign(uIntProp->value());
    return;
    }

  const LongUnsignedIntProperty *uLongIntProp = f->castTo<LongUnsignedIntProperty>();
  if(uLongIntProp)
    {
    to->assign(uLongIntProp->value());
    return;
    }

  const FloatProperty *floatProp = f->castTo<FloatProperty>();
  if(floatProp)
    {
    to->assign(floatProp->value());
    return;
    }

  const DoubleProperty *doubleProp = f->castTo<DoubleProperty>();
  if(doubleProp)
    {
    to->assign(doubleProp->value());
    return;
    }
  }

void FloatProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  FloatProperty *to = t->uncheckedCastTo<FloatProperty>();

  const BoolProperty *boolProp = f->castTo<BoolProperty>();
  if(boolProp)
    {
    to->assign(boolProp->value());
    return;
    }

  const IntProperty *intProp = f->castTo<IntProperty>();
  if(intProp)
    {
    to->assign(intProp->value());
    return;
    }

  const LongIntProperty *longIntProp = f->castTo<LongIntProperty>();
  if(longIntProp)
    {
    to->assign(longIntProp->value());
    return;
    }

  const UnsignedIntProperty *uIntProp = f->castTo<UnsignedIntProperty>();
  if(uIntProp)
    {
    to->assign(uIntProp->value());
    return;
    }

  const LongUnsignedIntProperty *uLongIntProp = f->castTo<LongUnsignedIntProperty>();
  if(uLongIntProp)
    {
    to->assign(uLongIntProp->value());
    return;
    }

  const FloatProperty *floatProp = f->castTo<FloatProperty>();
  if(floatProp)
    {
    to->assign(floatProp->value());
    return;
    }

  const DoubleProperty *doubleProp = f->castTo<DoubleProperty>();
  if(doubleProp)
    {
    to->assign(doubleProp->value());
    return;
    }
  }

void DoubleProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  DoubleProperty *to = t->uncheckedCastTo<DoubleProperty>();

  const BoolProperty *boolProp = f->castTo<BoolProperty>();
  if(boolProp)
    {
    to->assign(boolProp->value());
    return;
    }

  const IntProperty *intProp = f->castTo<IntProperty>();
  if(intProp)
    {
    to->assign(intProp->value());
    return;
    }

  const LongIntProperty *longIntProp = f->castTo<LongIntProperty>();
  if(longIntProp)
    {
    to->assign(longIntProp->value());
    return;
    }

  const UnsignedIntProperty *uIntProp = f->castTo<UnsignedIntProperty>();
  if(uIntProp)
    {
    to->assign(uIntProp->value());
    return;
    }

  const LongUnsignedIntProperty *uLongIntProp = f->castTo<LongUnsignedIntProperty>();
  if(uLongIntProp)
    {
    to->assign(uLongIntProp->value());
    return;
    }

  const FloatProperty *floatProp = f->castTo<FloatProperty>();
  if(floatProp)
    {
    to->assign(floatProp->value());
    return;
    }

  const DoubleProperty *doubleProp = f->castTo<DoubleProperty>();
  if(doubleProp)
    {
    to->assign(doubleProp->value());
    return;
    }
  }

void Vector2DProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  Vector2DProperty *to = t->uncheckedCastTo<Vector2DProperty>();

  const Vector2DProperty *aProp = f->castTo<Vector2DProperty>();
  if(aProp)
    {
    to->assign(aProp->value());
    return;
    }

  const Vector3DProperty *bProp = f->castTo<Vector3DProperty>();
  if(bProp)
    {
    to->assign(bProp->value().head<2>());
    return;
    }

  const Vector4DProperty *cProp = f->castTo<Vector4DProperty>();
  if(cProp)
    {
    to->assign(cProp->value().head<2>());
    return;
    }

  const ColourProperty *colProp = f->castTo<ColourProperty>();
  if(colProp)
    {
    to->assign(colProp->value().head<2>());
    return;
    }

  const QuaternionProperty *qProp = f->castTo<QuaternionProperty>();
  if(qProp)
    {
    to->assign(qProp->value().coeffs().head<2>());
    return;
    }
  }

void Vector3DProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  Vector3DProperty *to = t->uncheckedCastTo<Vector3DProperty>();

  const Vector2DProperty *aProp = f->castTo<Vector2DProperty>();
  if(aProp)
    {
    to->assign(aProp->value().head<3>());
    return;
    }

  const Vector3DProperty *bProp = f->castTo<Vector3DProperty>();
  if(bProp)
    {
    to->assign(bProp->value());
    return;
    }

  const Vector4DProperty *cProp = f->castTo<Vector4DProperty>();
  if(cProp)
    {
    to->assign(cProp->value().head<3>());
    return;
    }

  const ColourProperty *colProp = f->castTo<ColourProperty>();
  if(colProp)
    {
    to->assign(colProp->value().head<3>());
    return;
    }

  const QuaternionProperty *qProp = f->castTo<QuaternionProperty>();
  if(qProp)
    {
    to->assign(qProp->value().coeffs().head<3>());
    return;
    }
  }

void Vector4DProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  ColourProperty *to = t->uncheckedCastTo<ColourProperty>();

  const Vector2DProperty *aProp = f->castTo<Vector2DProperty>();
  if(aProp)
    {
    const XColour &col = aProp->value().head<4>();
    to->assign(col);
    return;
    }

  const Vector3DProperty *bProp = f->castTo<Vector3DProperty>();
  if(bProp)
    {
    to->assign(bProp->value().head<4>());
    return;
    }

  const Vector4DProperty *cProp = f->castTo<Vector4DProperty>();
  if(cProp)
    {
    to->assign(cProp->value());
    return;
    }

  const ColourProperty *colProp = f->castTo<ColourProperty>();
  if(colProp)
    {
    to->assign(colProp->value());
    return;
    }

  const QuaternionProperty *qProp = f->castTo<QuaternionProperty>();
  if(qProp)
    {
    to->assign(qProp->value().coeffs());
    return;
    }
  }

void QuaternionProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  ColourProperty *to = t->uncheckedCastTo<ColourProperty>();

  const Vector2DProperty *aProp = f->castTo<Vector2DProperty>();
  if(aProp)
    {
    to->assign(aProp->value().head<4>());
    return;
    }

  const Vector3DProperty *bProp = f->castTo<Vector3DProperty>();
  if(bProp)
    {
    to->assign(bProp->value().head<4>());
    return;
    }

  const Vector4DProperty *cProp = f->castTo<Vector4DProperty>();
  if(cProp)
    {
    to->assign(cProp->value());
    return;
    }

  const ColourProperty *colProp = f->castTo<ColourProperty>();
  if(colProp)
    {
    to->assign(colProp->value());
    return;
    }

  const QuaternionProperty *qProp = f->castTo<QuaternionProperty>();
  if(qProp)
    {
    to->assign(qProp->value().coeffs().head<4>());
    return;
    }
  }

void ColourProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  ColourProperty *to = t->uncheckedCastTo<ColourProperty>();

  const Vector2DProperty *aProp = f->castTo<Vector2DProperty>();
  if(aProp)
    {
    to->assign(aProp->value().head<4>());
    return;
    }

  const Vector3DProperty *bProp = f->castTo<Vector3DProperty>();
  if(bProp)
    {
    to->assign(bProp->value().head<4>());
    return;
    }

  const Vector4DProperty *cProp = f->castTo<Vector4DProperty>();
  if(cProp)
    {
    to->assign(cProp->value());
    return;
    }

  const ColourProperty *colProp = f->castTo<ColourProperty>();
  if(colProp)
    {
    to->assign(colProp->value());
    return;
    }

  const QuaternionProperty *qProp = f->castTo<QuaternionProperty>();
  if(qProp)
    {
    to->assign(qProp->value().coeffs().head<4>());
    return;
    }
  }

void StringProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  StringProperty *to = t->uncheckedCastTo<StringProperty>();

  const StringProperty *sProp = f->castTo<StringProperty>();
  if(sProp)
    {
    to->assign(sProp->value());
    return;
    }

  const LongStringProperty *lsProp = f->castTo<LongStringProperty>();
  if(lsProp)
    {
    to->assign(lsProp->value());
    return;
    }
  }

void LongStringProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  LongStringProperty *to = t->uncheckedCastTo<LongStringProperty>();

  const LongStringProperty *lsProp = f->castTo<LongStringProperty>();
  if(lsProp)
    {
    to->assign(lsProp->value());
    return;
    }

  const StringProperty *sProp = f->castTo<StringProperty>();
  if(sProp)
    {
    to->assign(sProp->value());
    return;
    }
  }

void ByteArrayProperty::assignPOD(const SProperty *f, SProperty *t)
  {
  }

void PointerArray::addPointer(SProperty *prop)
  {
  SDatabase* db = database();
  xAssert(db);

  SBlock b(db);
  Pointer *p = add();
  p->setPointed(prop);
  }

void PointerArray::removePointer(SProperty *p)
  {
  SProperty *c = firstChild();
  while(c)
    {
    Pointer *cT = c->castTo<Pointer>();
    if(cT)
      {
      if(cT->pointed() == p)
        {
        remove(c);
        return;
        }
      }
    c = c->nextSibling();
    }
  }

bool PointerArray::hasPointer(SProperty *prop) const
  {
  SProperty *child = firstChild();
  while(child)
    {
    if(prop == child->input())
      {
      return true;
      }
    child = child->nextSibling();
    }
  return false;
  }

S_PROPERTY_CONTAINER_EMPTY_DEFINITION(PointerArray, STypedPropertyArray<Pointer>, saveContainer, loadContainer)

void Pointer::setPointed(SProperty *prop)
  {
  if(hasInput())
    {
    input()->disconnect(this);
    }

  if(prop)
    {
    prop->connect(this);
    }
  postSet();
  }
