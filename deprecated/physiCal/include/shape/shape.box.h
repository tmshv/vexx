#ifndef SHAPE_BOX_H
#define SHAPE_BOX_H

#include "shape/shape.types.h"
#include "shape/shape.line.h"

BEGIN_PHYSICAL_NAMESPACE

class PHYSICAL_EXPORT box
    {
public:
    //! Position of the line
    pVector position;
    //! The size of the lboxine
    pVector size;

    //! Initiate
    box( pVector=pVector(), pVector=pVector() );
    ~box( );

    //! Check for the shortest distance to a point
    pReal shortestDistance( pVector a );
    //! Check for the shortest distance between a line and this line
    pReal shortestDistance( line a );
    };

END_PHYSICAL_NAMESPACE

#endif
