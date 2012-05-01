#ifndef GCBASEPROPERTIES_H
#define GCBASEPROPERTIES_H

#include "GCGlobal.h"
#include "sbaseproperties.h"
#include "XTransform.h"
#include "QImage"
#include "XShader.h"
#include "XGeometry.h"
#include "XCuboid.h"

namespace
{
QTextStream &operator <<(QTextStream &str, const QImage &data)
  {
  QByteArray arr;
  // approximate hopeful size?
  arr.resize(data.byteCount() + 64);
  QDataStream dStr(&arr, QIODevice::WriteOnly);
  dStr << data;

  return str << arr.toHex();
  }

QTextStream &operator >>(QTextStream &str, QImage &data)
  {
  QByteArray arr;
  str >> arr;
  arr = QByteArray::fromHex(arr);

  QDataStream dStr(&arr, QIODevice::ReadOnly);
  dStr >> data;
  return str;
  }

QTextStream &operator<<(QTextStream &s, const XGeometry &)
  {
  xAssertFail();
  return s;
  }

QTextStream &operator>>(QTextStream &s, const XGeometry &)
  {
  xAssertFail();
  return s;
  }

}

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, TransformProperty, XTransform, XTransform::Identity(), 150);
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, ComplexTransformProperty, XComplexTransform, XComplexTransform(), 151);
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCRuntimeShader, XShader, XShader(), 152)
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCQImage, QImage, QImage(), 153)
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCRuntimeGeometry, XGeometry, XGeometry(), 154)
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCBoundingBox, XCuboid, XCuboid(), 155)

#endif // GCBASEPROPERTIES_H
