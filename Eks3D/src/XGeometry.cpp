#include "XGeometry.h"
#include "XRenderer.h"
#include "XTriangle.h"
#include "XCuboid.h"

XAbstractGeometry::~XAbstractGeometry()
  {
  }

XGeometry::XGeometry( BufferType type ) : _internal( 0 ), _changedP( false ), _changedL( false ),
    _changedT( false ), _attributeSizeChanged( false ), _attributeSize( 0 ), _renderer( 0 ), _type( type )
  {
  }

XGeometry::XGeometry( const XGeometry &cpy ) : _internal( 0 ), _changedP( false ), _changedL( false ),
    _changedT( false ), _renderer( 0 ), _type( cpy._type )
  {
  _attributeSize = cpy._attributeSize;
  _attr1 = cpy._attr1;
  _attr2 = cpy._attr2;
  _attr3 = cpy._attr3;
  _attr4 = cpy._attr4;
  _points = cpy._points;
  _lines = cpy._lines;
  _triangles = cpy._triangles;
  }

XGeometry& XGeometry::operator=( const XGeometry &cpy )
  {
  if( _internal )
    {
    xAssert(_renderer);
    _renderer->destroyGeometry(_internal);
    _internal = 0;
    }

  _attributeSize = cpy._attributeSize;
  _attr1 = cpy._attr1;
  _attr2 = cpy._attr2;
  _attr3 = cpy._attr3;
  _attr4 = cpy._attr4;
  _points = cpy._points;
  _lines = cpy._lines;
  _triangles = cpy._triangles;

  return *this;
  }

const XVector<unsigned int> &XGeometry::points() const
  {
  return _points;
  }

const XVector<unsigned int> &XGeometry::lines() const
  {
  return _lines;
  }

const XVector<unsigned int> &XGeometry::triangles() const
  {
  return _triangles;
  }

const XHash <QString, XVector<xReal> > &XGeometry::attributes1D() const
  {
  return _attr1;
  }

const XHash <QString, XVector<XGeometry::Vertex2D> > &XGeometry::attributes2D() const
  {
  return _attr2;
  }

const XHash <QString, XVector<XGeometry::Vertex3D> > &XGeometry::attributes3D() const
  {
  return _attr3;
  }

const XHash <QString, XVector<XGeometry::Vertex4D> > &XGeometry::attributes4D() const
  {
  return _attr4;
  }

void XGeometry::setPoints( const XVector<unsigned int> &v )
  {
  _points = v;
  _changedP = true;
  }

void XGeometry::setLines( const XVector<unsigned int> &v )
  {
  _lines = v;
  _changedL = true;
  }

void XGeometry::setTriangles( const XVector<unsigned int> &v )
  {
  _triangles = v;
  _changedT = true;
  }

void XGeometry::setAttribute( QString n, const XVector<xReal> &v )
  {
  if( v.size() )
    {
    _attr1[n] = v;
    if( _attr1[n].size() != _attributeSize )
      {
      _attributeSize = _attr1[n].size();
      _attributeSizeChanged = true;
      }
    }
  else
    {
    _attr1.remove(n);
    }
  _changedA1 << n;
  }

void XGeometry::setAttribute( QString n, const XVector<Vertex2D> &v )
  {
  if( v.size() )
    {
    _attr2[n] = v;
    if( _attr2[n].size() != _attributeSize )
      {
      _attributeSize = _attr2[n].size();
      _attributeSizeChanged = true;
      }
    }
  else
    {
    _attr1.remove(n);
    }
  _changedA2 << n;
  }

void XGeometry::setAttribute( QString n, const XVector<Vertex3D> &v )
  {
  if( v.size() )
    {
    _attr3[n] = v;
    if( _attr3[n].size() != _attributeSize )
      {
      _attributeSize = _attr3[n].size();
      _attributeSizeChanged = true;
      }
    }
  else
    {
    _attr1.remove(n);
    }
  _changedA3 << n;
  }

void XGeometry::setAttribute( QString n, const XVector<Vertex4D> &v )
  {
  if( v.size() )
    {
    _attr4[n] = v;
    if( _attr4[n].size() != _attributeSize )
      {
      _attributeSize = _attr4[n].size();
      _attributeSizeChanged = true;
      }
    }
  else
    {
    _attr1.remove(n);
    }
  _changedA4 << n;
  }

