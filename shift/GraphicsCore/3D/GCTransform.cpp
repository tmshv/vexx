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
  }

void ComplexTransformProperty::assignProperty(const SProperty *f, SProperty *t)
  {
  }

S_IMPLEMENT_PROPERTY(GCTransform)

SPropertyInformation *GCTransform::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTransform>("GCTransform");

  info->add(&GCTransform::transform, "transform");

  return info;
  }

GCTransform::GCTransform()
  {
  }
