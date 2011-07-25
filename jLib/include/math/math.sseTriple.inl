#ifndef MATH_SSETRIPLE_INL_H
#define MATH_SSETRIPLE_INL_H

#include "math.sseTriple.h"

BEGIN_JLIB_MATH_NAMESPACE

JFLOAT sseTriple::x() const
    {
    float JLIB_PRE_ALIGN( 16 ) out[4] JLIB_POST_ALIGN( 16 );
    _mm_store_ps( out, _data );

    return out[1];
    }

JFLOAT sseTriple::y() const
    {
    float JLIB_PRE_ALIGN( 16 ) out[4] JLIB_POST_ALIGN( 16 );
    _mm_store_ps( out, _data );

    return out[2];
    }

JFLOAT sseTriple::z() const
    {
    float JLIB_PRE_ALIGN( 16 ) out[4] JLIB_POST_ALIGN( 16 );
    _mm_store_ps( out, _data );

    return out[3];
    }

inline void sseTriple::x( JFLOAT x )
    {
    set( x, y(), z() );
    }

inline void sseTriple::y( JFLOAT y )
    {
    set( x(), y, z() );
    }

inline void sseTriple::z( JFLOAT z )
    {
    set( x(), y(), z );
    }

sseTriple::sseTriple( )
    {
    }

sseTriple::sseTriple( const triple &in )
    {
    set( in );
    }

sseTriple::sseTriple( JFLOAT x, JFLOAT y, JFLOAT z )
    {
    set( x, y, z );
    }

sseTriple::sseTriple( JFLOAT x )
    {
    set( x, x, x );
    }

triple sseTriple::toTriple() const
    {
    float JLIB_PRE_ALIGN( 16 ) out[4] JLIB_POST_ALIGN( 16 );
    _mm_store_ps( out, _data );

    return triple( out[1], out[2], out[3] );
    }

sseTriple::operator triple() const
    {
    return toTriple();
    }

void sseTriple::set( const triple &in )
    {
    set( in.x(), in.y(), in.z() );
    }

void sseTriple::set( JFLOAT x, JFLOAT y, JFLOAT z )
    {
    _data = _mm_set_ps( z, y, x, 0.0 );
    }

void sseTriple::set( JFLOAT x )
    {
    set( x, x, x );
    }

JFLOAT sseTriple::dot( const sseTriple &in ) const
    {
    float JLIB_PRE_ALIGN( 16 ) out[4] JLIB_POST_ALIGN( 16 );
    _mm_store_ps( out, _mm_mul_ps( _data, in._data ) );

    return out[1] + out[2] + out[3];
    }

sseTriple sseTriple::cross( const sseTriple &in ) const
    {
    sseTriple ret;

    const int maskYZX = _MM_SHUFFLE(1,3,2,0);
    const int maskZXY = _MM_SHUFFLE(2,1,3,0);

    ret._data = _mm_sub_ps(
        _mm_mul_ps(
            _mm_shuffle_ps( _data, _data, maskYZX ),
            _mm_shuffle_ps( in._data, in._data, maskZXY ) ),
        _mm_mul_ps(
            _mm_shuffle_ps( _data, _data, maskZXY),
            _mm_shuffle_ps( in._data, in._data, maskYZX ) )
        );

    return ret;
    }

JFLOAT sseTriple::length( ) const
    {
    return sqrt( squaredLength() );
    }

JFLOAT sseTriple::squaredLength() const
    {
    return dot( *this );
    }

sseTriple sseTriple::normalise() const
    {
    return *this / length();
    }

sseTriple sseTriple::scaleTo( JFLOAT len ) const
    {
    return len * ( *this / length() );
    }

sseTriple sseTriple::negate( ) const
    {
    return *this * sseTriple( -1 );
    }

sseTriple sseTriple::operator+( const sseTriple &in ) const
    {
    sseTriple ret;
    ret._data = _mm_add_ps( _data, in._data );
    return ret;
    }

sseTriple sseTriple::operator-( const sseTriple &in ) const
    {
    sseTriple ret;
    ret._data = _mm_sub_ps( _data, in._data );
    return ret;
    }

sseTriple sseTriple::operator*( const sseTriple &in ) const
    {
    sseTriple ret;
    ret._data = _mm_mul_ps( _data, in._data );
    return ret;
    }

sseTriple sseTriple::operator/( const sseTriple &in ) const
    {
    sseTriple ret;
    ret._data = _mm_div_ps( _data, in._data );
    return ret;
    }

sseTriple sseTriple::operator+( const JFLOAT &f ) const
    {
    return *this + sseTriple( f );
    }

sseTriple sseTriple::operator-( const JFLOAT &f ) const
    {
    return *this - sseTriple( f );
    }

sseTriple sseTriple::operator*( const JFLOAT &f ) const
    {
    return *this * sseTriple( f );
    }

sseTriple sseTriple::operator/( const JFLOAT &f ) const
    {
    return *this / sseTriple( f );
    }

sseTriple &sseTriple::operator+=( const sseTriple &in )
    {
    _data = _mm_add_ps( _data, in._data );
    return *this;
    }

sseTriple &sseTriple::operator-=( const sseTriple &in )
    {
    _data = _mm_sub_ps( _data, in._data );
    return *this;
    }

sseTriple &sseTriple::operator*=( const sseTriple &in )
    {
    _data = _mm_mul_ps( _data, in._data );
    return *this;
    }

sseTriple &sseTriple::operator/=( const sseTriple &in )
    {
    _data = _mm_div_ps( _data, in._data );
    return *this;
    }

bool sseTriple::operator==( const sseTriple &in ) const
    {
    float JLIB_PRE_ALIGN( 16 ) out[4] JLIB_POST_ALIGN( 16 );

    _mm_store_ps( out, _mm_or_ps( _mm_cmpgt_ps( _data, ( in + EPSILON )._data ),
                                  _mm_cmplt_ps( _data, ( in - EPSILON )._data ) ) );

    return ((int)out[1] == 0x0) && ((int)out[2] == 0x0) && ((int)out[3] == 0x0);
    }

bool sseTriple::operator==( const JFLOAT &f ) const
    {
    return *this == sseTriple( f );
    }

bool sseTriple::operator!=( const sseTriple &in ) const
    {
    float JLIB_PRE_ALIGN( 16 ) out[4] JLIB_POST_ALIGN( 16 );

    _mm_store_ps( out, _mm_or_ps( _mm_cmpgt_ps( _data, ( in + EPSILON )._data ),
                                  _mm_cmplt_ps( _data, ( in - EPSILON )._data ) ) );

    return ((int)out[1] != 0x0) || ((int)out[2] != 0x0) || ((int)out[3] != 0x0);
    }

bool sseTriple::operator!=( const JFLOAT &f ) const
    {
    return *this != sseTriple( f );
    }

sseTriple operator+( const JFLOAT &f, const sseTriple &t )
    {
    return t * sseTriple( f );
    }

sseTriple operator-( const JFLOAT &f, const sseTriple &t )
    {
    return t - sseTriple( f );
    }

sseTriple operator*( const JFLOAT &f, const sseTriple &t )
    {
    return t * sseTriple( f );
    }

sseTriple operator/( const JFLOAT &f, const sseTriple &t )
    {
    return t / sseTriple( f );
    }

END_JLIB_MATH_NAMESPACE

#endif
