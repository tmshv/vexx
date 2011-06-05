#include "XEnvironmentArea.h"

XEnvironmentArea::XEnvironmentArea() : XInitProperty(visBoxWidth, 0),
    XInitProperty(visBoxHeight, 0), XInitProperty(visBoxDepth, 0)
  {
  }

void XEnvironmentArea::save(QDataStream &stream) const
  {
  stream << bounds() << childAreas() << shadingGroups() << visBoxWidth() << visBoxHeight() << visBoxDepth();
  }

void XEnvironmentArea::restore(QDataStream &stream)
  {
  stream >> bounds() >> childAreas() << shadingGroups() << _visBoxWidth << _visBoxHeight << _visBoxDepth;
  }

xuint32 XEnvironmentArea::getVisBoxID(xuint32 w, xuint32 h, xuint32 d) const
  {
  return (PermanentVisBox+1) + d + (h*visBoxDepth()) + (w*visBoxDepth()*visBoxHeight());
  }

void XEnvironmentArea::saveVisBox(QDataStream &stream, xuint32 id) const
  {
  xAssert(spatialVisBoxes().contains(id));
  stream << spatialVisBoxes()[id];
  }

void XEnvironmentArea::restoreVisBox(QDataStream &stream, xuint32 id)
  {
  stream >> spatialVisBoxes()[id];
  }

QDataStream &operator<<(QDataStream &stream, const XEnvironmentArea::ShadingGroup &s)
  {
  return stream << s._shader << s._meshes;
  }

QDataStream &operator>>(QDataStream &stream, XEnvironmentArea::ShadingGroup &s)
  {
  return stream >> s._shader >> s._meshes;
  }

QDataStream &operator<<(QDataStream &stream, const XEnvironmentArea::MeshPair &m)
  {
  return stream << m._mesh << m._transform.matrix();
  }

QDataStream &operator>>(QDataStream &stream, XEnvironmentArea::MeshPair &m)
  {
  return stream >> m._mesh >> m._transform.matrix();
  }
