#ifndef SHAPE_LINE_H
#define SHAPE_LINE_H

#include "shape/shape.types.h"

BEGIN_PHYSICAL_NAMESPACE

class PHYSICAL_EXPORT line
    {
public:
    //! Position of the line
    pVector position;
    //! The direction of the line
    pVector direction;

    //! Initiate from a point and a vector
    line( pVector=pVector(), pVector=pVector() );
    ~line();

    //! Find whether a point lies on the line
    bool liesOn( pVector );
    //! Sample this line at t
    pVector sample( pReal );
    //! Returns the shortest distance to a point
    pReal shortestDistance( pVector );
    //! Check for the shortest distance between a line and this line
    pReal shortestDistance( line );
    //! Return the t for this line at which this line is closest to the given line
    pReal collision( line );
    //! Return the t for this line at which this line is closest to the given line
    pReal collision( pVector );
    };

line lineToSpace( line, jLib::math::matrix4x4 );

END_PHYSICAL_NAMESPACE

#endif
