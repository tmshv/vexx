#include "MCDensityShape.h"
#include "MCPolyhedron.h"

#include <CGAL/IO/output_surface_facets_to_polyhedron.h>
#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/Complex_2_in_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>

S_IMPLEMENT_PROPERTY(MCDensityShape)

SPropertyInformation *MCDensityShape::createTypeInformation()
  {
  SPropertyInformation* info = SPropertyInformation::create<MCDensityShape>("MCDensityShape");

  GCGeometry::InstanceInformation *inst = info->add(&MCDensityShape::geometry, "geometry");
  inst->setCompute(computeGeometry);

  return info;
  }

MCDensityShape::MCDensityShape()
  {
  }

MCDensityShape::~MCDensityShape()
  {
  }

// default triangulation for Surface_mesher
typedef CGAL::Surface_mesh_default_triangulation_3 Tr;

// c2t3
typedef CGAL::Complex_2_in_triangulation_3<Tr> C2t3;

typedef Tr::Geom_traits GT;
typedef GT::Sphere_3 Sphere_3;
typedef GT::Point_3 Point_3;
typedef GT::FT FT;

float MCDensityShape::evaluate(const XVector3D &) const
  {
  return 0;
  }

void MCDensityShape::computeGeometry(const SPropertyInstanceInformation*, MCDensityShape* shape)
  {
  MCGeometry& geo = shape->geometry;

  MCPolyhedronProperty::ComputeLock l(&geo.polygons);

  MCPolyhedron& p = *l.data();

  p.clear();

  Tr tr;            // 3D-Delaunay triangulation
  C2t3 c2t3 (tr);   // 2D-complex in 3D-Delaunay triangulation

  class Evaluator
    {
  public:
    MCDensityShape* _this;
    Evaluator(MCDensityShape* s) : _this(s)
      {
      }

    FT operator()(Point_3 pt) const
      {
      XVector3D vPt(pt.x(), pt.y(), pt.z());
      float val = _this->evaluate(vPt);
      return FT(val);
      }
    };
  typedef CGAL::Implicit_surface_3<GT, Evaluator> Surface_3;
  Evaluator evaluator(shape);

  float radius = shape->boundingRadius();

  // defining the surface
  Surface_3 surface(evaluator,             // pointer to function
                    Sphere_3(CGAL::ORIGIN, radius*radius)); // bounding sphere
  // Note that "2." above is the *squared* radius of the bounding sphere!

  // defining meshing criteria
  CGAL::Surface_mesh_default_criteria_3<Tr> criteria(shape->minimumSurfaceAngle(),  // angular bound
                                                     shape->maximumApproximateTriangleSize(),  // radius bound
                                                     shape->maximumApproximateTriangleSize()); // distance bound
  // meshing surface
  CGAL::make_surface_mesh(c2t3, surface, criteria, CGAL::Non_manifold_tag());

  CGAL::output_surface_facets_to_polyhedron(c2t3, p);

  p.computeNormals();
  }
