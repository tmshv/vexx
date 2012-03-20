#include "GCSphere.h"
#include "sarrayproperty.h"
#include "3D/Manipulators/GCDistanceManipulator.h"

S_IMPLEMENT_PROPERTY(GCSphere)

void computeSphere(const SPropertyInstanceInformation *, GCSphere *sph)
  {
  GCProfileFunction
  GCGeometry &geo = sph->geometry;

  float radius = sph->radius();

  geo.clearAttributes();

  geo.addAttribute<SVector3ArrayProperty>("vertex");
  GCGeometryAttribute *vertex = geo.attribute("vertex");
  SVector3ArrayProperty *vertexData = vertex->attributeData<SVector3ArrayProperty>();

  geo.addAttribute<SVector3ArrayProperty>("normals");
  GCGeometryAttribute *normals = geo.attribute("normals");
  SVector3ArrayProperty *normalData = normals->attributeData<SVector3ArrayProperty>();

  const xsize uDivisions = 16; // width
  const xsize vDivisions = 12; // height
  xAssert(uDivisions > 2, uDivisions);
  xAssert(vDivisions > 1, vDivisions);

  XVector<xuint32> polys;
  polys.reserve(uDivisions * vDivisions);
  for(xsize i=0; i<(vDivisions-1); ++i)
    {
    xuint32 size = 4;
    if(i == 0 || i == (vDivisions-2))
      {
      size = 3;
      }

    for(xsize j=0; j<uDivisions; ++j)
      {
      polys << size;
      }
    }

  geo.addPolygons(polys);

  xsize numPoints = (uDivisions * (vDivisions-2)) + 2;
  SVector3ArrayProperty::EigenArray pos;
  SVector3ArrayProperty::EigenArray norm;
  pos.resize(numPoints,1);
  norm.resize(numPoints,1);
  xsize posIndex = 0;
  xsize normIndex = 0;

  for(xsize v=1; v<(vDivisions-1); ++v)
    {
    float yPercent = (float)v/(float)(vDivisions-1);
    float yNormalisedHeight = cos(yPercent*M_PI);
    float yRadiusMultiplier = sin(yPercent*M_PI);
    float yPos = yNormalisedHeight * radius;

    for(xsize u=0; u<uDivisions; ++u)
      {
      float uAngle = M_PI * 2.0f * (float)u/(float)uDivisions;

      float xPos = sin(uAngle) * radius * yRadiusMultiplier;
      float zPos = cos(uAngle) * radius * yRadiusMultiplier;

      pos(posIndex++,0) = XVector3D(xPos, yPos, zPos);
      norm(normIndex++,0) = XVector3D(xPos, yPos, zPos).normalized();
      }
    }

  pos(posIndex++, 0) = XVector3D(0.0f, radius, 0.0f);
  pos(posIndex++, 0) = XVector3D(0.0f, -radius, 0.0f);
  norm(normIndex++, 0) = XVector3D(0.0f, 1.0f, 0.0f);
  norm(normIndex++, 0) = XVector3D(0.0f, -1.0f, 0.0f);

  xAssert(posIndex == normIndex, posIndex, normIndex);
  xAssert(posIndex == pos.rows(), posIndex, pos.rows());

  vertexData->setData(pos);
  normalData->setData(norm);

  xsize polyId = 0;
  for(xsize i=0; i<(vDivisions-1); ++i)
    {
    xsize rowStart = (i-1) * uDivisions;
    xsize nextRowStart = i * uDivisions;

    if(i == 0)
      {
      for(xsize j=0; j<uDivisions; ++j)
        {
        xuint32 poly[] = { nextRowStart+j, (nextRowStart+j+1)%uDivisions,
                         numPoints-2 };

        vertex->setPolygon(polyId, poly, 3);
        normals->setPolygon(polyId++, poly, 3);
        }
      }
    else if(i == (vDivisions-2))
      {
      for(xsize j=0; j<uDivisions; ++j)
        {
        xuint32 poly[] = { rowStart+j, rowStart+((j+1)%uDivisions),
                         numPoints-1 };

        vertex->setPolygon(polyId, poly, 3);
        normals->setPolygon(polyId++, poly, 3);
        }
      }
    else
      {
      for(xsize j=0; j<uDivisions; ++j)
        {
        xuint32 poly[] = { rowStart+j, rowStart+((j+1)%uDivisions),
                         nextRowStart+((j+1)%uDivisions), nextRowStart+j };

        vertex->setPolygon(polyId, poly, 4);
        normals->setPolygon(polyId++, poly, 4);
        }
      }
    }
  }

SPropertyInformation *GCSphere::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCSphere>("GCSphere");

  GCGeometry::InstanceInformation *geoInfo = info->add(&GCSphere::geometry, "geometry");
  geoInfo->setCompute(computeSphere);

  FloatProperty::InstanceInformation *wInfo = info->add(&GCSphere::radius, "radius");
  wInfo->setAffects(geoInfo);
  wInfo->setDefault(1.0f);

  info->addInheritedInterface<GCSphere, GCManipulatable>();

  return info;
  }

GCSphere::GCSphere()
  {
  }

void GCSphere::addManipulators(SPropertyArray *, const GCTransform *)
  {
  /*
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

  // Z
    {
    GCDistanceManipulator *manip = a->add<GCDistanceManipulator>();

    manip->lockDirection = XVector3D(0.0f, 0.0f, 1.0f);
    manip->lockMode = GCDistanceManipulator::Linear;
    manip->scaleFactor = 0.5f;

    depth.connect(&manip->distance);

    manip->addDriven(&depth);

    if(tr)
      {
      tr->transform.connect(&manip->worldCentre);
      }
    }*/
  }
