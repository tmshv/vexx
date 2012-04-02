#ifndef MCGEOMETRY_H
#define MCGEOMETRY_H

#include "MeshCore.h"
#include "3D/GCGeometry.h"
#include "MCPolyhedron.h"

class MESHCORE_EXPORT MCGeometry : public GCGeometry
  {
  S_PROPERTY_CONTAINER(MCGeometry, GCGeometry, 0)

public:
  MCGeometry();

  MCPolyhedronProperty polygons;

  void appendTo(XGeometry *geo) const;
  };

S_PROPERTY_INTERFACE(MCGeometry);

#endif // MCGEOMETRY_H
