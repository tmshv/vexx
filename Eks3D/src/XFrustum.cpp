#include "XFrustum.h"
#include "XCuboid.h"
#include "XPlane.h"

XFrustum::XFrustum( )
  {
  }

XFrustum::XFrustum( const XVector3D &point, const XVector3D &look, const XVector3D &across,
    const XVector3D &up, float viewAngle, float aspect, float nearPlane, float farPlane )
  {
  XVector3D lookNorm = look.normalized();
  XVector3D acrossNorm = across.normalized();
  XVector3D upNorm = up.normalized();
  float fovUpY = tan(X_DEGTORAD(viewAngle)/2.0f);
  float fovUpX = tan(X_DEGTORAD(viewAngle*aspect)/2.0f);

  // near plane
  _planes[NearPlane] = XPlane(point+(lookNorm*nearPlane), -lookNorm);
  // far plane
  _planes[FarPlane] = XPlane(point+(lookNorm*farPlane), lookNorm);

  // top plane
  _planes[TopPlane] = XPlane(point, XVector3D::crossProduct(lookNorm+(fovUpY*upNorm), across) );
  // bottom plane
  _planes[BottomPlane] = XPlane(point, XVector3D::crossProduct(across, lookNorm-(fovUpY*upNorm)) );

  // left plane
  _planes[LeftPlane] = XPlane(point, XVector3D::crossProduct(up, lookNorm+(fovUpX*acrossNorm)) );
  // right plane
  _planes[RightPlane] = XPlane(point, XVector3D::crossProduct(lookNorm-(fovUpX*acrossNorm), up) );

  }

XFrustum::IntersectionResult XFrustum::intersects( const XCuboid &cuboid ) const
  {
  XFrustum::IntersectionResult ret = Inside;
  XVector3D vec;
  for( size_t i=0; i<6; ++i )
    {
    vec.setX( (_planes[i].normal().x() >= 0.0f) ? cuboid.maximum().x() : cuboid.minimum().x() );
    vec.setY( (_planes[i].normal().y() >= 0.0f) ? cuboid.maximum().y() : cuboid.minimum().y() );
    vec.setZ( (_planes[i].normal().z() >= 0.0f) ? cuboid.maximum().z() : cuboid.minimum().z() );

    if( _planes[i].distanceToPlane(vec) < 0.0f )
      {
      return Outside;
      }

    vec.setX( (_planes[i].normal().x() >= 0.0f) ? cuboid.minimum().x() : cuboid.maximum().x() );
    vec.setY( (_planes[i].normal().y() >= 0.0f) ? cuboid.minimum().y() : cuboid.maximum().y() );
    vec.setZ( (_planes[i].normal().z() >= 0.0f) ? cuboid.minimum().z() : cuboid.maximum().z() );

    if( _planes[i].distanceToPlane(vec) < 0.0f )
      {
      ret = Intersects;
      }
    }
  return ret;
  }
