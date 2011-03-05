#ifndef JLIB_GRAPHICS_SHAPE_TRIANGLE_H
#define JLIB_GRAPHICS_SHAPE_TRIANGLE_H

#include "graphics/shape/graphics.shape.types.h"
#include "graphics/shape/graphics.shape.line.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

class JLIB_EXPORT triangle
    {
public:
    //! Position of the line
    math::triple pointA;
    //! Position of the line
    math::triple pointB;
    //! Position of the line
    math::triple pointC;

    //! Initiate with no data
    triangle( math::triple=math::triple(), math::triple=math::triple(), math::triple=math::triple() );
    ~triangle( );

    //! Check for the shortest distance to a point
    bool liesOn( math::triple a ) const;
    //! Check for the shortest distance between a line and this line
    bool collision( line a ) const;
    //! Find this triangles barycentric coordinates for a specific point
    void getBarycentric( math::triple, float &, float &, float & );

    math::triple normal() const;
    };

END_JLIB_GRAPHICS_SHAPE_NAMESPACE

#endif
