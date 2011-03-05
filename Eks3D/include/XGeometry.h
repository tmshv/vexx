#ifndef XGEOMETRY_H
#define XGEOMETRY_H

#include "X3DGlobal.h"
#include "XMetaType"
#include "XHash"
#include "XList"
#include "XSet"
#include "XVector"
#include "XVector2D"
#include "XVector3D"
#include "XVector4D"
#include "XObject"
#include "XTransform.h"

class XAbstractGeometry;
class XRenderer;
class XCuboid;
class XLine;

class EKS3D_EXPORT XGeometry
    {
public:
    struct Vertex2D
        {
        Vertex2D() : a( 0.0 ), b( 0.0 ) { };
        Vertex2D( xReal _a, xReal _b ) : a( _a ), b( _b ) { };
        Vertex2D( QVector2D vec ) : a( vec.x() ), b( vec.y() ) { };
        xReal a, b;
        };
    struct Vertex3D
        {
        Vertex3D() : a( 0.0 ), b( 0.0 ), c( 0.0 ) { };
        Vertex3D( xReal _a, xReal _b, xReal _c ) : a( _a ), b( _b ), c( _c ) { };
        Vertex3D( QVector3D vec ) : a( vec.x() ), b( vec.y() ), c( vec.z() ) { };
        xReal a, b, c;
        };
    struct Vertex4D
        {
        Vertex4D() : a( 0.0 ), b( 0.0 ), c( 0.0 ), d( 0.0 ) { };
        Vertex4D( xReal _a, xReal _b, xReal _c, xReal _d ) : a( _a ), b( _b ), c( _c ), d( _d ) { };
        Vertex4D( QVector4D vec ) : a( vec.x() ), b( vec.y() ), c( vec.z() ), d( vec.w() ) { };
        xReal a, b, c, d;
        };

    enum BufferType { Static, Dynamic, Stream };
    XGeometry( BufferType=Static );
    XGeometry( const XGeometry & );
    XGeometry& operator=( const XGeometry & );

    const XVector<unsigned int> &points() const;
    const XVector<unsigned int> &lines() const;
    const XVector<unsigned int> &triangles() const;

    const XHash <QString, XVector<xReal> > &attributes1D() const;
    const XHash <QString, XVector<XGeometry::Vertex2D> > &attributes2D() const;
    const XHash <QString, XVector<XGeometry::Vertex3D> > &attributes3D() const;
    const XHash <QString, XVector<XGeometry::Vertex4D> > &attributes4D() const;

    void setPoints( const XList<unsigned int> & );
    void setLines( const XList<unsigned int> & );
    void setTriangles( const XList<unsigned int> & );

    void setPoints( const XVector<unsigned int> & );
    void setLines( const XVector<unsigned int> & );
    void setTriangles( const XVector<unsigned int> & );

    void setAttribute( QString, const XList<xReal> & );
    void setAttribute( QString, const XList<QVector2D> & );
    void setAttribute( QString, const XList<QVector3D> & );
    void setAttribute( QString, const XList<QVector4D> & );

    void setAttribute( QString, const XVector<xReal> & );
    void setAttribute( QString, const XVector<Vertex2D> & );
    void setAttribute( QString, const XVector<Vertex3D> & );
    void setAttribute( QString, const XVector<Vertex4D> & );

    void removeAttribute( QString );

    XCuboid computeBounds() const;

    void prepareInternal( XRenderer * ) const;
    XAbstractGeometry *internal() const;

    friend QDataStream EKS3D_EXPORT &operator<<( QDataStream &s, const XGeometry &geo );
    friend QDataStream EKS3D_EXPORT &operator>>( QDataStream &s, XGeometry &geo );

private:
    mutable XAbstractGeometry *_internal;
    XSet <QString> _changedA1;
    XSet <QString> _changedA2;
    XSet <QString> _changedA3;
    XSet <QString> _changedA4;
    bool _changedP;
    bool _changedL;
    bool _changedT;
    bool _attributeSizeChanged;
    int _attributeSize;
    XHash <QString, XVector<xReal> > _attr1;
    XHash <QString, XVector<XGeometry::Vertex2D> > _attr2;
    XHash <QString, XVector<XGeometry::Vertex3D> > _attr3;
    XHash <QString, XVector<XGeometry::Vertex4D> > _attr4;
    XVector <unsigned int> _points;
    XVector <unsigned int> _lines;
    XVector <unsigned int> _triangles;
    mutable XRenderer *_renderer;

    BufferType _type;
    };
X_DECLARE_SERIALISABLE_METATYPE(XGeometry);

class EKS3D_EXPORT XAbstractGeometry
    {
public:
    virtual ~XAbstractGeometry();
    virtual void setPoints( const XVector<unsigned int> & ) = 0;
    virtual void setLines( const XVector<unsigned int> & ) = 0;
    virtual void setTriangles( const XVector<unsigned int> & ) = 0;

    virtual void setAttributesSize( int, int, int, int, int ) = 0;

    virtual void setAttribute( QString, const XVector<xReal> & ) = 0;
    virtual void setAttribute( QString, const XVector<XGeometry::Vertex2D> & ) = 0;
    virtual void setAttribute( QString, const XVector<XGeometry::Vertex3D> & ) = 0;
    virtual void setAttribute( QString, const XVector<XGeometry::Vertex4D> & ) = 0;
    };

namespace XMeshUtilities
{
bool intersect( QString semantic, const XLine &ray, const XGeometry &geo, XVector3DList &pos, XList <unsigned int> &tris );
};

#endif // XGEOMETRY_H
