#ifndef JLIB_GRAPHICS_IMPLICIT_VECTOR_H
#define JLIB_GRAPHICS_IMPLICIT_VECTOR_H

#include "graphics/implicit/graphics.implicit.types.h"
#include "graphics/primitives/graphics.primitives.triangleCloud.h"
#include "graphics/primitives/graphics.primitives.lineCloud.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

class JLIB_EXPORT vector : public entity::entityG
    {
public:
    vector( entityG *p, math::colour4 cA=math::colour4( 1, 0, 0 ), math::colour4 cB=math::colour4( 0.6, 0, 0 ), JFLOAT length=1 );

    void rebuild( );
    void setColourA( math::colour4 cA );
    void setColourB( math::colour4 cB );
    math::colour4 getColourA( );
    math::colour4 getColourB( );
    JFLOAT getLength( );
    void setLength( JFLOAT length );
    void setHighlightColour( math::colour4 );
    math::colour4 getHighlightColour( );
    bool getHighlight( );
    void setHighlight( bool=TRUE );


private:
    bool _hL;
    math::colour4 _hLCol;
    primitives::lineCloud line;
    primitives::triangleCloud point;
    JFLOAT _length;
    math::colour4 _colourA;
    math::colour4 _colourB;
    };

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

#endif