void XGeometry::removeAttribute( QString in )
  {
  if( _attr1.contains(in) )
    {
    _attr1.remove( in );
    _changedA1 << in;
    _attributeSizeChanged = true;
    }
  if( _attr2.contains(in) )
    {
    _attr2.remove( in );
    _changedA2 << in;
    _attributeSizeChanged = true;
    }
  if( _attr3.contains(in) )
    {
    _attr3.remove( in );
    _changedA3 << in;
    _attributeSizeChanged = true;
    }
  if( _attr4.contains(in) )
    {
    _attr4.remove( in );
    _changedA4 << in;
    _attributeSizeChanged = true;
    }
  }

XCuboid XGeometry::computeBounds() const
  {
  const XVector<XGeometry::Vertex3D> &vtxList = _attr3["vertex"];

  XCuboid ret;
  foreach( const XGeometry::Vertex3D &vtx, vtxList )
    {
    ret.unite(XVector3D(vtx.a, vtx.b, vtx.c));
    }
  return ret;
  }

void XGeometry::setAttribute( QString name, const XList<xReal> &in )
  {
  setAttribute( name, in.toVector() );
  }

void XGeometry::setAttribute( QString name, const XList<QVector2D> &in )
  {
  XVector<Vertex2D> ver;
  ver.reserve( in.size() );
  foreach( QVector2D v, in )
    {
    ver << Vertex2D( v );
    }
  setAttribute( name, ver );
  }

void XGeometry::setAttribute( QString name, const XList<QVector3D> &in )
  {
  XVector<Vertex3D> ver;
  ver.reserve( in.size() );
  foreach( QVector3D v, in )
    {
    ver << Vertex3D( v );
    }
  setAttribute( name, ver );
  }

void XGeometry::setAttribute( QString name, const XList<QVector4D> &in )
  {
  XVector<Vertex4D> ver;
  ver.reserve( in.size() );
  foreach( QVector4D v, in )
    {
    ver << Vertex4D( v );
    }
  setAttribute( name, ver );
  }

void XGeometry::setPoints( const XList<unsigned int> &in )
  {
  setPoints( in.toVector() );
  }

void XGeometry::setLines( const XList<unsigned int> &in )
  {
  setLines( in.toVector() );
  }

void XGeometry::setTriangles( const XList<unsigned int> &in )
  {
  setTriangles( in.toVector() );
  }

void XGeometry::prepareInternal( XRenderer *r ) const
  {
  _renderer = r;
  if( !_internal )
    {
    _internal = _renderer->getGeometry( _type );
    _internal->setPoints( _points );
    _internal->setLines( _lines );
    _internal->setTriangles( _triangles );

    _internal->setAttributesSize( _attributeSize, _attr1.size(), _attr2.size(), _attr3.size(), _attr4.size() );

    foreach( QString na, _attr1.keys() )
      {
      _internal->setAttribute( na, _attr1[na] );
      }
    foreach( QString na, _attr2.keys() )
      {
      _internal->setAttribute( na, _attr2[na] );
      }
    foreach( QString na, _attr3.keys() )
      {
      _internal->setAttribute( na, _attr3[na] );
      }
    foreach( QString na, _attr4.keys() )
      {
      _internal->setAttribute( na, _attr4[na] );
      }
    }

  if( _changedP )
    {
    _internal->setPoints( _points );
    }
  if( _changedL )
    {
    _internal->setLines( _lines );
    }
  if( _changedT )
    {
    _internal->setTriangles( _triangles );
    }

  if( _attributeSizeChanged )
    {
    _internal->setAttributesSize( _attributeSize, _attr1.size(), _attr2.size(), _attr3.size(), _attr4.size() );

    foreach( QString na, _attr1.keys() )
      {
      _internal->setAttribute( na, _attr1[na] );
      }
    foreach( QString na, _attr2.keys() )
      {
      _internal->setAttribute( na, _attr2[na] );
      }
    foreach( QString na, _attr3.keys() )
      {
      _internal->setAttribute( na, _attr3[na] );
      }
    foreach( QString na, _attr4.keys() )
      {
      _internal->setAttribute( na, _attr4[na] );
      }
    }
  else
    {
    foreach( QString na, _changedA1 )
      {
      if( _attr1.contains( na ) )
        {
        _internal->setAttribute( na, _attr1[na] );
        }
      }
    foreach( QString na, _changedA2 )
      {
      if( _attr2.contains( na ) )
        {
        _internal->setAttribute( na, _attr2[na] );
        }
      }
    foreach( QString na, _changedA3 )
      {
      if( _attr3.contains( na ) )
        {
        _internal->setAttribute( na, _attr3[na] );
        }
      }
    foreach( QString na, _changedA4 )
      {
      if( _attr4.contains( na ) )
        {
        _internal->setAttribute( na, _attr4[na] );
        }
      }
    }
  }

