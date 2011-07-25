#include "XAbstractGeometryCache.h"
#include "XAbstractRenderer.h"
#include "XDebug"


void XAbstractGeometryCache::setVertices( const XList<QVector3D> &in )
    {
    XVector<Vertex3D> ver;
    ver.reserve( in.size() );
    foreach( QVector3D v, in )
        {
        ver << Vertex3D( v );
        }
    setVertices( ver );
    }

void XAbstractGeometryCache::setPoints( const XList<unsigned int> &in )
    {
    setPoints( in.toVector() );
    }

void XAbstractGeometryCache::setLines( const XList<unsigned int> &in )
    {
    XVector<Line> ver;
    ver.reserve( in.size() / 2 );
    for( int x=0; x<in.size(); x+=2 )
        {
        ver << Line( in[x], in[x+1] );
        }
    setLines( ver );
    }

void XAbstractGeometryCache::setTriangles( const XList<unsigned int> &in )
    {
    XVector<Triangle> ver;
    ver.reserve( in.size() / 3 );
    for( int x=0; x<in.size(); x+=3 )
        {
        ver << Triangle( in[x], in[x+1], in[x+2] );
        }
    setTriangles( ver );
    }

void XAbstractGeometryCache::setAttribute( QString name, const XList<xReal> &in )
    {
    setAttribute( name, in.toVector() );
    }

void XAbstractGeometryCache::setAttribute( QString name, const XList<QVector2D> &in )
    {
    XVector<Vertex2D> ver;
    ver.reserve( in.size() );
    foreach( QVector2D v, in )
        {
        ver << Vertex2D( v );
        }
    setAttribute( name, ver );
    }

void XAbstractGeometryCache::setAttribute( QString name, const XList<QVector3D> &in )
    {
    XVector<Vertex3D> ver;
    ver.reserve( in.size() );
    foreach( QVector3D v, in )
        {
        ver << Vertex3D( v );
        }
    setAttribute( name, ver );
    }

void XAbstractGeometryCache::setAttribute( QString name, const XList<QVector4D> &in )
    {
    XVector<Vertex4D> ver;
    ver.reserve( in.size() );
    foreach( QVector4D v, in )
        {
        ver << Vertex4D( v );
        }
    setAttribute( name, ver );
    }

void XGLGeometryCache::setVertices( const XVector<Vertex3D> &ver )
    {
    _vertices = ver;
    }

void XGLGeometryCache::setPoints( const XVector<Point> &poi )
    {
    _points = poi;
    }

void XGLGeometryCache::setLines( const XVector<Line> &lin )
    {
    _lines = lin;
    }

void XGLGeometryCache::setTriangles( const XVector<Triangle> &tri )
    {
    _triangles = tri;
    }

void XGLGeometryCache::setAttribute( QString name, const XVector<xReal> &attr )
    {
    removeAttribute( name );
    _attr1.insert( name, attr );
    }

void XGLGeometryCache::setAttribute( QString name, const XVector<Vertex2D> &attr )
    {
    removeAttribute( name );
    _attr2.insert( name, attr );
    }

void XGLGeometryCache::setAttribute( QString name, const XVector<Vertex3D> &attr )
    {
    removeAttribute( name );
    _attr3.insert( name, attr );
    }

void XGLGeometryCache::setAttribute( QString name, const XVector<Vertex4D> &attr )
    {
    removeAttribute( name );
    _attr4.insert( name, attr );
    }

void XGLGeometryCache::removeAttribute( QString name )
    {
    if( _attr1.contains( name ) )
        {
        _attr1.remove( name );
        }
    if( _attr2.contains( name ) )
        {
        _attr2.remove( name );
        }
    if( _attr3.contains( name ) )
        {
        _attr3.remove( name );
        }
    if( _attr4.contains( name ) )
        {
        _attr4.remove( name );
        }
    }
