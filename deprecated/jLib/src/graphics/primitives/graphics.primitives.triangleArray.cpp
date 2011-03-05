#include "graphics/primitives/graphics.primitives.triangleArray.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

triangleArray::triangleArray( entity::entityG *p ) : entity::entityD( p ), alphaBlend( FALSE )
    {
    setDrawable( );
    }

triangleArray::~triangleArray( )
    {
    }

jLib::graphics::shape::cuboid triangleArray::bounds() const
    {
    jLib::graphics::shape::cuboid ret;
    FOREACHCONST( _pointData, p )
        {
        ret.suggest( math::triple( p->position.x, p->position.y, p->position.z ) );
        }
    return ret;
    }

void triangleArray::setTriangle( unsigned int x, unsigned int a, unsigned int b, unsigned int c )
    {
    setTriangleA( x, a );
    setTriangleB( x, b );
    setTriangleC( x, c );
    }

void triangleArray::setTriangleA( unsigned int x, unsigned int a )
    {
    jAssert( x < _indexData.size() );
    _indexData[x].a = a;
    }

void triangleArray::setTriangleB( unsigned int x, unsigned int b )
    {
    jAssert( x < _indexData.size() );
    _indexData[x].b = b;
    }

void triangleArray::setTriangleC( unsigned int x, unsigned int c )
    {
    jAssert( x < _indexData.size() );
    _indexData[x].c = c;
    }

unsigned int triangleArray::getTriangleA( unsigned int x ) const
    {
    return _indexData[x].a;
    }

unsigned int triangleArray::getTriangleB( unsigned int x ) const
    {
    return _indexData[x].b;
    }

unsigned int triangleArray::getTriangleC( unsigned int x ) const
    {
    return _indexData[x].c;
    }

void triangleArray::removeTriangle( unsigned int x )
    {
    _indexData.erase( _indexData.begin() + x );
    }

void triangleArray::setTriangleArraySize( unsigned int n )
    {
    _indexData.resize( n );
    }

unsigned int triangleArray::sizeTriangles( ) const
    {
    return _indexData.size( );
    }

void triangleArray::clearTriangles( )
    {
    _indexData.clear( );
    }

void triangleArray::setPoint( unsigned int x, math::triple pos, math::triple norm, math::vector2 tex, math::colour4 col )
    {
    setPointPosition( x, pos );
    setPointNormal( x, norm );
    setPointTexture( x, tex );
    setPointColour( x, col );
    }

void triangleArray::setPointPosition( unsigned int x, math::triple p )
    {
    jAssert( x < _pointData.size() );
    _pointData[x].position.x = p.x();
    _pointData[x].position.y = p.y();
    _pointData[x].position.z = p.z();
    }

void triangleArray::setPointNormal( unsigned int x, math::triple n )
    {
#if TRIANGLEARRAY_NEEDS_NORMAL
    jAssert( x < _pointData.size() );
    _pointData[x].normal.x = n.x();
    _pointData[x].normal.y = n.y();
    _pointData[x].normal.z = n.z();
#endif
    }

void triangleArray::setPointTexture( unsigned int x, math::vector2 t )
    {
#if TRIANGLEARRAY_NEEDS_TEXTURE
    jAssert( x < _pointData.size() );
    _pointData[x].texture.x = t.x();
    _pointData[x].texture.y = t.y();
#endif
    }

void triangleArray::setPointColour( unsigned int x, math::colour4 c )
    {
#if TRIANGLEARRAY_NEEDS_COLOUR
    jAssert( x < _pointData.size() );
    _pointData[x].colour.r = c.r();
    _pointData[x].colour.g = c.g();
    _pointData[x].colour.b = c.b();
    _pointData[x].colour.a = c.a();
#endif
    }

math::triple triangleArray::getPointPosition( unsigned int x ) const
    {
    return math::triple( _pointData[x].position.x, _pointData[x].position.y, _pointData[x].position.z );
    }

