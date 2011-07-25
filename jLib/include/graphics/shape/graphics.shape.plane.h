#ifndef JLIB_GRAPHICS_SHAPE_PLANE_H
#define JLIB_GRAPHICS_SHAPE_PLANE_H

#include "graphics/shape/graphics.shape.types.h"
#include "graphics/shape/graphics.shape.line.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

class JLIB_EXPORT plane
    {
public:
    //! Position of the line
    math::triple position;
    //! The direction of the line
    math::triple normal;

    //! Initiate from a point and a vector
    plane( math::triple=math::triple(), math::triple=math::triple() );
    ~plane();

    //! Find if a point is on a plane
    bool liesOn( math::triple );
    //! Find if a point is on a plane
    bool liesOn( line );
    //! Collides this with a line, returns the T value of the line at collision, INF for no
    //! collision, and NaN for infinite collisions.
    JFLOAT collision( shape::line );

    bool isInFront( const math::triple &point ) const;
    };

END_JLIB_GRAPHICS_SHAPE_NAMESPACE

#endif
