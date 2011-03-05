#include "graphics/shape/graphics.shape.cuboid.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

cuboid::cuboid()
    {
    clear();
    }

cuboid::cuboid( jLib::math::triple min, jLib::math::triple max ) : _minimum( min ), _maximum( max )
    {
    }

cuboid cuboid::fromCuboid( const cuboid &in, jLib::math::triple min, jLib::math::triple max )
    {
    return cuboid(  in.minimum() + ( in.size()*min ), in.minimum() + ( in.size()*max ) );
    }

jLib::math::triple cuboid::size() const
    {
    return _maximum - _minimum;
    }

JFLOAT cuboid::volume() const
    {
    return size().x() * size().y() * size().z();
    }

void cuboid::clear()
    {
    _clear = TRUE;
    }

void cuboid::suggest( const math::triple &suggestion )
    {
    if( _clear )
        {
        _minimum = suggestion;
        _maximum = suggestion;
        _clear = FALSE;
        }
    else
        {
        for( unsigned int x=0; x<3; x++ )
            {
            if( suggestion[x] < _minimum[x] )
                {
                _minimum[x] = suggestion[x];
                }
            if( suggestion[x] > _maximum[x] )
                {
                _maximum[x] = suggestion[x];
                }
            }
        }
    }

void cuboid::suggest( const cuboid &cube )
    {
    suggest( cube.minimum() );
    suggest( cube.maximum() );
    }

bool cuboid::test( const cuboid &in ) const
    {
    if( this == &in )
        {
        return TRUE;
        }
    for( unsigned int x=0; x<3; x++ )
        {
        if( in._maximum[x] < _minimum[x] || in._minimum[x] > _maximum[x] )
            {
            //jDebugNeat<<"Bail at test "<<x<<endl;
            return FALSE;
            }
        }
    return TRUE;
    }

bool cuboid::test( const math::triple &in ) const
    {
    for( unsigned int x=0; x<3; x++ )
        {
        if( in[x] < _minimum[x] || in[x] > _maximum[x] )
            {
            return FALSE;
            }
        }
    return TRUE;
    }

END_JLIB_GRAPHICS_SHAPE_NAMESPACE
