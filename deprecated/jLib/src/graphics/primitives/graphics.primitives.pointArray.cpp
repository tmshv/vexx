#include "graphics/primitives/graphics.primitives.pointArray.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

pointArray::pointArray( entity::entityG *p, float s ) : entity::entityD( p ), alphaBlend( FALSE ), _size( s )
    {
    setDrawable( TRUE );
    }

pointArray::~pointArray( )
    {
    }

void pointArray::removePoint( unsigned int x )
    {
    jAssert( x < _pointData.size() );
    _pointData.erase( _pointData.begin() + x );
    }

void pointArray::setPoint( unsigned int x , math::triple p, math::colour4 c )
    {
    jAssert( x < _pointData.size() );
    setPointPosition( x, p );
    setPointColour( x, c );
    }

void pointArray::setPointPosition( unsigned int x, math::triple p )
    {
    jAssert( x < _pointData.size() );
    _pointData[x].position.x = p.x();
    _pointData[x].position.y = p.y();
    _pointData[x].position.z = p.z();
    }

void pointArray::setPointColour( unsigned int x, math::colour4 c )
    {
    jAssert( x < _pointData.size() );
#if POINTARRAY_NEEDS_COLOUR
    _pointData[x].colour.r = c.r();
    _pointData[x].colour.g = c.g();
    _pointData[x].colour.b = c.b();
    _pointData[x].colour.a = c.a();
#else
    jWarning<<"Your are trying to set a specific points colour when the pointArray class has been compiled without this attribute - set POINTARRAY_NEEDS_COLOUR"
#endif
    }

math::triple pointArray::getPointPosition( unsigned int x )
    {
    jAssert( x < _pointData.size() );
    return math::triple( _pointData.at( x ).position.x, _pointData.at( x ).position.y, _pointData.at( x ).position.z );
    }

math::colour4 pointArray::getPointColour( unsigned int x )
    {
    jAssert( x < _pointData.size() );
    return math::colour4( _pointData.at( x ).colour.r, _pointData.at( x ).colour.g, _pointData.at( x ).colour.b, _pointData.at( x ).colour.a );
    }

void pointArray::setPointArraySize( unsigned int n )
    {
    _pointData.resize( n );
    }

unsigned int pointArray::sizePoints( )
    {
    return _pointData.size();
    }

void pointArray::setDrawSize( JFLOAT s )
    {
    _size = s;
    }

void pointArray::clearPoints()
    {
    _pointData.clear();
    }

void pointArray::clear()
    {
    clearPoints( );
    }

void pointArray::draw( )
    {
    if( alphaBlend )
        {
        glEnable( GL_BLEND );
        }

#if !POINTARRAY_NEEDS_COLOUR
    colour.load();
#endif
    glPointSize( _size );

    glEnableClientState( GL_VERTEX_ARRAY );
#if POINTARRAY_NEEDS_COLOUR
    glEnableClientState( GL_COLOR_ARRAY );
#endif

    glVertexPointer( 3, GL_FLOAT, sizeof( struct pointArrayPrim ), &(_pointData[0].position) );
#if POINTARRAY_NEEDS_COLOUR
    glColorPointer( 4, GL_FLOAT, sizeof( struct pointArrayPrim ), &(_pointData[0].colour) );
#endif

    glDrawArrays( GL_POINTS, 0, _pointData.size() );

    glDisableClientState( GL_VERTEX_ARRAY );
#if POINTARRAY_NEEDS_COLOUR
    glDisableClientState( GL_COLOR_ARRAY );
#endif

    if( alphaBlend )
        {
        glDisable( GL_BLEND );
        }
    }

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE
