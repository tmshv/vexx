#ifndef MCPOLYHEDRON_H
#define MCPOLYHEDRON_H

#include "MeshCore.h"
#include "MCCGALSetup.h"
#include "MCKernel.h"
#include "CGAL/Polyhedron_3.h"
#include "XProperty"
#include "sbaseproperties.h"

template <class Refs, typename Pt> struct MCVertex : public CGAL::HalfedgeDS_vertex_base<Refs>
  {
  MCVertex()
    {
    }

  MCVertex(const Pt& pt) : _point(pt)
    {
    }

  typedef Pt Point;

XProperties:
  XRefProperty(Point, point);
  };

class MCPolyhedronItems
  {
public:
  template <class Refs, class Traits>
  struct Vertex_wrapper
    {
    typedef typename Traits::Point_3 Point;
    typedef MCVertex<Refs, Point> Vertex;
    };

  template <class Refs, class Traits>
  struct Halfedge_wrapper
    {
    typedef CGAL::HalfedgeDS_halfedge_base<Refs> Halfedge;
    };

  template <class Refs, class Traits>
  struct Face_wrapper
    {
    typedef typename Traits::Plane_3 Plane;
    typedef CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Plane> Face;
    };
  };

typedef CGAL::Polyhedron_3<MCKernel, MCPolyhedronItems> MCPolyhedron;
typedef MCPolyhedron::Halfedge_handle MCHalfedgeHandle;

DEFINE_POD_PROPERTY(MESHCORE_EXPORT, MCPolyhedronProperty, MCPolyhedron, MCPolyhedron(), 2000);

#endif // MCPOLYHEDRON_H
