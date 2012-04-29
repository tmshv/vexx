#ifndef MCKERNEL_H
#define MCKERNEL_H

#include "MCCGALSetup.h"
#include "XGlobal"
#include "XVector3D"
#include "XPlane.h"
#include <CGAL/Cartesian/Cartesian_base.h>
#include <CGAL/Handle_for.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Simple_cartesian<float>::Point_3 F3;
typedef CGAL::Simple_cartesian<double>::Point_3 D3;
typedef CGAL::Exact_predicates_inexact_constructions_kernel::Point_3 EPICK3;

template <typename Kernel_> struct MCKernelBase : public CGAL::Cartesian_base<Kernel_, xReal>
  {
  typedef xReal                                       RT;
  typedef xReal                                       FT;

  typedef XVector2D                                   Point_2;
  typedef XVector2D                                   Vector_2;
  typedef XVector2D                                   Direction_2;
  typedef CGAL::SegmentC2<Kernel>                     Segment_2;
  typedef CGAL::LineC2<Kernel>                        Line_2;
  typedef CGAL::RayC2<Kernel>                         Ray_2;
  typedef CGAL::TriangleC2<Kernel>                    Triangle_2;
  typedef CGAL::CircleC2<Kernel>                      Circle_2;
  typedef CGAL::Iso_rectangleC2<Kernel>               Iso_rectangle_2;
  typedef CGAL::Aff_transformationC2<Kernel>          Aff_transformation_2;

  typedef const xReal * Cartesian_const_iterator_3;
  typedef xReal * Cartesian_iterator_3;

  class Point_3 : public XVector3D
    {
  public:
    Point_3() : XVector3D(XVector3D::Zero()), _w(1.0f) { }
    Point_3(float x, float y, float z) : XVector3D(x, y, z), _w(1.0f) { }
    Point_3(float x, float y, float z, float w) : XVector3D(x, y, z), _w(w) { }
    Point_3(const XVector3D &p) : XVector3D(p), _w(1.0f) { }
    Point_3(const F3 &p) : XVector3D(p.x(), p.y(), p.z()), _w(1.0f) { }
    Point_3(const D3 &p) : XVector3D(p.x(), p.y(), p.z()), _w(1.0f) { }
    Point_3(const EPICK3 &p) : XVector3D(p.x(), p.y(), p.z()), _w(1.0f) { }
    Point_3(CGAL::Origin) : XVector3D(XVector3D::Zero()), _w(1.0f) { }
    Point_3(const Point_3 &p) : XVector3D(p), _w(p._w) { }
    Point_3& operator=(const XVector3D& p) { *((XVector3D*)this) = p; return *this; }

    const xReal *cartesian_begin() const { return data(); }
    const xReal *cartesian_end() const { return data() + 3; }

    xReal *cartesian_begin() { return data(); }
    xReal *cartesian_end() { return data() + 3; }

    const xReal &hx() const { return x(); }
    const xReal &hy() const { return y(); }
    const xReal &hz() const { return z(); }

  private:
    xReal _w;
    };

  class Vector_3 : public XVector3D
    {
  public:
    Vector_3() : XVector3D(XVector3D::Zero()) { }
    Vector_3(float x, float y, float z) : XVector3D(x, y, z) { }
    Vector_3(const XVector3D &p) : XVector3D(p) { }

    xReal squared_length() const { return squaredNorm(); }
    };

  typedef CGAL::LineC3<Kernel>                        Line_3;
  class Direction_3 : public XVector3D
    {
  public:
    Direction_3() : XVector3D(XVector3D::Zero()) { }
    Direction_3(float x, float y, float z) : XVector3D(x, y, z) { }
    Direction_3(const XVector3D &p) : XVector3D(p) { }
    Direction_3(const Line_3 &p) : XVector3D(p.direction()) { }

    const xReal &dx() const { return x(); }
    const xReal &dy() const { return y(); }
    const xReal &dz() const { return z(); }
    };

  typedef XPlane                                      Plane_3;
  typedef CGAL::RayC3<Kernel>                         Ray_3;
  typedef CGAL::SegmentC3<Kernel>                     Segment_3;
  typedef CGAL::TriangleC3<Kernel>                    Triangle_3;
  typedef CGAL::TetrahedronC3<Kernel>                 Tetrahedron_3;
  typedef CGAL::Iso_cuboidC3<Kernel>                  Iso_cuboid_3;
  typedef CGAL::SphereC3<Kernel>                      Sphere_3;
  typedef CGAL::CircleC3<Kernel>                      Circle_3;
  typedef CGAL::Aff_transformationC3<Kernel>          Aff_transformation_3;

  // The mechanism that allows to specify reference-counting or not.
  template <typename T> struct Handle { typedef T   type; };

  template <typename Kernel2>
  struct Base { typedef MCKernelBase<Kernel2>  Type; };

  typedef Kernel_ K;
#define CGAL_Kernel_pred(Y,Z) typedef CGAL::CartesianKernelFunctors::Y<K> Y; \
                              Y Z() const { return Y(); }
#define CGAL_Kernel_cons(Y,Z) CGAL_Kernel_pred(Y,Z)

#include <CGAL/Kernel/interface_macros.h>
  };

struct MCKernel : public CGAL::Type_equality_wrapper<MCKernelBase<MCKernel>, MCKernel>
  {
  };

#endif // MCKERNEL_H
