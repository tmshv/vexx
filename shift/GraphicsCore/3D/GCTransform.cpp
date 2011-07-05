#include "3D/GCTransform.h"
#include "styperegistry.h"
#include "sdatabase.h"

inline void writeValue(SSaver &s, const XTransform &t)
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

inline void readValue(SLoader &l, XTransform &t)
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

void TransformProperty::assignProperty(const SProperty *f, SProperty *t)
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
