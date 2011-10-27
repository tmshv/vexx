#ifndef GCQIMAGE_H
#define GCQIMAGE_H

#include "GCGlobal.h"
#include "sbaseproperties.h"
#include "QImage"

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
}

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCQImage, QImage, QImage())

#endif // GCQIMAGE_H
