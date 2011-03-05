#ifndef JLIB_MATH_MATRIX4X4STACKTRANSFORM_H
#define JLIB_MATH_MATRIX4X4STACKTRANSFORM_H

#include "math/math.types.h"
#include "math/math.matrix4x4Stack.h"

BEGIN_JLIB_MATH_NAMESPACE

class matrix4x4StackTransform : protected matrix4x4Stack
    {
public:
    matrix4x4StackTransform( );
    void setTranslate( triple );
    void setOrientation( quaternion );
    void setScale( triple );
    void setPreTransform( const matrix4x4 & );
    void setPostTransform( const matrix4x4 & );
    triple translate( ) const;
    int translateIndex( );
    quaternion orientation( ) const;
    matrix4x4 preTransform();
    matrix4x4 postTransform();
    int orientationIndex( );
    triple scale( ) const;
    int scaleIndex( );
    virtual void refresh( );
    inline matrix4x4 &operator[]( unsigned int x )
        { return matrix4x4Stack::at( x ); }
    inline matrix4x4 resultant() const
        { return matrix4x4Stack::resultant(); }
    unsigned int insert( unsigned int, matrix4x4 );

private:
    int _preRef;
    int _tRef;
    int _oRef;
    int _sRef;
    int _postRef;
    };

END_JLIB_MATH_NAMESPACE

#endif
