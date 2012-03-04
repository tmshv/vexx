#include "MCCube.h"
#include "XAssert"

#include "MCPolyhedron.h"

class MCCube::Impl
  {
public:
  Impl(MCCube* cube) : _cube(cube)
    {
    }

  MCPolyhedron _polyhedron;

  MCCube* _cube;
  };

S_IMPLEMENT_PROPERTY(MCCube)

SPropertyInformation *MCCube::createTypeInformation()
  {
  SPropertyInformation* info = SPropertyInformation::create<MCCube>("MCCube");

  GCGeometry::InstanceInformation *inst = info->add(&MCCube::geometry, "geometry");
  inst->setCompute(computeGeometry);


  return info;
  }

MCCube::MCCube()
  {
  _impl = new Impl(this);
  }

MCCube::~MCCube()
  {
  delete _impl;
  }

void MCCube::computeGeometry(const SPropertyInstanceInformation*, MCCube* cube)
  {
  MCPolyhedron& p = cube->_impl->_polyhedron;

  p.clear();

  MCHalfedgeHandle h = p.make_tetrahedron(XVector3D(1, 0, 0),
                                          XVector3D(0, 0, 1),
                                          XVector3D(0, 0, 0),
                                          XVector3D(0, 1, 0));
  MCHalfedgeHandle g = h->next()->opposite()->next();
  p.split_edge( h->next());
  p.split_edge( g->next());
  p.split_edge( g);
  h->next()->vertex()->point()     = XVector3D(1, 0, 1);
  g->next()->vertex()->point()     = XVector3D(0, 1, 1);
  g->opposite()->vertex()->point() = XVector3D(1, 1, 0);
  MCHalfedgeHandle f = p.split_facet( g->next(), g->next()->next()->next());
  MCHalfedgeHandle e = p.split_edge( f);
  e->vertex()->point() = XVector3D(1, 1, 1);
  p.split_facet( e, f->next()->next());


  GCGeometry& geo = cube->geometry;
  geo.clearAttributes();

  geo.addAttribute<SVector3ArrayProperty>("vertex");
  GCGeometryAttribute *vertex = geo.attribute("vertex");
  SVector3ArrayProperty *vertexData = vertex->attributeData<SVector3ArrayProperty>();

  geo.addAttribute<SVector3ArrayProperty>("normals");
  GCGeometryAttribute *normals = geo.attribute("normals");
  SVector3ArrayProperty *normalData = normals->attributeData<SVector3ArrayProperty>();

  SVector3ArrayProperty::EigenArray arr;
  SVector3ArrayProperty::EigenArray norm;

  xsize vSize = p.size_of_vertices();
  arr.resize(vSize, 1);
  norm.resize(vSize, 1);

    {
    MCPolyhedron::Vertex_const_iterator it = p.vertices_begin();
    MCPolyhedron::Vertex_const_iterator end = p.vertices_end();
    for(xsize i = 0; it != end; ++i, ++it)
      {
      const MCPolyhedron::Vertex& v = *it;
      const XVector3D& p = v.point();

      arr(i) = XVector3D(p.x(), p.y(), p.z());
      norm(i) = XVector3D(1, 1, 1);
      }
    }

  vertexData->setData(arr);
  normalData->setData(norm);

    {
    MCPolyhedron::Facet_const_iterator it = p.facets_begin();
    MCPolyhedron::Facet_const_iterator end = p.facets_end();
    for(size_t i = 0; it != end; ++i, ++it)
      {
      const MCPolyhedron::Facet& f = *it;

      xsize size = 0;

      MCPolyhedron::Halfedge_const_handle fIt = f.halfedge();
      MCPolyhedron::Halfedge_const_handle fEnd = f.halfedge();
      do
        {
        ++size;
        fIt = fIt->next();
        } while(fIt != fEnd);

      geo.addPolygon(size);

      xuint32* indices = (xsize*)alloca(size*sizeof(xuint32));

      fIt = f.halfedge();
      MCPolyhedron::Vertex_const_iterator vBegin = p.vertices_begin();
      xsize ptIdx = 0;
      do
        {
        MCPolyhedron::Vertex_const_iterator v = fIt->vertex();

        xuint32 vertIndex = 0;
        while(v != vBegin)
          {
          vertIndex++;
          --v;
          }

        indices[ptIdx++] = vertIndex;

        fIt = fIt->next();
        } while(fIt != fEnd);

      vertex->setPolygon(i, indices, size);

      }
    }
  }
