#include "XPlane.h"
#include "XLine.h"

XPlane::XPlane( ) : _d(0.0f)
  {
  }

XPlane::XPlane( float a, float b, float c, float d )
  {
  setD(d);
  setNormal(XVector3D(a,b,c));
  }

XPlane::XPlane( const XVector3D &point, const XVector3D &normal )
  {
  set(point, normal);
  }

XPlane::XPlane( const XVector3D &point, const XVector3D &liesOnPlaneA, const XVector3D &liesOnPlaneB )
  {
  XVector3D c = XVector3D::crossProduct(liesOnPlaneA-point,liesOnPlaneB-point);
  if( XVector3D::dotProduct( c, point ) < 0 )
    {
    c *= -1;
    }
  set( point, c );
  }

XVector3D XPlane::position() const
  {
  return normal() * d();
  }

void XPlane::set( const XVector3D &point, const XVector3D &n )
  {
  setNormal(n);
  setD( XVector3D::dotProduct( point, normal() ) );
  }

void XPlane::setNormal( const XVector3D &normal )
  {
  _normal = normal.normalized( );
  }

float XPlane::distanceToPlane( const XVector3D &in ) const
  {
  return XVector3D::dotProduct( in, _normal ) + d();
  }

float XPlane::intersection( const XLine &a ) const
  {
  xReal denominator = XVector3D::dotProduct( normal(), a.direction() );
  if( !qFuzzyCompare( denominator, 0.0f ) )
    {
    return XVector3D::dotProduct( normal(), position() - a.position() ) / denominator;
    }
  return HUGE_VAL;
  }
