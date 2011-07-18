#include "3D/GCTransform.h"
#include "styperegistry.h"
#include "sdatabase.h"

template <typename Scalar, int Size, int Type> inline void writeValue(SSaver &s, const Eigen::Transform<Scalar, Size, Type> &t)
  {
  if(s.streamMode() == SSaver::Text)
    {
    s.textStream() << t.matrix();
    }
  else
    {
    s.binaryStream() << t.matrix();
    }
  }

template <typename Scalar, int Size, int Type> inline void readValue(SLoader &l, Eigen::Transform<Scalar, Size, Type> &t)
  {
  if(l.streamMode() == SLoader::Text)
    {
    l.textStream() >> t.matrix();
    }
  else
    {
    l.binaryStream() >> t.matrix();
    }
  }

IMPLEMENT_POD_PROPERTY(TransformProperty, XTransform);
IMPLEMENT_POD_PROPERTY(ComplexTransformProperty, XComplexTransform);

void TransformProperty::assignProperty(const SProperty *f, SProperty *t)
  {
  TransformProperty *to = t->uncheckedCastTo<TransformProperty>();

  const TransformProperty *sProp = f->castTo<TransformProperty>();
  if(sProp)
    {
    to->assign(sProp->value());
    return;
    }

  const ComplexTransformProperty *tProp = f->castTo<ComplexTransformProperty>();
  if(tProp)
    {
    to->assign(XTransform(tProp->value().matrix()));
    return;
    }
  }

void ComplexTransformProperty::assignProperty(const SProperty *f, SProperty *t)
  {
  ComplexTransformProperty *to = t->uncheckedCastTo<ComplexTransformProperty>();

  const ComplexTransformProperty *tProp = f->castTo<ComplexTransformProperty>();
  if(tProp)
    {
    to->assign(tProp->value());
    return;
    }

  const TransformProperty *sProp = f->castTo<TransformProperty>();
  if(sProp)
    {
    to->assign(XComplexTransform(sProp->value().matrix()));
    return;
    }

  }

S_IMPLEMENT_PROPERTY(GCTransform)

SPropertyInformation *GCTransform::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTransform>("GCTransform");

  TransformProperty::InstanceInformation* trInfo = info->add(&GCTransform::transform, "transform");
  trInfo->setDefault(XTransform::Identity());

  return info;
  }

GCTransform::GCTransform()
  {
  }
