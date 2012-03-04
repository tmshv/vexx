#include "MCCuboid.h"
#include "3D/Manipulators/GCDistanceManipulator.h"
#include "3D/GCTransform.h"

#include "MCPolyhedron.h"

S_IMPLEMENT_PROPERTY(MCCuboid)

SPropertyInformation *MCCuboid::createTypeInformation()
  {
  SPropertyInformation* info = SPropertyInformation::create<MCCuboid>("MCCuboid");

  GCGeometry::InstanceInformation *inst = info->add(&MCCuboid::geometry, "geometry");
  inst->setCompute(computeGeometry);

  FloatProperty::InstanceInformation *wInfo = info->add(&MCCuboid::width, "width");
  wInfo->setAffects(inst);
  wInfo->setDefault(1.0f);

  FloatProperty::InstanceInformation *hInfo = info->add(&MCCuboid::height, "height");
  hInfo->setAffects(inst);
  hInfo->setDefault(1.0f);

  FloatProperty::InstanceInformation *dInfo = info->add(&MCCuboid::depth, "depth");
  dInfo->setAffects(inst);
  dInfo->setDefault(1.0f);

  info->addInheritedInterface<MCCuboid, GCManipulatable>();

  return info;
  }

MCCuboid::MCCuboid()
  {
  }

MCCuboid::~MCCuboid()
  {
  }

void MCCuboid::computeGeometry(const SPropertyInstanceInformation*, MCCuboid* cube)
  {
  MCGeometry& geo = cube->geometry;

  float x = cube->width() / 2;
  float y = cube->height() / 2;
  float z = cube->depth() / 2;

  MCPolyhedronProperty::ComputeLock l(&geo.polygons);

  MCPolyhedron& p = *l.data();

  p.clear();

  MCHalfedgeHandle h = p.make_tetrahedron(XVector3D(x, -y, -z),
                                          XVector3D(-x, -y, z),
                                          XVector3D(-x, -y, -z),
                                          XVector3D(-x, y, -z));

  MCHalfedgeHandle g = h->next()->opposite()->next();
  p.split_edge( h->next());
  p.split_edge( g->next());
  p.split_edge( g);
  h->next()->vertex()->point()     = XVector3D(x, -y, z);
  g->next()->vertex()->point()     = XVector3D(-x, y, z);
  g->opposite()->vertex()->point() = XVector3D(x, y, -z);
  MCHalfedgeHandle f = p.split_facet( g->next(), g->next()->next()->next());
  MCHalfedgeHandle e = p.split_edge( f);
  e->vertex()->point() = XVector3D(x, y, z);
  p.split_facet( e, f->next()->next());

  p.computeNormals();
  }

void MCCuboid::addManipulators(SPropertyArray *a, const GCTransform *tr)
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
    }
  }
