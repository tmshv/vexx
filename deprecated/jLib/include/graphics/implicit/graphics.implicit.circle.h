#ifndef JLIB_GRAPHICS_IMPLICIT_CIRCLE_H
#define JLIB_GRAPHICS_IMPLICIT_CIRCLE_H

#include "graphics/implicit/graphics.implicit.types.h"
#include "graphics/primitives/graphics.primitives.lineCloud.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

class JLIB_EXPORT circle : public primitives::lineCloud
    {
public:
    circle( entity::entityG *p, JFLOAT radius=1, unsigned int divisions=10, math::colour4 colour=math::colour4() );

    virtual void rebuild();
    void setRadius( JFLOAT );
    JFLOAT radius( ) const;
    void setDivisions( unsigned int );
    unsigned int divisions( );
    void setColour( math::colour4 );
    math::colour4 colour();

protected:
    unsigned int _divisions;
    JFLOAT _radius;
    math::colour4 _colour;
    };

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

#endif
