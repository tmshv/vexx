#include "XModeller.h"
#include "XGeometry.h"
#include "XCurve"
#include "XSize"
#include "XCuboid.h"

XModeller::XModeller( XGeometry *g ) : _geo( g )
    {
    save( );
    }

XModeller::~XModeller( )
    {
    flush( );
    }

void XModeller::flush( )
    {
    _geo->setTriangles( _triIndices );
    _geo->setLines(  _linIndices );
    _geo->setPoints( _poiIndices );
    _geo->setAttribute( "vertex", _vertex.toVector() );
    if( _texture.size() )
        {
        _geo->setAttribute( "texture", _texture.toVector() );
        }
    if( _normals.size() )
        {
        _geo->setAttribute( "normal", _normals.toVector() );
        }
    if( _colours.size() )
        {
        _geo->setAttribute( "colour", _colours.toVector() );
        }
    }


void XModeller::begin( Type type )
    {
    _quadCount = 0;
    _states.back().type = type;
    }

void XModeller::end( )
    {
    _states.back().type = None;
    }

void XModeller::vertex( XVector3D vec )
    {
    if( _normals.size() || !qFuzzyCompare( _states.back().normal.a, (xReal)0.0 )
                        || !qFuzzyCompare( _states.back().normal.b, (xReal)0.0 )
                        || !qFuzzyCompare( _states.back().normal.c, (xReal)0.0 ) )
        {
        while( _normals.size() < _vertex.size() )
            {
            _normals << XGeometry::Vertex3D();
            }
        _normals << transformNormal( _states.back().normal );
        }

    if( _texture.size() || !qFuzzyCompare( _states.back().texture.a, (xReal)0.0 )
                        || !qFuzzyCompare( _states.back().texture.b, (xReal)0.0 ) )
        {
        while( _texture.size() < _vertex.size() )
            {
            _texture << XGeometry::Vertex2D();
            }
        _texture << _states.back().texture;
        }

    if( _colours.size() || !qFuzzyCompare( _states.back().colour.a, (xReal)0.0 )
                        || !qFuzzyCompare( _states.back().colour.b, (xReal)0.0 )
                        || !qFuzzyCompare( _states.back().colour.c, (xReal)0.0 )
                        || !qFuzzyCompare( _states.back().colour.d, (xReal)0.0 ) )
        {
        while( _colours.size() < _vertex.size() )
            {
            _colours << XGeometry::Vertex4D();
            }
        _colours << _states.back().colour;
        }

    _vertex << transformPoint( vec );

    if( _states.back().type == Points )
        {
        _poiIndices << _vertex.size() - 1;
        }
    else if( _states.back().type == Lines )
        {
        _linIndices << _vertex.size() - 1;
        }
    else if( _states.back().type == Triangles )
        {
        _triIndices << _vertex.size() - 1;

        if( _states.back().normalsAutomatic && ( _triIndices.size() % 3 ) == 0 )
            {
            int i1( _vertex.size() - 3 );
            int i2( _vertex.size() - 2 );
            int i3( _vertex.size() - 1 );
            while( _normals.size() < _vertex.size() )
                {
                _normals << XGeometry::Vertex3D();
                }
            QVector3D vec1( _vertex[i2].a - _vertex[i1].a, _vertex[i2].b - _vertex[i1].b, _vertex[i2].c - _vertex[i1].c );
            QVector3D vec2( _vertex[i3].a - _vertex[i1].a, _vertex[i3].b - _vertex[i1].b, _vertex[i3].c - _vertex[i1].c );

            _normals[i1] = _normals[i2] = _normals[i3] = QVector3D::crossProduct( vec1, vec2 ).normalized();
            }
        }
    else if( _states.back().type == Quads )
        {
        _quadCount++;
        _triIndices << _vertex.size() - 1;

        if( _quadCount == 4 )
            {
            if( _states.back().normalsAutomatic )
                {
                int i1( _vertex.size() - 4 );
                int i2( _vertex.size() - 3 );
                int i3( _vertex.size() - 2 );
                int i4( _vertex.size() - 1 );
                while( _normals.size() < _vertex.size() )
                    {
                    _normals << XGeometry::Vertex3D();
                    }
                QVector3D vec1( _vertex[i2].a - _vertex[i1].a, _vertex[i2].b - _vertex[i1].b, _vertex[i2].c - _vertex[i1].c );
                QVector3D vec2( _vertex[i3].a - _vertex[i1].a, _vertex[i3].b - _vertex[i1].b, _vertex[i3].c - _vertex[i1].c );

                _normals[i1] = _normals[i2] = _normals[i3] = _normals[i4] = QVector3D::crossProduct( vec1, vec2 ).normalized();
                }

            _triIndices << _triIndices[_triIndices.size()-4] << _triIndices[_triIndices.size()-2];
            _quadCount = 0;
            }
        }
    }

