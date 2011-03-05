#ifndef JLIB_GRAPHICS_SHAPE_SHPERE_H
#define JLIB_GRAPHICS_SHAPE_SHPERE_H

#include "graphics/shape/graphics.shape.types.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

class JLIB_EXPORT sphere
    {
public:
    //! Position of the line
    math::triple position;
    //! The direction of the line
    float radius;

    sphere( math::triple=math::triple(), float=1 );
    ~sphere( );

    void collision( line, JFLOAT &, JFLOAT & );
    bool collision( line );

    //! Returns the shortest distance to between the two, values below 0 indicate the point is inside
    JFLOAT shortestDistance( math::triple );
    //! Check for the shortest distance between a line and this line, values below 0 indicate penetration
    JFLOAT shortestDistance( line );
    };

inline bool sphericalCollision( math::triple, JFLOAT r1, math::triple, JFLOAT r2 );

bool sphericalCollision( math::triple a, JFLOAT r1, math::triple b, JFLOAT r2 )
    {
    return ( ( a - b ).lengthSquared() ) < ( r1 + r2 ) * ( r1 + r2 );
    }

END_JLIB_GRAPHICS_SHAPE_NAMESPACE

#endif