math::triple triangleArray::getPointNormal( unsigned int x ) const
    {
    return math::triple( _pointData[x].normal.x, _pointData[x].normal.y, _pointData[x].normal.z );
    }

math::vector2 triangleArray::getPointTexture( unsigned int x ) const
    {
    return math::vector2( _pointData[x].texture.x, _pointData[x].texture.y );
    }

math::colour4 triangleArray::getPointColour( unsigned int x ) const
    {
    return math::colour4( _pointData[x].colour.r, _pointData[x].colour.g, _pointData[x].colour.b, _pointData[x].colour.a );
    }

void triangleArray::removePoint( unsigned int x )
    {
    _pointData.erase( _pointData.begin() + x );
    }

void triangleArray::setPointArraySize( unsigned int n )
    {
    _pointData.resize( n );
    }

unsigned int triangleArray::sizePoints( ) const
    {
    return _pointData.size( );
    }

void triangleArray::clearPoints( )
    {
    _pointData.clear();
    }

void triangleArray::clear( )
    {
    clearPoints( );
    clearTriangles( );
    }

void triangleArray::draw( )
    {
    if( alphaBlend )
        {
        glEnable( GL_BLEND );
        }

#if !TRIANGLEARRAY_NEEDS_COLOUR
    colour.load();
#endif

    glEnableClientState( GL_VERTEX_ARRAY );
#if TRIANGLEARRAY_NEEDS_COLOUR
    glEnableClientState( GL_COLOR_ARRAY );
#endif
#if TRIANGLEARRAY_NEEDS_NORMAL
    glEnableClientState( GL_NORMAL_ARRAY );
#endif
#if TRIANGLEARRAY_NEEDS_TEXTURE
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
#endif

    glVertexPointer( 3, GL_FLOAT, sizeof( struct triangleArrayPointPrim ), &(_pointData[0].position) );
#if TRIANGLEARRAY_NEEDS_COLOUR
    glColorPointer( 4, GL_FLOAT, sizeof( struct triangleArrayPointPrim ), &(_pointData[0].colour) );
#endif
#if TRIANGLEARRAY_NEEDS_NORMAL
    glNormalPointer( GL_FLOAT, sizeof( struct triangleArrayPointPrim ), &(_pointData[0].normal) );
#endif
#if TRIANGLEARRAY_NEEDS_TEXTURE
    glTexCoordPointer( 2, GL_FLOAT, sizeof( struct triangleArrayPointPrim ), &(_pointData[0].texture) );
#endif

    glDrawElements( GL_TRIANGLES, _indexData.size()*3, GL_UNSIGNED_INT, &(_indexData[0].a) );

    /*for( unsigned int x=0; x<_indexData.size(); x++ )
        {
        jDebugNeat<<_indexData[x].a<<" "<<_indexData[x].b<<" "<<_indexData[x].c<<endl;
        jDebugNeat<<_pointData[_indexData[x].a].position.x<<" "<<_pointData[_indexData[x].a].position.y<<" "<<_pointData[_indexData[x].a].position.z<<endl;
        jDebugNeat<<_pointData[_indexData[x].b].position.x<<" "<<_pointData[_indexData[x].b].position.y<<" "<<_pointData[_indexData[x].b].position.z<<endl;
        jDebugNeat<<_pointData[_indexData[x].c].position.x<<" "<<_pointData[_indexData[x].c].position.y<<" "<<_pointData[_indexData[x].c].position.z<<endl;
        }*/

    glDisableClientState( GL_VERTEX_ARRAY );
#if TRIANGLEARRAY_NEEDS_COLOUR
    glDisableClientState( GL_COLOR_ARRAY );
#endif
#if TRIANGLEARRAY_NEEDS_NORMAL
    glDisableClientState( GL_NORMAL_ARRAY );
#endif
#if TRIANGLEARRAY_NEEDS_TEXTURE
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
#endif

    if( alphaBlend )
        {
        glDisable( GL_BLEND );
        }
    }