void XModeller::normal( XVector3D norm )
    {
    _states.back().normal = norm;
    }

void XModeller::texture( XVector2D tex )
    {
    _states.back().texture = tex;
    }

void XModeller::colour( XVector4D col )
    {
    _states.back().colour = col;
    }

void XModeller::setNormalsAutomatic( bool nAuto )
    {
    _states.back().normalsAutomatic = nAuto;
    if( nAuto )
        {
        _states.back().normal = XGeometry::Vertex3D();
        }
    }

bool XModeller::normalsAutomatic( ) const
    {
    return _states.back().normalsAutomatic;
    }

void XModeller::drawWireCube( const XCuboid &cube )
    {
    XSize size = cube.size();
    XVector3D min = cube.minimum();

    int sI = _vertex.size();

    _vertex << min
        << min + XVector3D(size.x(), 0.0f, 0.0f)
        << min + XVector3D(size.x(), size.y(), 0.0f)
        << min + XVector3D(0.0f, size.y(), 0.0f)
        << min + XVector3D(0.0f, 0.0f, size.z())
        << min + XVector3D(size.x(), 0.0f, size.z())
        << min + size
        << min + XVector3D(0.0f, size.y(), size.z());

    XVector3D n;
    XVector2D t;
    _normals << n << n << n << n << n << n << n << n;
    _texture << t << t << t << t << t << t << t << t;
    _linIndices << sI << sI+1
                << sI+1 << sI+2
                << sI+2 << sI+3
                << sI+3 << sI

                << sI+4 << sI+5
                << sI+5 << sI+6
                << sI+6 << sI+7
                << sI+7 << sI+4

                << sI+4 << sI
                << sI+5 << sI+1
                << sI+6 << sI+2
                << sI+7 << sI+3;
    }

