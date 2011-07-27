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
  SVector3ArrayProperty *vertex = geo.attribute<SVector3ArrayProperty>("vertex");

  SVector3ArrayProperty::EigenArray arr;
  arr << XVector3D(x,y,z),
        XVector3D(-x,y,z),
        XVector3D(-x,-y,z),
        XVector3D(x,-y,z),
        XVector3D(x,y,-z),
        XVector3D(-x,y,-z),
        XVector3D(-x,-y,-z),
        XVector3D(x,-y,-z);

  vertex->setData(arr);

  xuint32 polys[] = { 4, 4, 4, 4, 4, 4 };
  geo.polygons.addPolygons(polys, 6);

  xuint32 posZ[] = { 0, 1, 2, 3 };
  geo.polygons.setPolygon(0, posZ);

  xuint32 negZ[] = { 4, 5, 6, 7 };
  geo.polygons.setPolygon(1, negZ);
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
