#ifndef JLIB_MATH_BEZIERCURVE_H
#define JLIB_MATH_BEZIERCURVE_H

#include "math/math.types.h"
#include "math/math.triple.h"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_EXPORT bezierCurve
    {
public:
    //! Initiate an empty curve
    bezierCurve();
    //! Initiate a curve with the initial points pointed to by these arguments
    bezierCurve( triple *, unsigned int );
    //! Clear the curve ( no control points )
    void clear();
    //! Add a control point to the end of the curve
    void addPoint( triple );
    //! Insert a point at this position in the array in the curve
    void insertPoint( triple, unsigned );
    //! return the point at this position in the array
    triple &getPoint( unsigned int );
    //! Sample here, recommended ( 0 -> 1 )
    triple sample( float );
    //! Get the number of control points in the curve
    unsigned int size();
    //! Resize the array to this value ( deletes or adds zeroed points )
    void resize( unsigned int );
    //! Retrieve an array of this many points sampled along the curve ( 0 -> 1 )
    vector <triple> sampleAlong( unsigned int );
private:
    vector <triple> _points;
    };

END_JLIB_MATH_NAMESPACE

#endif
