#ifndef OBJECTID_H
#define OBJECTID_H

#include "XGlobal"
#include "QByteArray"

class ObjectId
  {
public:
  ObjectId(const xuint8 *data);

  QByteArray toHex() const;

private:
  QByteArray _data;
  };

#endif // OBJECTID_H
