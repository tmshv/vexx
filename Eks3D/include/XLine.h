#ifndef XLINE_H
#define XLINE_H

#include "X3DGlobal.h"
#include "XVector3D"
#include "XProperty"
#include "XTransform.h"

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

  void transform(const XTransform &);

  XVector3D sample(float) const;
  };

inline XLine operator*( const XTransform &tx, const XLine &line )
  {
  XLine l(line);
  l.transform(tx);
  return l;
  }

#endif // XLINE_H
