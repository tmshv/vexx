#include "GCQImage.h"
#include "sdatabase.h"
#include "styperegistry.h"

IMPLEMENT_POD_PROPERTY(GCQImage, QImage)


void GCQImage::assignProperty(const SProperty *f, SProperty *t)
  {
  SProfileFunction
  GCQImage *to = t->uncheckedCastTo<GCQImage>();

  const GCQImage *qImageProp = f->castTo<GCQImage>();
  if(qImageProp)
    {
    to->assign(qImageProp->value());
    }
  }
