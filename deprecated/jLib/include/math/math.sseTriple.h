#ifndef MATH_SSETRIPLE_H
#define MATH_SSETRIPLE_H

#ifdef __SSE__

#include "xmmintrin.h"
#include "math/math.types.h"
#include "math/math.triple.h"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_PRE_ALIGN( 16 ) sseTriple
    {
public:
    inline JFLOAT x() const;
    inline JFLOAT y() const;
    inline JFLOAT z() const;

    inline void x( JFLOAT );
    inline void y( JFLOAT );
    inline void z( JFLOAT );

    inline sseTriple( );
    inline sseTriple( const triple & );
    inline sseTriple( JFLOAT, JFLOAT, JFLOAT );
    inline sseTriple( JFLOAT );

    inline triple toTriple() const;
    inline operator triple() const;

    inline void set( const triple & );
    inline void set( JFLOAT, JFLOAT, JFLOAT );
    inline void set( JFLOAT );

    inline JFLOAT dot( const sseTriple & ) const;
    inline sseTriple cross( const sseTriple & ) const;

    inline JFLOAT length( ) const;
    inline JFLOAT squaredLength() const;
    inline sseTriple normalise() const;
    inline sseTriple scaleTo( JFLOAT ) const;
    inline sseTriple negate( ) const;

    inline sseTriple operator+( const sseTriple & ) const;
    inline sseTriple operator-( const sseTriple & ) const;
    inline sseTriple operator*( const sseTriple & ) const;
    inline sseTriple operator/( const sseTriple & ) const;

    inline sseTriple operator+( const JFLOAT & ) const;
    inline sseTriple operator-( const JFLOAT & ) const;
    inline sseTriple operator*( const JFLOAT & ) const;
    inline sseTriple operator/( const JFLOAT & ) const;

    inline sseTriple &operator+=( const sseTriple & );
    inline sseTriple &operator-=( const sseTriple & );
    inline sseTriple &operator*=( const sseTriple & );
    inline sseTriple &operator/=( const sseTriple & );

    inline bool operator==( const sseTriple & ) const;
    inline bool operator==( const JFLOAT & ) const;
    inline bool operator!=( const sseTriple & ) const;
    inline bool operator!=( const JFLOAT & ) const;

    friend inline sseTriple operator+( const JFLOAT &, const sseTriple & );
    friend inline sseTriple operator-( const JFLOAT &, const sseTriple & );
    friend inline sseTriple operator*( const JFLOAT &, const sseTriple & );
    friend inline sseTriple operator/( const JFLOAT &, const sseTriple & );

    static void *operator new( size_t );
    static void operator delete( void * );

private:
    __m128 _data;
    } JLIB_POST_ALIGN( 16 );


END_JLIB_MATH_NAMESPACE

#include "math/math.sseTriple.inl"

#else
//#warning sseTriple is triple as no sse support
#include "math/math.triple.h"

BEGIN_JLIB_MATH_NAMESPACE
typedef triple sseTriple;
END_JLIB_MATH_NAMESPACE
#endif

#endif