#if 0
triangleCloud::triangleCloud( entityG *p ) : entityD( p ), alphaBlend( FALSE ), _numIndexs( 0 ), _numPoints( 0 ), _indexBuf( 0 ), _vertexBuf( 0 )
    {
    setDrawable();
    }

triangleCloud::~triangleCloud( )
    {
    if( _vertexBuf != 0 )
        {
        delete [] _vertexBuf;
        _vertexBuf = 0;
        }
    if( _indexBuf != 0 )
        {
        delete [] _indexBuf;
        _indexBuf = 0;
        }
    }

unsigned int triangleCloud::addPoint( math::point4 p, math::vector4 n, math::vector2 t, colour4 c )
    {
    struct triangleCloudPointPrim *temp = new struct triangleCloudPointPrim[_numPoints+1];

    for( unsigned int x=0; x<_numPoints; x++ )
        {
        temp[x] = _vertexBuf[x];
        }

    temp[_numPoints].position.x = p.x();
    temp[_numPoints].position.y = p.y();
    temp[_numPoints].position.z = p.z();

#if TRIANGLECLOUD_NEEDS_NORMAL
    temp[_numPoints].normal.x = n.x();
    temp[_numPoints].normal.y = n.y();
    temp[_numPoints].normal.z = n.z();
#endif
#if TRIANGLECLOUD_NEEDS_TEXTURE
    temp[_numPoints].texture.x = t.x();
    temp[_numPoints].texture.y = t.y();
#endif
#if TRIANGLECLOUD_NEEDS_COLOUR
    temp[_numPoints].colour.r = c.r();
    temp[_numPoints].colour.g = c.g();
    temp[_numPoints].colour.b = c.b();
    temp[_numPoints].colour.a = c.a();
#endif
    _numPoints ++;

    if( _vertexBuf != 0 )
        {
        delete [] _vertexBuf;
        }
    _vertexBuf = temp;

    return _numPoints - 1;
    }
unsigned int triangleCloud::addPoint( math::point4 p, math::vector2 t, colour4 c )
    {
    return addPoint( p, math::vector4( 0 ), t, c );
    }

unsigned int triangleCloud::addPoint( math::point4 p, colour4 c )
    {
    return addPoint( p, math::vector4( 0 ), math::vector2( 0 ), c );
    }

void triangleCloud::removePoint( unsigned int pt )
    {
    if( pt < _numPoints && _numPoints > 1 )
        {
        struct triangleCloudPointPrim *temp = new struct triangleCloudPointPrim[_numPoints-1];

        for( unsigned int x=0; x<pt; x++ )
            {
            temp[x] = _vertexBuf[x];
            }

        for( unsigned int x=( pt + 1 ); x<_numPoints; x++ )
            {
            temp[x-1] = _vertexBuf[x];
            }


        _numPoints--;

        if( _vertexBuf != 0 )
            {
            delete [] _vertexBuf;
            }
        _vertexBuf = temp;
        }
    else if( _numPoints == 1 )
        {
        delete [] _vertexBuf;
        _vertexBuf = 0;
        _numPoints = 0;
        }
    }

unsigned int triangleCloud::addTriangle( unsigned int a, unsigned int b, unsigned int c )
    {
    unsigned int *temp = new unsigned int[_numIndexs + 3];

    for( unsigned int x=0; x<_numIndexs; x++ )
        {
        temp[x] = _indexBuf[x];
        }

    temp[_numIndexs] = a;
    temp[_numIndexs + 1] = b;
    temp[_numIndexs + 2] = c;

    _numIndexs += 3;

    if( _indexBuf != 0 )
        {
        delete [] _indexBuf;
        }
    _indexBuf = temp;

    return _numIndexs - 3;
    }

