#ifndef INTERFACE_H
#define INTERFACE_H

class QTcpSocket;

#include "QList"
#include "QDataStream"

#include "XModeller.h"
#include "XShader.h"

#include "XEnvironment.h"
#include "XAbstractEnvironmentInterface.h"
#include "XColladaFile.h"
#include "XTexture.h"

class Interface : public QObject, public XAbstractEnvironmentInterface
  {
  Q_OBJECT

public:
  Interface();

  virtual void poll();

  virtual void requestItem( const ItemRequest & );
  virtual void syncItem( const ItemRequest & );

private slots:
  void requestReady();
  void onConnected();

private:
  QTcpSocket *_socket;
  QList<ItemRequest> _requests;
  QList<ItemRequest> _syncRequests;
  xuint64 _readingLength;
  void pollPendingRequests();
  };


#endif // INTERFACE_H
