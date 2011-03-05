#ifndef XENVIRONMENTITEMREQUEST_H
#define XENVIRONMENTITEMREQUEST_H

#include "XProperty"
#include "X3DGlobal.h"
#include "XEnvironmentArea.h"

class EKS3D_EXPORT XEnvironmentRequest
  {
public:
  typedef XEnvironmentID ItemID;
  typedef XEnvironmentArea::SubType SubType;

  enum
    {
    StaticSaveSize = sizeof(xuint16) + // the type
                     sizeof(xuint16) + // the type
                     sizeof(ItemID) + // the item id
                     sizeof(xuint32) + // the request id
                     sizeof(quint32) // the size of the extra data
    };

properties:
  XROProperty( xuint16, type );
  XROProperty( ItemID, ID );
  XROProperty( SubType, subType );
  XRORefProperty( QByteArray, extraData );
  XProperty( xuint32, requestID, setRequestID );

public:
  XEnvironmentRequest();
  XEnvironmentRequest(xuint16 type, ItemID lev, xuint16 subType=0, xuint32 requestID=0);

  xuint64 saveSize() const { return StaticSaveSize + _extraData.size(); }

  bool hasExtraData() const;
  void setExtraData(const QByteArray &);

  friend EKS3D_EXPORT QDataStream &operator<<(QDataStream &stream, const XEnvironmentRequest &itemRequest);
  friend EKS3D_EXPORT QDataStream &operator>>(QDataStream &stream, XEnvironmentRequest &itemRequest);
  };

#endif // XENVIRONMENTITEMREQUEST_H
