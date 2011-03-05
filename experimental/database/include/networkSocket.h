#pragma once

#include "binaryStream.h"
#include "field.h"
#include "QObject"

class QTcpSocket;

namespace db
{
class DATABASE_EXPORT networkSocket : public QObject
{
Q_OBJECT
public:
  networkSocket( QObject *parent, std::string host, unsigned int port );
  void sendStream( const binaryStream & );
  void sendField( const field & );
  void getStream( binaryStream & );
  binaryStream getStream( );
  bool isConnected();
  bool isValid();
  bool hasStream();
  void blockForStream( unsigned int );
  static binaryStream streamFromSocket( QTcpSocket * );
  static void streamToSocket( QTcpSocket *, const binaryStream );
  static void streamToSocket( QTcpSocket *, const field * );
  static void streamToSocket( QTcpSocket *, const field & );
public slots:
  void onStream();
private:
  QTcpSocket *socket;
  std::vector <binaryStream> streams;
};
}

