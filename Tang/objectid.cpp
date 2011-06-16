#include "objectid.h"

ObjectId::ObjectId(const xuint8 *data) : _data((const char *)data, 16)
  {
  }

QByteArray ObjectId::toHex() const
  {
  return _data.toHex();
  }
