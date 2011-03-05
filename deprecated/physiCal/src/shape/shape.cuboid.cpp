#include "shape/shape.cuboid.h"

BEGIN_PHYSICAL_NAMESPACE

cuboid::cuboid()
    {
    clear();
    }

cuboid::cuboid( pVector min, pVector max ) : _minimum( min ), _maximum( max )
    {
    }

cuboid cuboid::fromCuboid( const cuboid &in, pVector min, pVector max )
    {
    return cuboid(  in.minimum() + ( in.size()*min ), in.minimum() + ( in.size()*max ) );
    }

pVector cuboid::size() const
    {
    return _maximum - _minimum;
    }

pReal cuboid::volume() const
    {
    return size().x() * size().y() * size().z();
    }

void cuboid::clear()
    {
    _clear = TRUE;
    }

void cuboid::suggest( const pVector &suggestion )
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

bool cuboid::test( const pVector &in ) const
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

END_PHYSICAL_NAMESPACE
