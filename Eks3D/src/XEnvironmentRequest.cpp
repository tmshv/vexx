#include "XEnvironmentRequest.h"

XEnvironmentRequest::XEnvironmentRequest() : _type(0), _ID(-1), _subType(0), _requestID(0)
  {
  }

XEnvironmentRequest::XEnvironmentRequest(xuint16 type, ItemID id, xuint16 subType, xuint32 rID) : _type(type), _ID(id), _subType(subType), _requestID(rID)
  {
  }

bool XEnvironmentRequest::hasExtraData() const
  {
  return !_extraData.isEmpty();
  }

void XEnvironmentRequest::setExtraData(const QByteArray &in)
  {
  _extraData = in;
  }

QDataStream &operator<<(QDataStream &stream, const XEnvironmentRequest &itemRequest)
  {
  return stream << itemRequest._type << itemRequest._ID << itemRequest._subType << itemRequest._requestID << itemRequest._extraData;
  }

QDataStream &operator>>(QDataStream &stream, XEnvironmentRequest &itemRequest)
  {
  return stream >> itemRequest._type >> itemRequest._ID >> itemRequest._subType >> itemRequest._requestID >> itemRequest._extraData;
  }
