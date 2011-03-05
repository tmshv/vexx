#include "graphics/implicit/graphics.implicit.sphere.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

lineSphere::lineSphere( entity::entityG *p, JFLOAT r, unsigned int d, math::colour4 c ) : circle( p, r, d, c )
    {
    rebuild();
    }

void lineSphere::rebuild()
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

    for( unsigned int x=0; x<_divisions; x++ )
        {
        arr[x].x() = sin( inc * x ) * _radius;
        arr[x].z() = cos( inc * x ) * _radius;
        arr[x].y() = 0;
        }

    addLineLoop( arr, col, _divisions );

    for( unsigned int x=0; x<_divisions; x++ )
        {
        arr[x].z() = sin( inc * x ) * _radius;
        arr[x].y() = cos( inc * x ) * _radius;
        arr[x].x() = 0;
        }

    addLineLoop( arr, col, _divisions );
    }

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE
