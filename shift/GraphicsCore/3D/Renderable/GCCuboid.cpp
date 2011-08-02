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

  xuint32 polys[] = { 4, 4, 4, 4, 4, 4 };
  geo.addPolygons(polys, 6);

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
