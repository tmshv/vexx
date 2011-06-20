#include "GCQImage.h"
#include "sdatabase.h"


QTextStream &operator <<(QTextStream &str, const QImage &data)
  {
  QByteArray arr;
  // approximate hopeful size?
  arr.resize(data.byteCount() + 64);
  QDataStream dStr(&arr, QIODevice::WriteOnly);
  dStr << data;

  return str << data;
  }

QTextStream &operator >>(QTextStream &str, QImage &data)
  {
  QByteArray arr;
  str >> arr;

  QDataStream dStr(&arr, QIODevice::ReadOnly);
  dStr >> data;
  return str;
  }

IMPLEMENT_POD_PROPERTY(GCQImage, QImage)


void GCQImage::assignProperty(const SProperty *f, SProperty *t)
  {
  SProfileFunction
  GCQImage *to = t->uncheckedCastTo<GCQImage>();

  const GCQImage *qImageProp = f->castTo<GCQImage>();
  if(qImageProp)
    {
    to->assign(qImageProp->value());
    }
  }
