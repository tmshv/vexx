#include "shape/shape.triangle.h"
#include "shape/shape.plane.h"

BEGIN_PHYSICAL_NAMESPACE

pReal getTwiceAreaOfTriangle( const pVector &normal, const pVector &a, const pVector &b, const pVector &c )
    {
    return normal.dot( pVector( b-a ).cross( c-a ) );
    }

triangle::triangle( pVector a, pVector b, pVector c ) : pointA( a ), pointB( b ), pointC( c )
    {
    }

triangle::~triangle( )
    {
    }

// http://blackpawn.com/texts/pointinpoly/default.html
bool triangle::liesOn( pVector p ) const
    {
    pVector v0 = pointC - pointA;
    pVector v1 = pointB - pointA;
    pVector v2 = p - pointA;

    pReal dot00 = v0.dot( v0 );
    pReal dot01 = v0.dot( v1 );
    pReal dot02 = v0.dot( v2 );
    pReal dot11 = v1.dot( v1 );
    pReal dot12 = v1.dot( v2 );

    pReal invDenominator = 1 / ( dot00 * dot11 - dot01 * dot01 );
    pReal u = ( dot11 * dot02 - dot01 * dot12 ) * invDenominator;
    pReal v = ( dot00 * dot12 - dot01 * dot02 ) * invDenominator;

    if( u != u || v != v )
        {
        jDebugNeat<<"During Triangular collision a nan was encountered."<<endl;
        }

    return ( u > 0 ) && ( v > 0 ) && ( u + v < 1 );
    }

bool triangle::collision( line a, pReal *intersection ) const
    {
    plane pla( pointA, pVector( pointB - pointA ).cross( pointC - pointA ).normalise() );
    if( intersection )
        {
        *intersection = pla.collision( a );
        return liesOn( a.sample( *intersection ) );
        }
    else
        {
        return liesOn( a.sample( pla.collision( a ) ) );
        }
    }

void triangle::getBarycentric( pVector p, float &a, float &b, float &c )
    {
    pVector normal = pVector( pointB - pointA ).cross( pointC - pointA ).normalise();
    pReal abcArea = getTwiceAreaOfTriangle( normal, pointA, pointB, pointC );

    a = getTwiceAreaOfTriangle( normal, p, pointB, pointC ) / abcArea;
    b = getTwiceAreaOfTriangle( normal, pointA, p, pointC ) / abcArea;
    c = getTwiceAreaOfTriangle( normal, pointA, pointB, p ) / abcArea;
    }

END_PHYSICAL_NAMESPACE
