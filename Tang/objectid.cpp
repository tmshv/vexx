#include "objectid.h"

ObjectId::ObjectId() : _data()
  {
  }

ObjectId::ObjectId(const QByteArray &data)
  {
  if(data.length() == 16)
    {
    _data = data;
    }
  }

QByteArray ObjectId::toHex() const
  {
  return _data.toHex();
  }

bool ObjectId::isValid() const
  {
  return _data.length() == 16;
  }
