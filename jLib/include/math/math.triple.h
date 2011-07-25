#ifndef JLIB_MATH_TRIPLE_H
#define JLIB_MATH_TRIPLE_H

//#include "math/math.types.h"
#include "math/math.vectorX.h"
#include "QVector3D"

BEGIN_JLIB_MATH_NAMESPACE

class vector2;

class JLIB_EXPORT triple : public vectorX
    {
public:
    inline void x( JFLOAT in )
        { cmp[0] = in; }
    inline void y( JFLOAT in )
        { cmp[1] = in; }
    inline void z( JFLOAT in )
        { cmp[2] = in; }
    inline JFLOAT& x()
        { return cmp[0]; }
    inline JFLOAT& y()
        { return cmp[1]; }
    inline JFLOAT& z()
        { return cmp[2]; }
    inline JFLOAT& r()
        { return cmp[0]; }
    inline JFLOAT& g()
        { return cmp[1]; }
    inline JFLOAT& b()
        { return cmp[2]; }
    inline JFLOAT& u()
        { return cmp[0]; }
    inline JFLOAT& v()
        { return cmp[1]; }
    inline JFLOAT& w()
        { return cmp[2]; }

    inline const JFLOAT& x() const
        { return cmp[0]; }
    inline const JFLOAT& y() const
        { return cmp[1]; }
    inline const JFLOAT& z() const
        { return cmp[2]; }
    inline const JFLOAT& r() const
        { return cmp[0]; }
    inline const JFLOAT& g() const
        { return cmp[1]; }
    inline const JFLOAT& b() const
        { return cmp[2]; }
    inline const JFLOAT& u() const
        { return cmp[0]; }
    inline const JFLOAT& v() const
        { return cmp[1]; }
    inline const JFLOAT& w() const
        { return cmp[2]; }

    triple( );
    triple( QVector3D );
    triple( JFLOAT in );
    triple( vector2, JFLOAT in );
    explicit triple( JFLOAT *in );
    triple( JFLOAT X, JFLOAT Y, JFLOAT Z );
    triple( const triple &nv );
    triple( vectorX in );

    JFLOAT dotProduct( triple in ) const;
    triple crossProduct( triple b ) const;
    JFLOAT dot( triple in ) const;
    triple cross( triple b ) const;    };

END_JLIB_MATH_NAMESPACE

#endif
