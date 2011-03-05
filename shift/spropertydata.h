#ifndef SENTITYDATA_H
#define SENTITYDATA_H

#include "sglobal.h"
#include "XList"
#include "QString"
#include "XVariant"

class SHIFT_EXPORT SPropertyData
  {
public:
  enum Mode
    {
    Ascii,
    Binary
    };

  SPropertyData();
  SPropertyData(const QByteArray &);

  void setValue(const QByteArray &);
  void setName(const QString &);

  QByteArray value() const {return _value;}
  QString name() const {return _name;}

  const XList<SPropertyData> &childData() const {return _children;}
  QByteArray attribute(const QString &n) const {return _attributes[n];}

  void append(const SPropertyData &);
  void appendAttribute(const QString &, const QByteArray &);
  bool hasAttribute(const QString &a) const;

  friend SHIFT_EXPORT QDataStream &operator<<(QDataStream &str, const SPropertyData &);
  friend SHIFT_EXPORT QDataStream &operator>>(QDataStream &str, SPropertyData &);

private:
  XList <SPropertyData> _children;
  XMap <QString, QByteArray> _attributes;
  QByteArray _value;
  QString _name;
  };


#endif // SENTITYDATA_H
