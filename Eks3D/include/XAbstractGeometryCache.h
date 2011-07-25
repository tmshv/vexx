#ifndef XGEOMETRYCACHE_H
#define XGEOMETRYCACHE_H

#include "X3DGlobal.h"
#include "XMap"
#include "XList"
#include "XVector"
#include "XVector2D"
#include "XVector3D"
#include "XVector4D"

class XAbstractGeometryCache
    {
public:
    typedef unsigned int VertexRef;
    typedef VertexRef Point;
    struct Line { Line() { }; Line( VertexRef _a, VertexRef _b ) : a( _a ), b( _b ) { }; VertexRef a, b; };
    struct Triangle { Triangle() { }; Triangle( VertexRef _a, VertexRef _b, VertexRef _c ) : a( _a ), b( _b ), c( _c ) { }; VertexRef a, b, c; };

    struct Vertex2D
        {
        Vertex2D() { };
        Vertex2D( xReal _a, xReal _b ) : a( _a ), b( _b ) { };
        Vertex2D( QVector2D vec ) : a( vec.x() ), b( vec.y() ) { };
        xReal a, b;
        };
    struct Vertex3D
        {
        Vertex3D() { };
        Vertex3D( xReal _a, xReal _b, xReal _c ) : a( _a ), b( _b ), c( _c ) { };
        Vertex3D( QVector3D vec ) : a( vec.x() ), b( vec.y() ), c( vec.z() ) { };
        xReal a, b, c;
        };
    struct Vertex4D
        {
        Vertex4D() { };
        Vertex4D( xReal _a, xReal _b, xReal _c, xReal _d ) : a( _a ), b( _b ), c( _c ), d( _d ) { };
        Vertex4D( QVector4D vec ) : a( vec.x() ), b( vec.y() ), c( vec.z() ), d( vec.w() ) { };
        xReal a, b, c, d;
        };


    void setVertices( const XList<QVector3D> & );
    void setPoints( const XList<unsigned int> & );
    void setLines( const XList<unsigned int> & );
    void setTriangles( const XList<unsigned int> & );

    virtual void setVertices( const XVector<Vertex3D> & ) = 0;
    virtual void setPoints( const XVector<Point> & ) = 0;
    virtual void setLines( const XVector<Line> & ) = 0;
    virtual void setTriangles( const XVector<Triangle> & ) = 0;


    void setAttribute( QString, const XList<xReal> & );
    void setAttribute( QString, const XList<QVector2D> & );
    void setAttribute( QString, const XList<QVector3D> & );
    void setAttribute( QString, const XList<QVector4D> & );

    virtual void setAttribute( QString, const XVector<xReal> & ) = 0;
    virtual void setAttribute( QString, const XVector<Vertex2D> & ) = 0;
    virtual void setAttribute( QString, const XVector<Vertex3D> & ) = 0;
    virtual void setAttribute( QString, const XVector<Vertex4D> & ) = 0;
    virtual void removeAttribute( QString ) = 0;
    };

class XGLRenderer;
class XGLGeometryCache : public XAbstractGeometryCache
    {
public:
    virtual void setVertices( const XVector<Vertex3D> & );
    virtual void setPoints( const XVector<Point> & );
    virtual void setLines( const XVector<Line> & );
    virtual void setTriangles( const XVector<Triangle> & );

    virtual void setAttribute( QString, const XVector<xReal> & );
    virtual void setAttribute( QString, const XVector<Vertex2D> & );
    virtual void setAttribute( QString, const XVector<Vertex3D> & );
    virtual void setAttribute( QString, const XVector<Vertex4D> & );
    virtual void removeAttribute( QString );

private:
    XVector <Vertex3D> _vertices;

    XMap <QString, XVector<xReal> > _attr1;
    XMap <QString, XVector<Vertex2D> > _attr2;
    XMap <QString, XVector<Vertex3D> > _attr3;
    XMap <QString, XVector<Vertex4D> > _attr4;

    XVector <Point> _points;
    XVector <Line> _lines;
    XVector <Triangle> _triangles;

    friend class XGLRenderer;
    };

#endif // XGEOMETRYCACHE_H
