#ifndef MCKERNEL_H
#define MCKERNEL_H

#include "MCCGALSetup.h"
#include "XGlobal"
#include "XVector3D"
#include <CGAL/Cartesian/Cartesian_base.h>
#include <CGAL/Cartesian/Cartesian_base.h>
#include <CGAL/Handle_for.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>

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

  typedef XVector3D                                   Point_3;
  typedef XVector3D                                   Vector_3;
  typedef XVector3D                                   Direction_3;
  typedef CGAL::LineC3<Kernel>                        Line_3;
  typedef CGAL::PlaneC3<Kernel>                       Plane_3;
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
