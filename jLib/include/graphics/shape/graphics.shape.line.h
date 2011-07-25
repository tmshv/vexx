#ifndef JLIB_GRAPHICS_SHAPE_LINE_H
#define JLIB_GRAPHICS_SHAPE_LINE_H

#include "graphics/shape/graphics.shape.types.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

class JLIB_EXPORT line
    {
public:
    //! Position of the line
    math::triple position;
    //! The direction of the line
    math::triple direction;

    //! Initiate from a point and a vector
    line( math::triple=math::triple(), math::triple=math::triple() );
    ~line();

    //! Find whether a point lies on the line
    bool liesOn( math::triple );
    //! Sample this line at t
    math::triple sample( JFLOAT );
    //! Returns the shortest distance to a point
    JFLOAT shortestDistance( math::triple );
    //! Check for the shortest distance between a line and this line
    JFLOAT shortestDistance( line );
    //! Return the t for this line at which this line is closest to the given line
    JFLOAT collision( line );
    //! Return the t for this line at which this line is closest to the given line
    JFLOAT collision( math::triple );
    };

shape::line lineToSpace( shape::line, math::matrix4x4 );

END_JLIB_GRAPHICS_SHAPE_NAMESPACE

#endif
