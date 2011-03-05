#ifndef JLIB_GRAPHICS_SHAPE_BOX_H
#define JLIB_GRAPHICS_SHAPE_BOX_H

#include "graphics/shape/graphics.shape.types.h"
#include "graphics/shape/graphics.shape.line.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

class JLIB_EXPORT box
    {
public:
    //! Position of the line
    math::triple position;
    //! The size of the lboxine
    math::triple size;

    //! Initiate
    box( math::triple=math::triple(), math::triple=math::triple() );
    ~box( );

    //! Check for the shortest distance to a point
    JFLOAT shortestDistance( math::triple a );
    //! Check for the shortest distance between a line and this line
    JFLOAT shortestDistance( line a );
    };

END_JLIB_GRAPHICS_SHAPE_NAMESPACE

#endif
