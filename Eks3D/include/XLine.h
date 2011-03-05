#ifndef XLINE_H
#define XLINE_H

#include "X3DGlobal.h"
#include "XVector3D"
#include "XProperty"

class XTransform;

class EKS3D_EXPORT XLine
  {
  XProperty( XVector3D, position, setPosition );
  XProperty( XVector3D, direction, setDirection );
public:
  enum ConstructionMode
    {
    TwoPoints,
    PointAndDirection
    };

  XLine( XVector3D, XVector3D, ConstructionMode=TwoPoints );

  XVector3D sample(float) const;
  };

XLine operator*( const XTransform &tx, const XLine &line );

#endif // XLINE_H
