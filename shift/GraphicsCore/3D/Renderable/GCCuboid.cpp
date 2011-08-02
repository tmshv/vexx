#include "GCCuboid.h"
#include "sarrayproperty.h"

S_IMPLEMENT_PROPERTY(GCCuboid)

void computeCube(const SPropertyInstanceInformation *info, SPropertyContainer *cont)
  {
  GCCuboid *cube = cont->uncheckedCastTo<GCCuboid>();
  GCGeometry &geo = cube->geometry;

  float x = cube->width() / 2;
  float y = cube->height() / 2;
  float z = cube->depth() / 2;

  geo.clearAttributes();

  geo.addAttribute<SVector3ArrayProperty>("vertex");
  GCGeometryAttribute *vertex = geo.attribute("vertex");
  SVector3ArrayProperty *vertexData = vertex->attributeData<SVector3ArrayProperty>();

  geo.addAttribute<SVector3ArrayProperty>("normals");
  GCGeometryAttribute *normals = geo.attribute("normals");
  SVector3ArrayProperty *normalData = normals->attributeData<SVector3ArrayProperty>();

  xuint32 polys[] = { 4, 4, 4, 4, 4, 4 };
  geo.addPolygons(polys, 6);

  SVector3ArrayProperty::EigenArray arr;
  arr.resize(8,1);
  arr << XVector3D(x,y,z),
        XVector3D(-x,y,z),
        XVector3D(-x,-y,z),
        XVector3D(x,-y,z),
        XVector3D(x,y,-z),
        XVector3D(-x,y,-z),
        XVector3D(-x,-y,-z),
        XVector3D(x,-y,-z);
  vertexData->setData(arr);

  SVector3ArrayProperty::EigenArray norm;
  norm.resize(6,1);
  norm << XVector3D(1.0f, 0.0f, 0.0f),
        XVector3D(-1.0f, 0.0f, 0.0f),
        XVector3D(0.0f, 1.0f, 0.0f),
        XVector3D(0.0f, -1.0f, 0.0f),
        XVector3D(0.0f, 0.0f, 1.0f),
        XVector3D(0.0f, 0.0f, -1.0f);
  normalData->setData(norm);

  // pos
    {
    xuint32 posX[] = { 0, 3, 7, 4 };
    vertex->setPolygon(0, posX);

    xuint32 negX[] = { 1, 2, 6, 5 };
    vertex->setPolygon(1, negX);

    xuint32 posY[] = { 0, 1, 5, 4 };
    vertex->setPolygon(2, posY);

    xuint32 negY[] = { 2, 3, 7, 6 };
    vertex->setPolygon(3, negY);

    xuint32 posZ[] = { 0, 1, 2, 3 };
    vertex->setPolygon(4, posZ);

    xuint32 negZ[] = { 4, 5, 6, 7 };
    vertex->setPolygon(5, negZ);
    }

  // norm
    {
    xuint32 posX[] = { 0, 0, 0, 0 };
    normals->setPolygon(0, posX);

    xuint32 negX[] = { 1, 1, 1, 1 };
    normals->setPolygon(1, negX);

    xuint32 posY[] = { 2, 2, 2, 2 };
    normals->setPolygon(2, posY);

    xuint32 negY[] = { 3, 3, 3, 3 };
    normals->setPolygon(3, negY);

    xuint32 posZ[] = { 4, 4, 4, 4 };
    normals->setPolygon(4, posZ);

    xuint32 negZ[] = { 5, 5, 5, 5 };
    normals->setPolygon(5, negZ);
    }
  }

SPropertyInformation *GCCuboid::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCCuboid>("GCCuboid");

  GCGeometry::InstanceInformation *geoInfo = info->add(&GCCuboid::geometry, "geometry");
  geoInfo->setCompute(computeCube);

  FloatProperty::InstanceInformation *wInfo = info->add(&GCCuboid::width, "width");
  wInfo->setAffects(geoInfo);
  wInfo->setDefault(1.0f);

  FloatProperty::InstanceInformation *hInfo = info->add(&GCCuboid::height, "height");
  hInfo->setAffects(geoInfo);
  hInfo->setDefault(1.0f);

  FloatProperty::InstanceInformation *dInfo = info->add(&GCCuboid::depth, "depth");
  dInfo->setAffects(geoInfo);
  dInfo->setDefault(1.0f);

  return info;
  }

GCCuboid::GCCuboid()
  {
  }
