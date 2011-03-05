#include "graphics/implicit/graphics.implicit.vectorAxis.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

vectorAxis::vectorAxis( entityG *p, math::colour4 cXA, math::colour4 cYA, math::colour4 cZA, math::colour4 cXB, math::colour4 cYB, math::colour4 cZB, JFLOAT lengthX, JFLOAT lengthY, JFLOAT lengthZ ) : entityG( p ), vectorA( this, cXA, cXB, lengthX ), vectorB( this, cYA, cYB, lengthY ), vectorC( this, cZA, cZB, lengthZ ), _quadSize( 0.3 ), angleLine( this ), angleQuad( this )
    {
    vectorC.setOrientation( math::quaternion::fromEulerAngles( math::triple( 0, 0, DEGTORAD*90 ) ) );
    vectorB.setOrientation( math::quaternion::fromEulerAngles( math::triple( 0, DEGTORAD*-90, 0 ) ) );
    setDrawable( );
    rebuild( );
    }

void vectorAxis::rebuild( )
    {
    vectorA.rebuild( );
    vectorB.rebuild( );
    vectorC.rebuild( );
    angleLine.clear( );
    angleQuad.clear( );
        {
        math::colour4 lineColour = vectorA.getColourA() + vectorB.getColourA();
        if( vectorA.getHighlight() && vectorB.getHighlight() )
            {
            lineColour = ( vectorA.getHighlightColour() + vectorB.getHighlightColour() ) / 2;
            }
        lineColour.a() = 1;

        math::colour4 quadColour = vectorA.getColourB() + vectorB.getColourB();
        if( vectorA.getHighlight() && vectorB.getHighlight() )
            {
            quadColour = ( vectorA.getHighlightColour() + vectorB.getHighlightColour() ) / 2;
            }
        quadColour.a() = 0.5;
        angleQuad.alphaBlend = TRUE;

            {
            unsigned int pts[3];
            pts[0] = angleLine.addPoint( math::triple( _quadSize, 0, 0 ), lineColour );
            pts[1] = angleLine.addPoint( math::triple( _quadSize, _quadSize, 0 ), lineColour );
            pts[2] = angleLine.addPoint( math::triple( 0, _quadSize, 0 ), lineColour );
            angleLine.addLineStrip( pts, 3 );
            }
            {
            unsigned int pts[4];
            pts[0] = angleQuad.addPoint( math::triple( 0, 0, 0 ), math::triple(), math::vector2(), quadColour );
            pts[1] = angleQuad.addPoint( math::triple( _quadSize, 0, 0 ), math::triple(), math::vector2(), quadColour );
            pts[2] = angleQuad.addPoint( math::triple( _quadSize, _quadSize, 0 ), math::triple(), math::vector2(), quadColour );
            pts[3] = angleQuad.addPoint( math::triple( 0, _quadSize, 0 ), math::triple(), math::vector2(), quadColour );
            angleQuad.addQuad( pts[0], pts[1], pts[2], pts[3] );
            angleQuad.addQuad( pts[0], pts[3], pts[2], pts[1] );
            }
        }
        {
        math::colour4 lineColour = vectorB.getColourA() + vectorC.getColourA();
        if( vectorB.getHighlight() && vectorC.getHighlight() )
            {
            lineColour = ( vectorB.getHighlightColour() + vectorC.getHighlightColour() ) / 2;
            }
        lineColour.a() = 1;

        math::colour4 quadColour = vectorB.getColourB() + vectorC.getColourB();
        if( vectorB.getHighlight() && vectorC.getHighlight() )
            {
            quadColour = ( vectorB.getHighlightColour() + vectorC.getHighlightColour() ) / 2;
            }
        quadColour.a() = 0.5;
        angleQuad.alphaBlend = TRUE;

            {
            unsigned int pts[3];
            pts[0] = angleLine.addPoint( math::triple( 0, _quadSize, 0 ), lineColour );
            pts[1] = angleLine.addPoint( math::triple( 0, _quadSize, _quadSize ), lineColour );
            pts[2] = angleLine.addPoint( math::triple( 0, 0, _quadSize ), lineColour );
            angleLine.addLineStrip( pts, 3 );
            }
            {
            unsigned int pts[4];
            pts[0] = angleQuad.addPoint( math::triple( 0, 0, 0 ), math::triple(), math::vector2(), quadColour );
            pts[1] = angleQuad.addPoint( math::triple( 0, _quadSize, 0 ), math::triple(), math::vector2(), quadColour );
            pts[2] = angleQuad.addPoint( math::triple( 0, _quadSize, _quadSize ), math::triple(), math::vector2(), quadColour );
            pts[3] = angleQuad.addPoint( math::triple( 0, 0, _quadSize ), math::triple(), math::vector2(), quadColour );
            angleQuad.addQuad( pts[0], pts[1], pts[2], pts[3] );
            angleQuad.addQuad( pts[0], pts[3], pts[2], pts[1] );
            }
        }
        {
        math::colour4 lineColour = vectorC.getColourA() + vectorA.getColourA();
        if( vectorC.getHighlight() && vectorA.getHighlight() )
            {
            lineColour = ( vectorC.getHighlightColour() + vectorA.getHighlightColour() ) / 2;
            }
        lineColour.a() = 1;

        math::colour4 quadColour = vectorC.getColourB() + vectorA.getColourB();
        if( vectorC.getHighlight() && vectorA.getHighlight() )
            {
            quadColour = ( vectorC.getHighlightColour() + vectorA.getHighlightColour() ) / 2;
            }
        quadColour.a() = 0.5;
        angleQuad.alphaBlend = TRUE;

            {
            unsigned int pts[3];
            pts[0] = angleLine.addPoint( math::triple( 0, 0, _quadSize ), lineColour );
            pts[1] = angleLine.addPoint( math::triple( _quadSize, 0, _quadSize ), lineColour );
            pts[2] = angleLine.addPoint( math::triple( _quadSize, 0, 0 ), lineColour );
            angleLine.addLineStrip( pts, 3 );
            }
            {
            unsigned int pts[4];
            pts[0] = angleQuad.addPoint( math::triple( 0, 0, 0 ), math::triple(), math::vector2(), quadColour );
            pts[1] = angleQuad.addPoint( math::triple( 0, 0, _quadSize ), math::triple(), math::vector2(), quadColour );
            pts[2] = angleQuad.addPoint( math::triple( _quadSize, 0, _quadSize ), math::triple(), math::vector2(), quadColour );
            pts[3] = angleQuad.addPoint( math::triple( _quadSize, 0, 0 ), math::triple(), math::vector2(), quadColour );
            angleQuad.addQuad( pts[0], pts[1], pts[2], pts[3] );
            angleQuad.addQuad( pts[0], pts[3], pts[2], pts[1] );
            }
        }
    }

