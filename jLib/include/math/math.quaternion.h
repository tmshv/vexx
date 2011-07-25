#ifndef JLIB_MATH_QUATERNION_H
#define JLIB_MATH_QUATERNION_H

#include "math/math.types.h"
#include "math/math.vectorX.h"
#include "math/math.triple.h"
#include "math/math.matrix3x3.h"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_EXPORT quaternion : public vectorX
    {
public:
    inline JFLOAT& x()
        { return cmp[0]; }
    inline JFLOAT& y()
        { return cmp[1]; }
    inline JFLOAT& z()
        { return cmp[2]; }
    inline JFLOAT& w()
        { return cmp[3]; }
    inline JFLOAT& i()
        { return cmp[0]; }
    inline JFLOAT& j()
        { return cmp[1]; }
    inline JFLOAT& k()
        { return cmp[2]; }
    inline JFLOAT& r()
        { return cmp[3]; }

    inline const JFLOAT& x() const
        { return cmp[0]; }
    inline const JFLOAT& y() const
        { return cmp[1]; }
    inline const JFLOAT& z() const
        { return cmp[2]; }
    inline const JFLOAT& w() const
        { return cmp[3]; }
    inline const JFLOAT& i() const
        { return cmp[0]; }
    inline const JFLOAT& j() const
        { return cmp[1]; }
    inline const JFLOAT& k() const
        { return cmp[2]; }
    inline const JFLOAT& r() const
        { return cmp[3]; }


    quaternion( );
    quaternion( const JFLOAT &W, const triple & );
    quaternion( JFLOAT X, JFLOAT Y, JFLOAT Z, JFLOAT W );
    quaternion( const quaternion &nv );
    quaternion( vectorX in );

    triple vector() const;
    JFLOAT angleRadians() const;
    JFLOAT angleDegrees() const;
    void setVector( const triple & );
    void setAngleRadians( const JFLOAT );
    void setAngleDegrees( const JFLOAT );
    void setEulerAngles( triple );
    void setAxisAngle( triple, JFLOAT );
    triple eulerAngles( );

    quaternion multiply( const quaternion & ) const;
    inline quaternion operator*=( const quaternion & );
    inline quaternion operator*( const quaternion & ) const;

    static quaternion fromAxisAngle( triple, JFLOAT );
    static quaternion fromEulerAngles( triple );
    static quaternion identity();
    const static quaternion IDENTITY;
    };

inline quaternion quaternion::operator*=( const quaternion &in )
    {
    return ( *this = multiply( in ) );
    }
inline quaternion quaternion::operator*( const quaternion &in ) const
    {
    return multiply( in );
    }

END_JLIB_MATH_NAMESPACE

#endif
