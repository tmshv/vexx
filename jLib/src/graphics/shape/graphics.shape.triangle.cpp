#include "graphics/shape/graphics.shape.triangle.h"
#include "graphics/shape/graphics.shape.plane.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

JFLOAT getTwiceAreaOfTriangle( const math::triple &normal, const math::triple &a, const math::triple &b, const math::triple &c )
    {
    return normal.dotProduct( math::triple( b-a ).crossProduct( c-a ) );
    }

triangle::triangle( math::triple a, math::triple b, math::triple c ) : pointA( a ), pointB( b ), pointC( c )
    {
    }

triangle::~triangle( )
    {
    }

// http://blackpawn.com/texts/pointinpoly/default.html
bool triangle::liesOn( math::triple p ) const
    {
    math::triple v0 = pointC - pointA;
    math::triple v1 = pointB - pointA;
    math::triple v2 = p - pointA;

    JFLOAT dot00 = v0.dotProduct( v0 );
    JFLOAT dot01 = v0.dotProduct( v1 );
    JFLOAT dot02 = v0.dotProduct( v2 );
    JFLOAT dot11 = v1.dotProduct( v1 );
    JFLOAT dot12 = v1.dotProduct( v2 );

    JFLOAT invDenominator = 1 / ( dot00 * dot11 - dot01 * dot01 );
    JFLOAT u = ( dot11 * dot02 - dot01 * dot12 ) * invDenominator;
    JFLOAT v = ( dot00 * dot12 - dot01 * dot02 ) * invDenominator;

    if( u != u || v != v )
        {
        jDebugNeat<<"During Triangular collision a nan was encountered."<<endl;
        }

    return ( u >= 0 ) && ( v >= 0 ) && ( u + v <= 1 );
    }

bool triangle::collision( line a ) const
    {
    plane pla( pointA, math::triple( pointB - pointA ).crossProduct( pointC - pointA ).normalise() );
    return liesOn( a.sample( pla.collision( a ) ) );
    }

void triangle::getBarycentric( math::triple p, float &a, float &b, float &c )
    {
    math::triple normal = math::triple( pointB - pointA ).crossProduct( pointC - pointA ).normalise();
    JFLOAT abcArea = getTwiceAreaOfTriangle( normal, pointA, pointB, pointC );

    a = getTwiceAreaOfTriangle( normal, p, pointB, pointC ) / abcArea;
    b = getTwiceAreaOfTriangle( normal, pointA, p, pointC ) / abcArea;
    c = getTwiceAreaOfTriangle( normal, pointA, pointB, p ) / abcArea;
    }

math::triple triangle::normal() const
    {
    return math::triple( pointB - pointA ).crossProduct( pointC - pointA ).normalise();
    }
END_JLIB_GRAPHICS_SHAPE_NAMESPACE
