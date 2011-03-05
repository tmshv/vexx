#include "graphics/implicit/graphics.implicit.circle.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

circle::circle( entity::entityG *p, JFLOAT rad, unsigned int div, math::colour4 col ) : lineCloud( p ),
        _divisions( div ), _radius( rad ), _colour( col )
    {
    rebuild();
    }

void circle::rebuild()
    {
    clear();

    math::triple arr[_divisions];
    math::colour4 col[_divisions];

    float inc = JM2xPI / _divisions;
    for( unsigned int x=0; x<_divisions; x++ )
        {
        arr[x].x() = sin( inc * x ) * _radius;
        arr[x].y() = cos( inc * x ) * _radius;
        arr[x].z() = 0;
        col[x] = _colour;
        }

    addLineLoop( arr, col, _divisions );
    }

void circle::setRadius( JFLOAT rad )
    {
    _radius = rad;
    }

JFLOAT circle::radius( ) const
    {
    return _radius;
    }

void circle::setDivisions( unsigned int div )
    {
    _divisions = div;
    }

unsigned int circle::divisions( )
    {
    return _divisions;
    }

void circle::setColour( math::colour4 col )
    {
    _colour = col;
    }

math::colour4 circle::colour()
    {
    return _colour;
    }

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE
