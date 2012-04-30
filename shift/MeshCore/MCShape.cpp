#include "MCShape.h"
#include "XRenderer.h"
#include "XLine.h"
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_polyhedron_triangle_primitive.h>

S_IMPLEMENT_PROPERTY(MCShape)

void MCShape::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&MCShape::geometry, "geometry");
    }
  }

MCShape::MCShape()
  {
  }

void MCShape::render(XRenderer *r) const
  {
  r->drawGeometry(geometry.runtimeGeometry());
  }

typedef CGAL::AABB_polyhedron_triangle_primitive<MCKernel, MCPolyhedron> Primitive;
typedef CGAL::AABB_traits<MCKernel, Primitive> Traits;
typedef CGAL::AABB_tree<Traits> Tree;
typedef Tree::Object_and_primitive_id Object_and_primitive_id;
typedef Tree::Primitive_id Primitive_id;

void MCShape::intersect(const XLine& line, Selector *s)
  {
  // need to be able to get a non const iterator for cgal.
  MCPolyhedron& polyhedron = const_cast<MCPolyhedron&>(geometry.polygons());

  // constructs AABB tree
  Tree tree(polyhedron.facets_begin(), polyhedron.facets_end());
  tree.accelerate_distance_queries();

  MCKernel::Point_3 start = line.position();
  MCKernel::Point_3 end = XVector3D(line.position() + line.direction());

  // constructs segment query
  MCKernel::Segment_3 segment(start, end);

  class Inserter
    {
  public:
    Inserter(Selector *s, MCShape *shape) : _s(s), _shape(shape)
      {
      }
    Inserter &operator*()
      {
      return *this;
      }
    Inserter& operator=(const Object_and_primitive_id& object)
      {
      if(object.first.is<MCKernel::Point_3>())
        {
        const MCKernel::Point_3& pt4D = CGAL::object_cast<MCKernel::Point_3>(object.first);
        const MCPolyhedron::Face& prim = *object.second;
        const XVector3D pt = pt4D / pt4D.hw();
        const XVector3D normal = prim.plane().normal();

        _s->onHit(pt, normal, _shape);
        }

      return *this;
      }
    Inserter operator++()
      {
      return *this;
      }
    Inserter operator++(int)
      {
      return *this;
      }

  private:
    Selector *_s;
    MCShape *_shape;
    };
  Inserter insertor(s, this);

  // computes all intersections with segment query (as pairs object - primitive_id)
  tree.all_intersections(segment, insertor);
  }

void MCShape::intersect(const XFrustum& frus, Selector *)
  {
  }
