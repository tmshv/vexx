#include "graphics/implicit/graphics.implicit.vectorPlane.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

vectorPlane::vectorPlane( entity::entityG *p, math::colour4 cXA, math::colour4 cYA, math::colour4 cXB, math::colour4 cYB, JFLOAT lengthX, JFLOAT lengthY ) : entityG( p ), vectorA( this, cXA, cXB, lengthX ), vectorB( this, cYA, cYB, lengthY ), angleLine( this ), angleQuad( this )
    {
    vectorB.setOrientation( math::quaternion::fromEulerAngles( math::triple( 0, 0, DEGTORAD*90 ) ) );
    setDrawable( );
    rebuild( );
    }

void vectorPlane::rebuild( )
    {
    vectorA.rebuild( );
    vectorB.rebuild( );
    angleLine.clear( );
    angleQuad.clear( );

    math::colour4 lineColour = vectorA.getColourA() + vectorB.getColourA();
    lineColour.a() = 1;

    math::colour4 quadColour = vectorA.getColourB() + vectorB.getColourB();
    quadColour.a() = 0.5;
    angleQuad.alphaBlend = TRUE;

        {
        unsigned int pts[3];
        pts[0] = angleLine.addPoint( math::triple( 0.3, 0, 0 ), lineColour );
        pts[1] = angleLine.addPoint( math::triple( 0.3, 0.3, 0 ), lineColour );
        pts[2] = angleLine.addPoint( math::triple( 0, 0.3, 0 ), lineColour );
        angleLine.addLineStrip( pts, 3 );
        }
        {
        unsigned int pts[4];
        pts[0] = angleQuad.addPoint( math::triple( 0, 0, 0 ), math::triple(), math::vector2(), quadColour );
        pts[1] = angleQuad.addPoint( math::triple( 0.3, 0, 0 ), math::triple(), math::vector2(), quadColour );
        pts[2] = angleQuad.addPoint( math::triple( 0.3, 0.3, 0 ), math::triple(), math::vector2(), quadColour );
        pts[3] = angleQuad.addPoint( math::triple( 0, 0.3, 0 ), math::triple(), math::vector2(), quadColour );
        angleQuad.addQuad( pts[0], pts[1], pts[2], pts[3] );
        angleQuad.addQuad( pts[0], pts[3], pts[2], pts[1] );
        }
    }

math::colour4 vectorPlane::getColourXA( )
    {
    return vectorA.getColourA( );
    }

math::colour4 vectorPlane::getColourXB( )
    {
    return vectorA.getColourB( );
    }

math::colour4 vectorPlane::getColourYA( )
    {
    return vectorB.getColourA( );
    }

math::colour4 vectorPlane::getColourYB( )
    {
    return vectorB.getColourB( );
    }

JFLOAT vectorPlane::getLengthX( )
    {
    return vectorA.getLength( );
    }

JFLOAT vectorPlane::getLengthY( )
    {
    return vectorB.getLength( );
    }

void vectorPlane::setColourXA( math::colour4 c )
    {
    vectorA.setColourA( c );
    }

void vectorPlane::setColourXB( math::colour4 c )
    {
    vectorA.setColourB( c );
    }

void vectorPlane::setColourYA( math::colour4 c )
    {
    vectorB.setColourA( c );
    }

void vectorPlane::setColourYB( math::colour4 c )
    {
    vectorB.setColourB( c );
    }

void vectorPlane::setLengthX( JFLOAT x )
    {
    vectorA.setLength( x );
    }

void vectorPlane::setLengthY( JFLOAT x )
    {
    vectorB.setLength( x );
    }

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE
