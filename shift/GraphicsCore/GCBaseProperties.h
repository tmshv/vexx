#ifndef GCBASEPROPERTIES_H
#define GCBASEPROPERTIES_H

#include "GCGlobal.h"
#include "sbaseproperties.h"
#include "XTransform.h"
#include "QImage"
#include "XShader.h"
#include "XGeometry.h"

namespace
{
template <typename Scalar, int Size, int Type> inline void writeValue(SSaver &s, const Eigen::Transform<Scalar, Size, Type> &t)
  {
  if(s.streamMode() == SSaver::Text)
    {
    s.textStream() << t.matrix();
    }
  else
    {
    s.binaryStream() << t.matrix();
    }
  }

template <typename Scalar, int Size, int Type> inline void readValue(SLoader &l, Eigen::Transform<Scalar, Size, Type> &t)
  {
  if(l.streamMode() == SLoader::Text)
    {
    l.textStream() >> t.matrix();
    }
  else
    {
    l.binaryStream() >> t.matrix();
    }
  }

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

void writeValue(SSaver &, const XGeometry &)
  {
  xAssertFail();
  }

void readValue(SLoader &, XGeometry &)
  {
  xAssertFail();
  }
}

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, TransformProperty, XTransform, XTransform::Identity(), 150);
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, ComplexTransformProperty, XComplexTransform, XComplexTransform(), 151);
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCRuntimeShader, XShader, XShader(), 152)
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCQImage, QImage, QImage(), 153)
DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCRuntimeGeometry, XGeometry, XGeometry(), 154)

#endif // GCBASEPROPERTIES_H