unsigned int addTriangles( unsigned int *, size_t s )
    {
    jAssert( !(s % 3) );
    jError<<"This function isnt implemented"<<endl;
    return 0;
    }

void triangleCloud::removeTriangle( unsigned int num )
    {
    jAssert( !(_numIndexs % 3) );
    if( num < (_numIndexs/3) && _numIndexs > 3 )
        {
        unsigned int *temp = new unsigned int[_numIndexs - 3];

        for( unsigned int x=0; x<( num * 3 ); x++ )
            {
            temp[x] = _indexBuf[x];
            }

        for( unsigned int x=( ( num + 1 ) * 3 ); x<_numIndexs; x++ )
            {
            temp[x-3] = _indexBuf[x];
            }

        _numIndexs -= 3;
        }
    else if( num == 3 )
        {
        delete [] _indexBuf;
        _indexBuf = 0;
        _numIndexs = 0;
        }
    }

unsigned int triangleCloud::addQuad( unsigned int, unsigned int, unsigned int, unsigned int )
    {
    jError<<"Not Implemented Quads fool."<<endl;
    return 0;
    }

unsigned int addQuads( unsigned int *, size_t s )
    {
    jAssert( !(s % 4) );
    jError<<"This function isnt implemented"<<endl;
    return 0;
    }

unsigned int triangleCloud::addPolygon( unsigned int *data, size_t s )
    {
    vector <unsigned int> temp;
    for( size_t x=0; x<s; x++ )
        {
        temp.push_back( data[x] );
        }
    return addPolygon( temp );
    }

unsigned int triangleCloud::addPolygon( vector<unsigned int> )
    {
    jError<<"Not Implemented Polygons fool."<<endl;
    return 0;
    }
unsigned int triangleCloud::numTriangles()
    {
    return _numIndexs/3;
    }

unsigned int triangleCloud::numPoints()
    {
    return _numPoints;
    }

void triangleCloud::draw( )
    {
#if !TRIANGLECLOUD_NEEDS_COLOUR
    colour.load();
#endif
    if( alphaBlend )
        {
        glEnable( GL_BLEND );
        }

    glEnableClientState( GL_VERTEX_ARRAY );
#if TRIANGLECLOUD_NEEDS_COLOUR
    glEnableClientState( GL_COLOR_ARRAY );
#endif
#if TRIANGLECLOUD_NEEDS_NORMAL
    glEnableClientState( GL_NORMAL_ARRAY );
#endif
#if TRIANGLECLOUD_NEEDS_TEXTURE
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
#endif

    glVertexPointer( 3, GL_FLOAT, sizeof( struct triangleCloudPointPrim ), &(_vertexBuf[0].position) );
#if TRIANGLECLOUD_NEEDS_COLOUR
    glColorPointer( 4, GL_FLOAT, sizeof( struct triangleCloudPointPrim ), &(_vertexBuf[0].colour) );
#endif
#if TRIANGLECLOUD_NEEDS_NORMAL
    glNormalPointer( GL_FLOAT, sizeof( struct triangleCloudPointPrim ), &(_vertexBuf[0].normal) );
#endif
#if TRIANGLECLOUD_NEEDS_TEXTURE
    glTexCoordPointer( 2, GL_FLOAT, sizeof( struct triangleCloudPointPrim ), &(_vertexBuf[0].texture) );
#endif

    glDrawElements( GL_TRIANGLES, _numIndexs, GL_UNSIGNED_INT, _indexBuf );


    glDisableClientState( GL_VERTEX_ARRAY );
#if TRIANGLECLOUD_NEEDS_COLOUR
    glDisableClientState( GL_COLOR_ARRAY );
#endif
#if TRIANGLECLOUD_NEEDS_NORMAL
    glDisableClientState( GL_NORMAL_ARRAY );
#endif
#if TRIANGLECLOUD_NEEDS_TEXTURE
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
#endif

    if( alphaBlend )
        {
        glDisable( GL_BLEND );
        }
    }
#endif

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE
