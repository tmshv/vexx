#ifndef JLIB_MATH_BEZIERPATCH_H
#define JLIB_MATH_BEZIERPATCH_H

#include "math/math.types.h"
#include "math/math.triple.h"
#include "math/math.bezierCurve.h"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_EXPORT bezierPatch
    {
public:
    //! Initiate a bezier patch with this many segments
    bezierPatch( unsigned int=1, unsigned int=1 );
    //! Get the control point at these coordinates on the patch
    triple &getPoint( unsigned int, unsigned int );
    //! Set the number of divisions in U, and say whether to rebuild
    void setSizeUPoints( unsigned int, bool=true );
    //! Set the number of divisions in V, and say whether to rebuild
    void setSizeVPoints( unsigned int, bool=true );
    //! Rebuild internal curves to the defined size, after changing the patch divisions
    void rebuildCurves( );
    //! Get the numebr of U divisions
    unsigned int getSizeUPoints( );
    //! Get the numebr of V divisions
    unsigned int getSizeVPoints( );
    //! Retrieve a control curve in the u direction
    bezierCurve getUCurve( unsigned int );
    //! Retrieve a control curve in the u direction
    bezierCurve getVCurve( unsigned int );
    //! Retrieve a Bezier Curve sampled at these coordinates along the surface
    bezierCurve sample( float );
    //! Sample at a position on the surface ( 0 -> 1, 0 -> 1 )
    triple sample( float, float );
    //! Sample the surface with these u and v divisions
    vector <vector<triple> > sampleAlong( unsigned int, unsigned int );
private:
    vector <bezierCurve> _uCurves;
    unsigned int _uDivisions;
    unsigned int _vDivisions;
    };

END_JLIB_MATH_NAMESPACE

#endif
