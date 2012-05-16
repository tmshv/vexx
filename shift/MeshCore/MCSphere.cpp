#include "MCSphere.h"
#include "spropertyinformationhelpers.h"
#include "3D/GCTransform.h"
#include "3D/Manipulators/GCDistanceManipulator.h"

S_IMPLEMENT_PROPERTY(MCSphere, MeshCore)

void MCSphere::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    MCGeometry::InstanceInformation* geomInst = info->child(&MCSphere::geometry);

    FloatProperty::InstanceInformation* radInst = info->add(&MCSphere::radius, "radius");
    radInst->setAffects(geomInst);
    radInst->setDefault(1.0f);
    }

  if(data.registerInterfaces)
    {
    info->addInheritedInterface<MCSphere, GCManipulatable>();
    }
  }

MCSphere::MCSphere()
  {
  }

float MCSphere::evaluate(const XVector3D &p) const
  {
  float r = radius();
  const float x2=p.x()*p.x(), y2=p.y()*p.y(), z2=p.z()*p.z();
  return x2+y2+z2 - (r*r);
  }

void MCSphere::addManipulators(SPropertyArray *a, const GCTransform *tr)
  {
  // radius
    {
    GCDistanceManipulator *manip = a->add<GCDistanceManipulator>();

    manip->lockDirection = XVector3D(1.0f, 0.0f, 0.0f);
    manip->lockMode = GCDistanceManipulator::Linear;

    radius.connect(&manip->distance);

    manip->addDriven(&radius);

    if(tr)
      {
      tr->transform.connect(&manip->worldCentre);
      }
    }
  }
