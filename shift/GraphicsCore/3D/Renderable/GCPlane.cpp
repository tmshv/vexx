#include "GCPlane.h"
#include "sarrayproperty.h"
#include "3D/Manipulators/GCDistanceManipulator.h"

S_IMPLEMENT_PROPERTY(GCPlane)

void computePlane(const SPropertyInstanceInformation *, GCPlane *plane)
  {
  GCProfileFunction
  GCGeometry &geo = plane->geometry;

  float x = plane->width() / 2;
  float y = plane->height() / 2;

  geo.clearAttributes();

  geo.addAttribute<SVector3ArrayProperty>("vertex");
  GCGeometryAttribute *vertex = geo.attribute("vertex");
  SVector3ArrayProperty *vertexData = vertex->attributeData<SVector3ArrayProperty>();

  geo.addAttribute<SVector3ArrayProperty>("normals");
  GCGeometryAttribute *normals = geo.attribute("normals");
  SVector3ArrayProperty *normalData = normals->attributeData<SVector3ArrayProperty>();

  xuint32 polys[] = { 4 };
  geo.addPolygons(polys, 1);

  SVector3ArrayProperty::EigenArray arr;
  arr.resize(4,1);
  arr << XVector3D(x,y,0),
        XVector3D(-x,y,0),
        XVector3D(-x,-y,0),
        XVector3D(x,-y,0);
  vertexData->setData(arr);

  SVector3ArrayProperty::EigenArray norm;
  norm.resize(1,1);
  norm << XVector3D(0.0f, 0.0f, 1.0f);
  normalData->setData(norm);

  // pos
  xuint32 pos[] = { 0, 1, 2, 3 };
  vertex->setPolygon(0, pos, 4);

  // norm
  xuint32 normIdx[] = { 0, 0, 0, 0 };
  normals->setPolygon(0, normIdx, 4);
  }

SPropertyInformation *GCPlane::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCPlane>("GCPlane");

  GCGeometry::InstanceInformation *geoInfo = info->add(&GCPlane::geometry, "geometry");
  geoInfo->setCompute(computePlane);

  FloatProperty::InstanceInformation *wInfo = info->add(&GCPlane::width, "width");
  wInfo->setAffects(geoInfo);
  wInfo->setDefault(1.0f);

  FloatProperty::InstanceInformation *hInfo = info->add(&GCPlane::height, "height");
  hInfo->setAffects(geoInfo);
  hInfo->setDefault(1.0f);

  info->addInheritedInterface<GCPlane, GCManipulatable>();

  return info;
  }

GCPlane::GCPlane()
  {
  }

void GCPlane::addManipulators(SPropertyArray *a, const GCTransform *tr)
  {
  // X
    {
    GCDistanceManipulator *manip = a->add<GCDistanceManipulator>();

    manip->lockDirection = XVector3D(1.0f, 0.0f, 0.0f);
    manip->lockMode = GCDistanceManipulator::Linear;
    manip->scaleFactor = 0.5f;

    width.connect(&manip->distance);

    manip->addDriven(&width);

    if(tr)
      {
      tr->transform.connect(&manip->worldCentre);
      }
    }

  // Y
    {
    GCDistanceManipulator *manip = a->add<GCDistanceManipulator>();

    manip->lockDirection = XVector3D(0.0f, 1.0f, 0.0f);
    manip->lockMode = GCDistanceManipulator::Linear;
    manip->scaleFactor = 0.5f;

    height.connect(&manip->distance);

    manip->addDriven(&height);

    if(tr)
      {
      tr->transform.connect(&manip->worldCentre);
      }
    }
  }
