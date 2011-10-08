#include "GCBaseProperties.h"
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
