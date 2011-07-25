#ifndef SHAPE_TRIANGLE_H
#define SHAPE_TRIANGLE_H

#include "shape/shape.types.h"
#include "shape/shape.line.h"

BEGIN_PHYSICAL_NAMESPACE

class PHYSICAL_EXPORT triangle
    {
public:
    //! Position of the line
    pVector pointA;
    //! Position of the line
    pVector pointB;
    //! Position of the line
    pVector pointC;

    //! Initiate with no data
    triangle( pVector=pVector(), pVector=pVector(), pVector=pVector() );
    ~triangle( );

    //! Check for the shortest distance to a point
    bool liesOn( pVector a ) const;
    //! Check for the shortest distance between a line and this line
    bool collision( line a, pReal *intersection=0 ) const;
    //! Find this triangles barycentric coordinates for a specific point
    void getBarycentric( pVector, float &, float &, float & );
    };

END_PHYSICAL_NAMESPACE

#endif
