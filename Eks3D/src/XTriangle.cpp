#include "XTriangle.h"
#include "XLine.h"
#include "XPlane.h"
#include "QDebug"

XTriangle::XTriangle( const XVector3D &pA, const XVector3D &pB, const XVector3D &pC )
    : _pointA(pA), _pointB(pB), _pointC(pC)
  {
  }

bool XTriangle::intersects( const XLine &ray, XVector3D &pos )
  {
  XPlane pla( pointA(), pointB(), pointC() );
  pos = ray.sample( pla.intersection( ray ) );
  return liesOn( pos );
  }

// http://blackpawn.com/texts/pointinpoly/default.html
bool XTriangle::liesOn( const XVector3D &p ) const
    {
    XVector3D v0 = pointC() - pointA();
    XVector3D v1 = pointB() - pointA();
    XVector3D v2 = p - pointA();

    xReal dot00 = XVector3D::dotProduct( v0, v0 );
    xReal dot01 = XVector3D::dotProduct( v0, v1 );
    xReal dot02 = XVector3D::dotProduct( v0, v2 );
    xReal dot11 = XVector3D::dotProduct( v1, v1 );
    xReal dot12 = XVector3D::dotProduct( v1, v2 );

    xReal invDenominator = 1.0f / ( dot00 * dot11 - dot01 * dot01 );
    xReal u = ( dot11 * dot02 - dot01 * dot12 ) * invDenominator;
    xReal v = ( dot00 * dot12 - dot01 * dot02 ) * invDenominator;

    if( u != u || v != v )
        {
        qDebug() << "During Triangular collision a nan was encountered.";
        }

    return ( u >= 0.0f ) && ( v >= 0.0f ) && ( u + v <= 1.0f );
    }
