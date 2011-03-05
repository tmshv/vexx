#ifndef JLIB_GRAPHICS_IMPLICIT_VECTORAXIS_H
#define JLIB_GRAPHICS_IMPLICIT_VECTORAXIS_H

#include "graphics/implicit/graphics.implicit.types.h"
#include "graphics/implicit/graphics.implicit.vector.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

class JLIB_EXPORT vectorAxis : public entity::entityG
    {
public:
    enum { X=1, Y=2, Z=4, XY=3, YZ=6, ZX=5, XYZ=7 } axis;
    vectorAxis( entity::entityG *p, math::colour4 cXA=math::colour4( 1, 0, 0 ), math::colour4 cYA=math::colour4( 0, 1, 0 ), math::colour4 cZA=math::colour4( 0, 0, 1 ), math::colour4 cXB=math::colour4( 0.6, 0, 0 ), math::colour4 cYB=math::colour4( 0, 0.6, 0 ), math::colour4 cZB=math::colour4( 0, 0, 0.6 ), JFLOAT lengthX=1, JFLOAT lengthY=1, JFLOAT lengthZ=1 );

    void rebuild( );
    math::colour4 getColourXA( );
    math::colour4 getColourXB( );
    math::colour4 getColourYA( );
    math::colour4 getColourYB( );
    math::colour4 getColourZA( );
    math::colour4 getColourZB( );
    JFLOAT getLengthX( );
    JFLOAT getLengthY( );
    JFLOAT getLengthZ( );
    void setColourXA( math::colour4 c );
    void setColourXB( math::colour4 c );
    void setColourYA( math::colour4 c );
    void setColourYB( math::colour4 c );
    void setColourZA( math::colour4 c );
    void setColourZB( math::colour4 c );
    void setLengthX( JFLOAT x );
    void setLengthY( JFLOAT x );
    void setLengthZ( JFLOAT x );
    void setHighlightColour( math::colour4 );
    math::colour4 getHighlightColour( );
    void setQuadSize( JFLOAT );
    JFLOAT getQuadSize( );
    void setHighlight( int );
    int getHighlight( );

private:
    implicit::vector vectorA;
    implicit::vector vectorB;
    implicit::vector vectorC;
    JFLOAT _quadSize;
    primitives::lineCloud angleLine;
    primitives::triangleCloud angleQuad;
    };

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

#endif