void vectorAxis::setQuadSize( JFLOAT in )
    {
    _quadSize = in;
    }

JFLOAT vectorAxis::getQuadSize( )
    {

    return _quadSize;
    }

math::colour4 vectorAxis::getColourXA( )
    {
    return vectorA.getColourA( );
    }

math::colour4 vectorAxis::getColourXB( )
    {
    return vectorA.getColourB( );
    }

math::colour4 vectorAxis::getColourYA( )
    {
    return vectorB.getColourA( );
    }

math::colour4 vectorAxis::getColourYB( )
    {
    return vectorB.getColourB( );
    }

math::colour4 vectorAxis::getColourZA( )
    {
    return vectorC.getColourA( );
    }

math::colour4 vectorAxis::getColourZB( )
    {
    return vectorC.getColourB( );
    }

JFLOAT vectorAxis::getLengthX( )
    {
    return vectorA.getLength( );
    }

JFLOAT vectorAxis::getLengthY( )
    {
    return vectorB.getLength( );
    }

JFLOAT vectorAxis::getLengthZ( )
    {
    return vectorC.getLength( );
    }

void vectorAxis::setColourXA( math::colour4 c )
    {
    vectorA.setColourA( c );
    }

void vectorAxis::setColourXB( math::colour4 c )
    {
    vectorA.setColourB( c );
    }

void vectorAxis::setColourYA( math::colour4 c )
    {
    vectorB.setColourA( c );
    }

void vectorAxis::setColourYB( math::colour4 c )
    {
    vectorB.setColourB( c );
    }

void vectorAxis::setColourZA( math::colour4 c )
    {
    vectorC.setColourA( c );
    }

void vectorAxis::setColourZB( math::colour4 c )
    {
    vectorC.setColourB( c );
    }

void vectorAxis::setLengthX( JFLOAT x )
    {
    vectorA.setLength( x );
    }

void vectorAxis::setLengthY( JFLOAT x )
    {
    vectorB.setLength( x );
    }

void vectorAxis::setLengthZ( JFLOAT x )
    {
    vectorC.setLength( x );
    }

void vectorAxis::setHighlightColour( math::colour4 in )
    {
    vectorA.setHighlightColour( in );
    vectorB.setHighlightColour( in );
    vectorC.setHighlightColour( in );
    }

math::colour4 vectorAxis::getHighlightColour( )
    {
    return vectorA.getHighlightColour();
    }

void vectorAxis::setHighlight( int in )
    {
    vectorA.setHighlight( FALSE );
    vectorB.setHighlight( FALSE );
    vectorC.setHighlight( FALSE );
    if( ( in & X ) != 0 )
        {
        vectorA.setHighlight();
        }
    if( ( in & Y ) != 0 )
        {
        vectorB.setHighlight();
        }
    if( ( in & Z ) != 0 )
        {
        vectorC.setHighlight();
        }
    }

int vectorAxis::getHighlight( )
    {
    int ret = 0;
    if( vectorA.getHighlight() )
        {
        ret |= X;
        }
    if( vectorB.getHighlight() )
        {
        ret |= Y;
        }
    if( vectorC.getHighlight() )
        {
        ret |= Z;
        }
    return ret;
    }

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE
