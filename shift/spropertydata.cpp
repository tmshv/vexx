#include "spropertydata.h"

SPropertyData::SPropertyData()
  {
  }

SPropertyData::SPropertyData(const QByteArray &in) : _value(in)
  {
  }

void SPropertyData::setName(const QString &val)
  {
  _name = val;
  }

void SPropertyData::setValue(const QByteArray &val)
  {
  _value = val;
  }

void SPropertyData::append(const SPropertyData &data)
  {
  _children << data;
  }

void SPropertyData::appendAttribute(const QString &a, const QByteArray &b)
  {
  _attributes.insert(a, b);
  }

bool SPropertyData::hasAttribute(const QString &a) const
  {
  return _attributes.contains(a);
  }

QDataStream &operator<<(QDataStream &str, const SPropertyData &d)
  {
  return str << d._name << d._value << d._attributes << d._children;
  }


QDataStream &operator>>(QDataStream &str, SPropertyData &d)
  {
  return str >> d._name >> d._value >> d._attributes >> d._children;
  }
