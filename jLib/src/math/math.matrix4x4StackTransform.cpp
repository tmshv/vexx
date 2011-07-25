#include "math/math.matrix4x4StackTransform.h"
#include "math/math.triple.h"
#include "math/math.quaternion.h"

BEGIN_JLIB_MATH_NAMESPACE

matrix4x4StackTransform::matrix4x4StackTransform( ) : _tRef( -1 ), _oRef( -1 ), _sRef( -1 )
    {
    _preRef = add( matrix4x4::IDENTITY );
    _oRef = add( matrix4x4::IDENTITY );
    _sRef = add( matrix4x4::IDENTITY );
    _tRef = add( matrix4x4::IDENTITY );
    _postRef = add( matrix4x4::IDENTITY );
    }

void matrix4x4StackTransform::setTranslate( triple p )
    {
    at( _tRef ).setTranslation( p );

    refresh();
    }

void matrix4x4StackTransform::setOrientation( quaternion r )
    {
    at( _oRef ).setOrientation( r );
    refresh();
    }

void matrix4x4StackTransform::setScale( triple s )
    {
    at( _sRef ).setScale( s );
    refresh();
    }

void matrix4x4StackTransform::setPreTransform( const matrix4x4 &s )
    {
    at( _preRef ) = s;
    refresh();
    }

void matrix4x4StackTransform::setPostTransform( const matrix4x4 &s )
    {
    at( _postRef ) = s;
    refresh();
    }

triple matrix4x4StackTransform::translate( ) const
    {
    return at( _tRef ).translation();
    }

int matrix4x4StackTransform::translateIndex( )
    {
    return _tRef;
    }

quaternion matrix4x4StackTransform::orientation( ) const
    {
    return at( _oRef ).orientation();
    }

matrix4x4 matrix4x4StackTransform::preTransform( )
    {
    return at( _preRef );
    }

matrix4x4 matrix4x4StackTransform::postTransform( )
    {
    return at( _postRef );
    }

int matrix4x4StackTransform::orientationIndex( )
    {
    return _oRef;
    }

triple matrix4x4StackTransform::scale( ) const
    {
    return at( _sRef ).scale();
    }

int matrix4x4StackTransform::scaleIndex( )
    {
    return _sRef;
    }

void matrix4x4StackTransform::refresh( )
    {
    matrix4x4Stack::refresh();
    }

unsigned int matrix4x4StackTransform::insert( unsigned int x, matrix4x4 mat )
    {
    unsigned int ret = matrix4x4Stack::insert( x, mat );
    if( _preRef >= (int)x )
        {
        _preRef++;
        }
    if( _tRef >= (int)x )
        {
        _tRef++;
        }
    if( _sRef >= (int)x )
        {
        _sRef++;
        }
    if( _oRef >= (int)x )
        {
        _oRef++;
        }
    if( _postRef >= (int)x )
        {
        _postRef++;
        }
    return ret;
    }

END_JLIB_MATH_NAMESPACE
