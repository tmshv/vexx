#ifndef GCBASEPROPERTIES_H
#define GCBASEPROPERTIES_H

#include "GCGlobal.h"
#include "sbaseproperties.h"
#include "XTransform.h"

namespace
{
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
}

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, TransformProperty, XTransform, XTransform::Identity());
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, ComplexTransformProperty, XComplexTransform, XComplexTransform());

#endif // GCBASEPROPERTIES_H