void XModeller::drawCube( XVector3D h, XVector3D v, XVector3D d, float pX, float pY )
    {
    h *= 0.5; v *= 0.5; d *= 0.5;

    XVector3D p1( transformPoint( -h-v-d ) ),
              p2( transformPoint( h-v-d ) ),
              p3( transformPoint( h+v-d ) ),
              p4( transformPoint( -h+v-d ) ),
              p5( transformPoint( -h-v+d ) ),
              p6( transformPoint( h-v+d ) ),
              p7( transformPoint( h+v+d ) ),
              p8( transformPoint( -h+v+d ) );

    XGeometry::Vertex3D n1( transformNormal( XVector3D(0,1,0) ) ),
              n2( transformNormal( XVector3D(0,-1,0) ) ),
              n3( transformNormal( XVector3D(1,0,0) ) ),
              n4( transformNormal( XVector3D(-1,0,0) ) ),
              n5( transformNormal( XVector3D(0,0,-1) ) ),
              n6( transformNormal( XVector3D(0,0,1) ) );

    // Top Face BL
        {
        unsigned int begin( _vertex.size() );
        _triIndices << begin << begin + 1 << begin + 2 << begin + 2 << begin + 1 << begin + 3;

        _normals << n1 << n1 << n1 << n1;
        _texture << XVector2D(1.0/3.0,pY) << XVector2D(1.0/3.0,0.5-pY) << XVector2D(pX,pY) << XVector2D(pX,0.5-pY);
        _vertex << p3 << p4 << p7 << p8;
        }

    // Back Face BM
        {
        unsigned int begin( _vertex.size() );
        _triIndices << begin << begin + 1 << begin + 2 << begin + 2 << begin + 1 << begin + 3;

        _normals << n5 << n5 << n5 << n5;
        _texture << XVector2D(2.0/3.0,pY) << XVector2D(2.0/3.0,0.5-pY) << XVector2D(1.0/3.0,pY) << XVector2D(1.0/3.0,0.5-pY);
        _vertex << p2 << p1 << p3 << p4;
        }

    // Bottom Face BR
        {
        unsigned int begin( _vertex.size() );
        _triIndices << begin << begin + 1 << begin + 2 << begin + 2 << begin + 1 << begin + 3;

        _normals << n2 << n2 << n2 << n2;
        _texture << XVector2D(2.0/3.0,0.5-pY) << XVector2D(2.0/3.0,pY) << XVector2D(1-pX,0.5-pY) << XVector2D(1-pX,pY);
        _vertex << p1 << p2 << p5 << p6;
        }

    // Left Face TL
        {
        unsigned int begin( _vertex.size() );
        _triIndices << begin << begin + 1 << begin + 2 << begin + 2 << begin + 1 << begin + 3;

        _normals << n3 << n3 << n3 << n3;
        _texture << XVector2D(1-pX,0.5+pY) << XVector2D(1-pX,1-pY) << XVector2D(2.0/3.0,0.5+pY) << XVector2D(2.0/3.0,1-pY);
        _vertex << p2 << p3 << p6 << p7;
        }

    // Front Face TM
        {
        unsigned int begin( _vertex.size() );
        _triIndices << begin << begin + 1 << begin + 2 << begin + 2 << begin + 1 << begin + 3;

        _normals << n6 << n6 << n6 << n6;
        _texture << XVector2D(1.0/3.0,0.5+pY) << XVector2D(2.0/3.0,0.5+pY) << XVector2D(1.0/3.0,1-pY) << XVector2D(2.0/3.0,1-pY);
        _vertex << p5 << p6 << p8 << p7;
        }

    // Right Face TR
        {
        unsigned int begin( _vertex.size() );
        _triIndices << begin << begin + 1 << begin + 2 << begin + 2 << begin + 1 << begin + 3;

        _normals << n4 << n4 << n4 << n4;
        _texture << XVector2D(pX,1-pY) << XVector2D(pX,0.5+pY) << XVector2D(1.0/3.0,1-pY) << XVector2D(1.0/3.0,0.5+pY);
        _vertex << p4 << p1 << p8 << p5;
        }
    }

void XModeller::drawQuad( XVector3D h, XVector3D v )
    {
    h /= 2.0; v /= 2.0;

    unsigned int begin( _vertex.size() );
    _triIndices << begin << begin + 1 << begin + 2 << begin << begin + 2 << begin + 3;
    _vertex << transformPoint( -h - v ) << transformPoint( h - v ) << transformPoint( h + v ) << transformPoint( -h + v );
    _texture << XVector2D(0,0) << XVector2D(1,0) << XVector2D(1,1) << XVector2D(0,1);

    XGeometry::Vertex3D norm( transformNormal( XVector3D::crossProduct( h, v ).normalized() ) );
    _normals << norm << norm << norm << norm;
    }

void XModeller::drawGeometry( const XGeometry &geo )
    {
    unsigned int begin( _vertex.size() );

    _vertex << transformPoints( geo.attributes3D()["vertex"] ).toList();
    _texture << geo.attributes2D()["texture"].toList();
    _normals << transformNormals( geo.attributes3D()["normal"] ).toList();

    foreach( const unsigned int &i, geo.triangles() )
        {
        _triIndices << begin + i;
        }
    foreach( const unsigned int &i, geo.lines() )
        {
        _linIndices << begin + i;
        }
    foreach( const unsigned int &i, geo.points() )
        {
        _poiIndices << begin + i;
        }
    }

void XModeller::drawGeometry( XList <XVector3D> positions, const XGeometry &geo )
    {
    foreach( XVector3D pos, positions )
        {
        unsigned int begin( _vertex.size() );
        foreach( XGeometry::Vertex3D curPos, geo.attributes3D()["vertex"] )
            {
            _vertex << transformPoint( XVector3D( curPos.a, curPos.b, curPos.c ) + pos );
            }
        _texture << geo.attributes2D()["texture"].toList();
        _normals << transformNormals( geo.attributes3D()["normal"] ).toList();

        foreach( const unsigned int &i, geo.triangles() )
            {
            _triIndices << begin + i;
            }
        foreach( const unsigned int &i, geo.lines() )
            {
            _linIndices << begin + i;
            }
        foreach( const unsigned int &i, geo.points() )
            {
            _poiIndices << begin + i;
            }
        }
    }

