#include "XCuboid.h"

XCuboid::XCuboid() : _isValid( false )
    {
    }

XCuboid::XCuboid( XVector3D minimum, XVector3D maximum ) : _minimum( minimum ), _maximum( maximum ), _isValid( true )
    {
    }

XCuboid::XCuboid( XVector3D minimum, XSize size ) : _minimum( minimum ), _maximum( _minimum + size ), _isValid( true )
    {
    }

XCuboid XCuboid::united( const XCuboid &in ) const
    {
    return this->united( in.minimum() ).united( in.maximum() );
    }

XCuboid XCuboid::united( const XVector3D &in ) const
    {
    XCuboid ret( *this );
    if( !ret.isValid() )
        {
        ret._minimum = in;
        ret._maximum = in;
        ret._isValid = true;
        }
    else
        {
        if( in.x() < ret._minimum.x() )
            {
            ret._minimum.setX( in.x() );
            }
        if( in.x() > ret._maximum.x() )
            {
            ret._maximum.setX( in.x() );
            }
        if( in.y() < ret._minimum.y() )
            {
            ret._minimum.setY( in.y() );
            }
        if( in.y() > ret._maximum.y() )
            {
            ret._maximum.setY( in.y() );
            }
        if( in.z() < ret._minimum.z() )
            {
            ret._minimum.setZ( in.y() );
            }
        if( in.z() > ret._maximum.z() )
            {
            ret._maximum.setZ( in.y() );
            }
        }
    return ret;
    }

XCuboid &XCuboid::operator|=( const XCuboid &in )
    {
    *this = *this | in;
    return *this;
    }

XCuboid XCuboid::operator|( const XCuboid &in ) const
    {
    return in.united( *this );
    }

void XCuboid::setMinimum( const XVector3D &in )
    {
    _isValid = true;
    _minimum = in;
    }

void XCuboid::setMaximum( const XVector3D &in )
    {
    _isValid = true;
    _maximum = in;
    }

XSize XCuboid::size() const
    {
    return XSize( _maximum - _minimum );
    }

bool XCuboid::isInside( const XCuboid &in ) const
    {
    if( this == &in )
        {
        return TRUE;
        }
    return in.maximum().x() < _minimum.x() && in.minimum().x() > _maximum.x() &&
            in.maximum().y() < _minimum.y() && in.minimum().y() > _maximum.y() &&
            in.maximum().z() < _minimum.z() && in.minimum().z() > _maximum.z();
    }

bool XCuboid::isInside( const XVector3D &in ) const
    {
    return in.x() < _minimum.x() && in.x() > _maximum.x() &&
            in.y() < _minimum.y() && in.y() > _maximum.y() &&
            in.z() < _minimum.z() && in.z() > _maximum.z();
    }
