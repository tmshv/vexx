#ifndef GCGEOMETRY_H
#define GCGEOMETRY_H

#include "GCGlobal.h"
#include "spropertycontainer.h"
#include "spropertyarray.h"
#include "sarrayproperty.h"
#include "sdatabase.h"
#include "XGeometry.h"
#include "sbasepointerproperties.h"
#include "GCBaseProperties.h"

class GRAPHICSCORE_EXPORT GCGeometry : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(GCGeometry, SPropertyContainer, 0)

public:
  GCGeometry();

  GCRuntimeGeometry runtimeGeometry;
  };

S_PROPERTY_INTERFACE(GCGeometry)

S_TYPED_POINTER_TYPE(GCGeometryPointer, GCGeometry)

S_TYPED_POINTER_ARRAY_TYPE(GCGeometryPointerArray, GCGeometryPointer)

#endif // GCGEOMETRY_H
