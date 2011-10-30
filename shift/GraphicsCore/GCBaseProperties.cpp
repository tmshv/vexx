#include "GCBaseProperties.h"
#include "sdatabase.h"

void writeValue(SSaver &s, const XShader &t)
  {
  xAssertFail();
  }

void readValue(SLoader &l, XShader &t)
  {
  xAssertFail();
  }

IMPLEMENT_POD_PROPERTY(TransformProperty, XTransform);
IMPLEMENT_POD_PROPERTY(ComplexTransformProperty, XComplexTransform);
IMPLEMENT_POD_PROPERTY(GCRuntimeShader, XShader)
IMPLEMENT_POD_PROPERTY(GCQImage, QImage)
IMPLEMENT_POD_PROPERTY(GCRuntimeGeometry, XGeometry)

void GCRuntimeGeometry::assignProperty(const SProperty *f, SProperty *t)
  {
  GCRuntimeGeometry *to = t->uncheckedCastTo<GCRuntimeGeometry>();

  const GCRuntimeGeometry *sProp = f->castTo<GCRuntimeGeometry>();
  if(sProp)
    {
    to->assign(sProp->value());
    return;
    }
  }

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

void GCRuntimeShader::assignProperty(const SProperty *f, SProperty *t)
  {
  GCRuntimeShader *to = t->uncheckedCastTo<GCRuntimeShader>();

  const GCRuntimeShader *sProp = f->castTo<GCRuntimeShader>();
  if(sProp)
    {
    to->assign(sProp->value());
    return;
    }
  }

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
