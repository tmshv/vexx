#ifndef SHAPE_SHPERE_H
#define SHAPE_SHPERE_H

#include "shape/shape.types.h"
#include "shape/shape.line.h"

BEGIN_PHYSICAL_NAMESPACE

class PHYSICAL_EXPORT sphere
    {
public:
    //! Position of the line
    pVector position;
    //! The direction of the line
    float radius;

    sphere( pVector=pVector(), float=1 );
    ~sphere( );

    void collision( line, pReal &, pReal & );
    bool collision( line );

    //! Returns the shortest distance to between the two, values below 0 indicate the point is inside
    pReal shortestDistance( pVector );
    //! Check for the shortest distance between a line and this line, values below 0 indicate penetration
    pReal shortestDistance( line );
    };

inline bool sphericalCollision( pVector, pReal r1, pVector, pReal r2 );

bool sphericalCollision( pVector a, pReal r1, pVector b, pReal r2 )
    {
    return ( ( a - b ).lengthSquared() ) < ( r1 + r2 ) * ( r1 + r2 );
    }

END_PHYSICAL_NAMESPACE

#endif
