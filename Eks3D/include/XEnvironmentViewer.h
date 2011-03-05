#ifndef XENVIRONMENTVIEWER_H
#define XENVIRONMENTVIEWER_H

#include "X3DGlobal.h"
#include "XVector3D"

class QPoint;
class XFrustum;

class EKS3D_EXPORT XEnvironmentViewer
{
public:
    XEnvironmentViewer();
    virtual ~XEnvironmentViewer();

    virtual XVector3D screenToWorld(const QPoint &pt) const = 0;
    virtual XVector3D position() const = 0;
    virtual xReal farClipPlane() const = 0;
    virtual xReal nearClipPlane() const = 0;
    virtual XFrustum frustum() const = 0;
};

#endif // XENVIRONMENTVIEWER_H
