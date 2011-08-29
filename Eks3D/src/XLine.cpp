#include "XLine.h"
#include "XTransform.h"

XLine::XLine( XVector3D p1, XVector3D p2, ConstructionMode mode ) : _position(p1)
  {
  if(mode == TwoPoints)
    {
    _direction = p2-p1;
    }
  else
    {
    _direction = p2;
    }
  }

XVector3D XLine::sample( float t ) const
  {
  return position() + ( t * direction() );
  }

void XLine::transform( const XTransform &tx )
  {
  _position = XVector3D(tx * _position);
  _direction = XVector3D(tx.linear() * _direction);
  }
