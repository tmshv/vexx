#ifndef JLIB_GRAPHICS_IMPLICIT_VECTORPLANE_H
#define JLIB_GRAPHICS_IMPLICIT_VECTORPLANE_H

#include "graphics/implicit/graphics.implicit.types.h"
#include "graphics/implicit/graphics.implicit.vector.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

class JLIB_EXPORT vectorPlane : public entity::entityG
    {
public:
    vectorPlane( entity::entityG *p, math::colour4 cXA=math::colour4( 1, 0, 0 ), math::colour4 cYA=math::colour4( 0, 1, 0 ), math::colour4 cXB=math::colour4( 0.6, 0, 0 ), math::colour4 cYB=math::colour4( 0, 0.6, 0 ), JFLOAT lengthX=1, JFLOAT lengthY=1 );

    void rebuild( );
    math::colour4 getColourXA( );
    math::colour4 getColourXB( );
    math::colour4 getColourYA( );
    math::colour4 getColourYB( );
    JFLOAT getLengthX( );
    JFLOAT getLengthY( );
    void setColourXA( math::colour4 c );
    void setColourXB( math::colour4 c );
    void setColourYA( math::colour4 c );
    void setColourYB( math::colour4 c );
    void setLengthX( JFLOAT x );
    void setLengthY( JFLOAT x );

private:
    implicit::vector vectorA;
    implicit::vector vectorB;
    primitives::lineCloud angleLine;
    primitives::triangleCloud angleQuad;
    };

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

#endif