void XModeller::drawLocator( XSize size, XVector3D center )
    {
    int begin( _vertex.size() );
    _linIndices << begin << begin + 1 << begin + 2 << begin + 3 << begin + 4 << begin + 5;

    _vertex << transformPoint( center + XVector3D( -size.x(), 0, 0 ) )
                      << transformPoint( center + XVector3D( size.x(), 0, 0 ) )
                      << transformPoint( center + XVector3D( 0, -size.y(), 0 ) )
                      << transformPoint( center + XVector3D( 0, size.y(), 0 ) )
                      << transformPoint( center + XVector3D( 0, 0, -size.z() ) )
                      << transformPoint( center + XVector3D( 0, 0, size.z() ) );

    _texture << XVector2D() << XVector2D() << XVector2D() << XVector2D() << XVector2D() << XVector2D();
    _normals << XVector3D() << XVector3D() << XVector3D() << XVector3D() << XVector3D() << XVector3D();
    }

void XModeller::setTransform( XTransform t )
    {
    _transform = t;
    }

XTransform XModeller::transform( ) const
    {
    return _transform;
    }

void XModeller::save()
    {
    _states << State();
    }

void XModeller::restore()
    {
    if( _states.size() > 1 )
        {
        _states.removeLast();
        }
    }

XVector3D XModeller::transformPoint( XVector3D in )
    {
    return _transform * in;
    }

XGeometry::Vertex3D XModeller::transformPoint( XGeometry::Vertex3D in )
    {
    return _transform * XVector3D( in.a, in.b, in.c );
    }

XVector <XVector3D> XModeller::transformPoints( const XVector <XVector3D> &list )
    {
    if( _transform.isIdentity() )
        {
        return list;
        }

    XVector <XVector3D> ret;
    ret.reserve(list.size());

    foreach( XVector3D v, list )
        {
        ret << _transform * v;
        }

    return ret;
    }

XVector <XGeometry::Vertex3D> XModeller::transformPoints( const XVector <XGeometry::Vertex3D> &list )
    {
    if( _transform.isIdentity() )
        {
        return list;
        }

    XVector <XGeometry::Vertex3D> ret;
    ret.reserve(list.size());

    foreach( XGeometry::Vertex3D v, list )
        {
        ret << _transform * XVector3D( v.a, v.b, v.c );
        }

    return ret;
    }

XVector3D XModeller::transformNormal( XVector3D in )
    {
    return _transform * in;
    }

XGeometry::Vertex3D XModeller::transformNormal( XGeometry::Vertex3D in )
    {
    return _transform.mapVector( XVector3D( in.a, in.b, in.c ) );
    }

XVector <XVector3D> XModeller::transformNormals( const XVector <XVector3D> &list )
    {
    if( _transform.isIdentity() )
        {
        return list;
        }

    XVector <XVector3D> ret;
    ret.reserve(list.size());

    foreach( XVector3D v, list )
        {
        ret << _transform.mapVector( v );
        }

    return ret;
    }

XVector <XGeometry::Vertex3D> XModeller::transformNormals( const XVector <XGeometry::Vertex3D> &list )
    {
    if( _transform.isIdentity() )
        {
        return list;
        }

    XVector <XGeometry::Vertex3D> ret;
    ret.reserve(list.size());

    foreach( XGeometry::Vertex3D v, list )
        {
        ret << _transform.mapVector( XVector3D( v.a, v.b, v.c ) );
        }

    return ret;
    }

void XModeller::drawCurve( const XAbstractCurve <XVector3D> &curve, int segments )
    {
    xReal start( curve.minimumT() );
    xReal inc( ( curve.maximumT() - curve.minimumT() ) / (segments-1) );

    int begin( _vertex.size() );

    _vertex << transformPoint( curve.sample( start ) );
    _texture << XVector2D();
    _normals << XVector3D();

    for( int x=1; x<segments; x++ )
        {
        _linIndices << begin + (x-1) << begin + x;

        _vertex << transformPoint( curve.sample( start + ( x * inc ) ) );

        _texture << XVector2D();
        _normals << XVector3D();
        }
    }