XAbstractGeometry *XGeometry::internal() const
  {
  return _internal;
  }

QDataStream &operator<<( QDataStream &s, const XGeometry::Vertex2D &geo )
  {
  return s << geo.a << geo.b;
  }

QDataStream &operator>>( QDataStream &s, XGeometry::Vertex2D &geo )
  {
  return s >> geo.a >> geo.b;
  }

QDataStream &operator<<( QDataStream &s, const XGeometry::Vertex3D &geo )
  {
  return s << geo.a << geo.b << geo.c;
  }

QDataStream &operator>>( QDataStream &s, XGeometry::Vertex3D &geo )
  {
  return s >> geo.a >> geo.b >> geo.c;
  }

QDataStream &operator<<( QDataStream &s, const XGeometry::Vertex4D &geo )
  {
  return s << geo.a << geo.b << geo.c << geo.d;
  }

QDataStream &operator>>( QDataStream &s, XGeometry::Vertex4D &geo )
  {
  return s >> geo.a >> geo.b >> geo.c >> geo.d;
  }

QDataStream EKS3D_EXPORT &operator<<( QDataStream &s, const XGeometry &geo )
  {
  return s << geo._attr1 << geo._attr2 << geo._attr3 << geo._attr4 << geo._points << geo._lines << geo._triangles;
  }

QDataStream EKS3D_EXPORT &operator>>( QDataStream &s, XGeometry &geo )
  {
  if( geo._internal )
    {
    xAssert(geo._renderer);
    geo._renderer->destroyGeometry(geo._internal);
    geo._internal = 0;
    }

  geo._changedP = true;
  geo._changedL = true;
  geo._changedT = true;
  geo._attributeSizeChanged = true;

  s >> geo._attr1 >> geo._attr2 >> geo._attr3 >> geo._attr4 >> geo._points >> geo._lines >> geo._triangles;

  geo._attributeSize = -1;
  foreach( const XVector<xReal> &vec, geo._attr1 )
    {
    xAssert( geo._attributeSize == -1 || ( geo._attributeSize == vec.size() ) );
    geo._attributeSize = vec.size();
    }
  foreach( const XVector<XGeometry::Vertex2D> &vec, geo._attr2 )
    {
    xAssert( geo._attributeSize == -1 || ( geo._attributeSize == vec.size() ) );
    geo._attributeSize = vec.size();
    }
  foreach( const XVector<XGeometry::Vertex3D> &vec, geo._attr3 )
    {
    xAssert( geo._attributeSize == -1 || ( geo._attributeSize == vec.size() ) );
    geo._attributeSize = vec.size();
    }
  foreach( const XVector<XGeometry::Vertex4D> &vec, geo._attr4 )
    {
    xAssert( geo._attributeSize == -1 || ( geo._attributeSize == vec.size() ) );
    geo._attributeSize = vec.size();
    }

  if(geo._attributeSize == -1)
    {
    xAssert(geo._renderer);
    geo._renderer->destroyGeometry(geo._internal);
    geo._internal = 0;
    }

  return s;
  }

namespace XMeshUtilities
{
bool intersect( QString semantic,
                const XLine &ray,
                const XGeometry &geo,
                XVector3DList &posOut,
                XList <unsigned int> &triOut )
  {
  xAssert( geo.attributes3D().contains(semantic) );
  const XVector<XGeometry::Vertex3D> &positions = geo.attributes3D()[semantic];
  const XVector<unsigned int> &tris = geo.triangles();

  xAssert( tris.size()%3 == 0 );
  unsigned int trianglesSize = tris.size()/3;
  for( unsigned int index=0; index<trianglesSize; ++index )
    {
    const XGeometry::Vertex3D &a(positions[tris[index*3]]),
                              &b(positions[tris[(index*3)+1]]),
                              &c(positions[tris[(index*3)+2]]);

    XVector3D pos;
    if( XTriangle( XVector3D( a.a, a.b, a.c ),
                   XVector3D( b.a, b.b, b.c ),
                   XVector3D( c.a, c.b, c.c ) ).intersects( ray, pos ) )
      {
      posOut << pos;
      triOut << index;
      }
    }

  return posOut.size() > 0;
  }
}
