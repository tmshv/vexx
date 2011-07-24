#ifndef OBJECTID_H
#define OBJECTID_H

#include "XGlobal"
#include "QByteArray"

class ObjectId
  {
public:
  ObjectId();
  ObjectId(const QByteArray &data);

  QByteArray toHex() const;
  bool isValid() const;

private:
  QByteArray _data;
  };

#endif // OBJECTID_H
