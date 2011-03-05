#ifndef SHAPE_PLANE_H
#define SHAPE_PLANE_H

#include "shape/shape.types.h"
#include "shape/shape.line.h"

BEGIN_PHYSICAL_NAMESPACE

class PHYSICAL_EXPORT plane
    {
public:
    //! Position of the line
    pVector position;
    //! The direction of the line
    pVector normal;

    //! Initiate from a point and a vector
    plane( pVector=pVector(), pVector=pVector() );
    ~plane();

    //! Find if a point is on a plane
    bool liesOn( pVector );
    //! Find if a point is on a plane
    bool liesOn( line );
    //! Collides this with a line, returns the T value of the line at collision, INF for no
    //! collision, and NaN for infinite collisions.
    pReal collision( line ) const;

    bool isInFront( const pVector & ) const;
    };

END_PHYSICAL_NAMESPACE

#endif
