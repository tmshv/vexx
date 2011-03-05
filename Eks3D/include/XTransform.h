#ifndef XTRANSFORM_H
#define XTRANSFORM_H

#include "XMatrix4x4"
#include "XVector3D"
#include "XVector4D"
#include "XList"

class XTransform : public XMatrix4x4
  {
public:
  XTransform()
    {
    }
  XTransform( const XVector3D &pos )
    {
    translate(pos);
    }
  XTransform( const XMatrix4x4 &cpy ) : XMatrix4x4(cpy)
    {
    }
  XTransform &operator=( const XMatrix4x4 &cpy )
    {
    return *this = XTransform(cpy);
    }
  XVector3D translation() const
    {
    return column(3).toVector3D();
    }
  void setTranslation(const XVector3D &in)
    {
    setColumn(3, XVector4D(in, 1.0f));
    }
  };

typedef XList <XTransform> XTransformList;

#endif // XTRANSFORM_H
