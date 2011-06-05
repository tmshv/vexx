#ifndef XTRANSFORM_H
#define XTRANSFORM_H

#include "XGlobal"
#include "XList"
#include "Eigen/Geometry"
#include "XVector3D"
#include "XMatrix4x4"

typedef Eigen::Affine3f XTransform;
typedef Eigen::Projective3f XComplexTransform;

namespace XTransformUtilities
{
XTransform lookAt(const XVector3D &eye, const XVector3D &aim, const XVector3D &up);
XComplexTransform perspective(xReal angle, xReal aspect, xReal nearPlane, xReal farPlane);
}

typedef XList <XTransform> XTransformList;

#endif // XTRANSFORM_H
