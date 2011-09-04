#ifndef GCBASEPROPERTIES_H
#define GCBASEPROPERTIES_H

#include "GCGlobal.h"
#include "sbaseproperties.h"
#include "XTransform.h"

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, TransformProperty, XTransform, XTransform::Identity());
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, ComplexTransformProperty, XComplexTransform, XComplexTransform());

#endif // GCBASEPROPERTIES_H
