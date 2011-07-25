#include "graphics/primitives/graphics.primitives.triangleCloud.h"
#include "graphics/entity/graphics.entity.bBox.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

triangleCloud::triangleCloud( entity::entityG *p ) : triangleArray( p ), _usedPoints( 0 ), _usedTriangles( 0 )
    {
    }

triangleCloud::triangleCloud( entity::entityG *p, file::geometryFileAbstract geo ) : triangleArray( p ), _usedPoints( 0 ), _usedTriangles( 0 )
    {
    jAssert( geo.positions.size() == geo.normals.size() );
    jAssert( geo.positions.size() == geo.textures.size() );
    jAssert( geo.positions.size() == geo.colours.size() );

    for( unsigned int x=0; x<geo.positions.size(); x++ )
        {
        addPoint( geo.positions[x], geo.normals[x], geo.textures[x], geo.colours[x] );
        }

    for( unsigned int x=0; x<geo.polygons.size(); x++ )
        {
        addPolygon( &(geo.polygons[x][0]), geo.polygons[x].size() );
        }

    }

unsigned int triangleCloud::addQuad( unsigned int a, unsigned int b, unsigned int c, unsigned int d )
    {
    if( _usedTriangles+3 >= sizeTriangles() )
        {
        setTriangleArraySize( _usedTriangles + 2 );
        }

    unsigned int ret = _usedTriangles;

    addTriangle( a, b, c );
    addTriangle( a, c, d );

    return ret;
    }

unsigned int triangleCloud::addPolygon( unsigned int *data, unsigned int n )
    {
    if( _usedTriangles+n+1 >= sizeTriangles() )
        {
        setTriangleArraySize( _usedTriangles + n );
        }
    unsigned int ret = _usedTriangles;
    for( unsigned int x=1; x<( n - 1 ); x++ )
        {
        addTriangle( data[0], data[x], data[x+1] );
        }
    return ret;
    }
unsigned int triangleCloud::addTriangleFan( unsigned int *data, unsigned int n )
    {
    if( _usedTriangles+n+1 >= sizeTriangles() )
        {
        setTriangleArraySize( _usedTriangles + n );
        }
    unsigned int ret = _usedTriangles;
    for( unsigned int x=1; x<( n - 1 ); x++ )
        {
        addTriangle( data[0], data[x+1], data[x] );
        }
    addTriangle( data[0], data[1], data[n-1] );
    return ret;
    }

unsigned int triangleCloud::addTriangle( unsigned int a, unsigned int b, unsigned int c )
    {
    if( _usedTriangles >= sizeTriangles() )
        {
        setTriangleArraySize( _usedTriangles + 1 );
        }
    setTriangle( _usedTriangles++, a, b, c );
    return _usedTriangles - 1;
    }

void triangleCloud::removeTriangle( unsigned int x )
    {
    _usedTriangles--;
    triangleArray::removeTriangle( x );
    }

void triangleCloud::clearTriangles( )
    {
    _usedTriangles = 0;
    triangleArray::clearTriangles( );
    }

unsigned int triangleCloud::addPoint( math::triple pos, math::triple norm, math::vector2 tex, math::colour4 col )
    {
    if( _usedPoints >= sizePoints() )
        {
        setPointArraySize( _usedPoints + 1 );
        }
    setPoint( _usedPoints++, pos, norm, tex, col );
    return _usedPoints - 1;
    }

void triangleCloud::removePoint( unsigned int pt )
    {
    for( unsigned int x=0; x<_pointData.size(); x++ )
        {
        if( _indexData[x].a == pt || _indexData[x].b == pt || _indexData[x].c == pt )
            {
            removeTriangle( x );
            }
        else if( _indexData[x].a > pt )
            {
            _indexData[x].a--;
            }
        else if( _indexData[x].b > pt )
            {
            _indexData[x].b--;
            }
        else if( _indexData[x].c > pt )
            {
            _indexData[x].c--;
            }
        }
    triangleArray::removePoint( pt );
    }

void triangleCloud::clearPoints( )
    {
    _usedPoints = 0;
    triangleArray::clearPoints( );
    }

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE
