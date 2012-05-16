#include "MCGeometry.h"
#include "spropertyinformationhelpers.h"
#include "sprocessmanager.h"
#include "QVarLengthArray"

S_IMPLEMENT_PROPERTY(MCGeometry, MeshCore)

void computeRuntimeGeometry(const SPropertyInstanceInformation *, MCGeometry *rtGeo)
  {
  xAssert(SProcessManager::isMainThread());

  XGeometry x;
  rtGeo->appendTo(&x);
  rtGeo->runtimeGeometry = x;
  }

void MCGeometry::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    GCRuntimeGeometry::InstanceInformation *rtGeo = info->child(&MCGeometry::runtimeGeometry);
    rtGeo->setCompute(computeRuntimeGeometry);
    rtGeo->setComputeLockedToMainThread(true);

    MCPolyhedronProperty::InstanceInformation *attrs = info->add(&MCGeometry::polygons, "polygons");
    attrs->setAffects(rtGeo);
    }
  }

MCGeometry::MCGeometry()
  {
  }

void MCGeometry::appendTo(XGeometry *geo) const
  {
  const MCPolyhedron &p = polygons();

  const QVector<xuint32>& trianglesIn = geo->triangles();
  QVector<XVector3D> positionsIn = geo->attributes3D()["vertex"];
  QVector<XVector3D> normalsIn = geo->attributes3D()["normals"];

  QVarLengthArray<xuint32> triangles;
  if(trianglesIn.size())
    {
    triangles.append(&trianglesIn.front(), trianglesIn.size());
    }

  QVarLengthArray<XVector3D> positions;
  if(positionsIn.size())
    {
    positions.append(&positionsIn.front(), positionsIn.size());
    }

  QVarLengthArray<XVector3D> normals;
  if(normalsIn.size())
    {
    normals.append(&normalsIn.front(), normalsIn.size());
    }

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

    fIt = f.halfedge();
    MCPolyhedron::Vertex_const_iterator vBegin = p.vertices_begin();
    xsize zeroIdx = positions.size();
    xsize ptIdx = 0;
    do
      {
      MCPolyhedron::Vertex_const_iterator vIt = fIt->vertex();
      const MCPolyhedron::Vertex& v = *vIt;

      // emit triangle
      if(ptIdx >= 2)
        {
        triangles << zeroIdx << zeroIdx + ptIdx - 1 << zeroIdx + ptIdx;
        }

      positions << v.point();
      normals << v.normal();

      fIt = fIt->next();
      ptIdx++;
      } while(fIt != fEnd);
    }

  geo->setAttribute("vertex", positions.data(), positions.size());
  geo->setAttribute("normals", normals.data(), normals.size());
  geo->setTriangles(triangles.data(), triangles.size());
  }
