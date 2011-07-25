#include "networkSocket.h"
#include "QTcpSocket"

namespace db
{
networkSocket::networkSocket( QObject *parent, std::string host, unsigned int port )
{
  socket = new QTcpSocket( parent );
  socket->connectToHost( QString::fromStdString(host), port, QIODevice::ReadWrite );
  connect( socket, SIGNAL(readyRead()), this, SLOT(onStream()) );
}

void networkSocket::onStream()
{
  binaryStream temp;
  qint64 available = socket->bytesAvailable();

  temp._data.resize( available );

  socket->read( &(temp._data[0]), available );

  streams.push_back( temp );
}

bool networkSocket::isConnected()
{
  return socket->state() == QAbstractSocket::ConnectedState;
}

bool networkSocket::isValid()
{
  return socket->isValid();
}

bool networkSocket::hasStream()
{
  return streams.size() > 0;
}

void networkSocket::blockForStream( unsigned int msecs )
{
  socket->waitForReadyRead( msecs );
}

void networkSocket::sendField( const field &fi )
{
  binaryStream stream;
  fi.getAsStream();
  sendStream( stream );
}

void networkSocket::sendStream( const binaryStream &stream )
{
  QString temp;
  temp.setNum( socket->write( stream.getDataPtr(), stream.size() ) );
}

void networkSocket::getStream( binaryStream &stream )
{
  stream = streams.front();
  streams.erase( streams.begin() );
}

binaryStream networkSocket::getStream( )
{
  binaryStream stream = streams.front();
  streams.erase( streams.begin() );
  return stream;
}

binaryStream networkSocket::streamFromSocket( QTcpSocket *socket )
{
  binaryStream ret;
  qint64 available = socket->bytesAvailable();

  ret._data.resize( available );

  socket->read( &(ret._data[0]), available );

  return ret;
}

void streamToSocket( QTcpSocket *socket, const binaryStream stream )
{
  socket->write( stream.getDataPtr(), stream.size() );
}

void streamToSocket( QTcpSocket *so, const field *fi )
{
  binaryStream stream;
  fi->getAsStream( stream );
  streamToSocket( so, stream );
}

void streamToSocket( QTcpSocket *so, const field &fi )
{
  binaryStream stream;
  fi.getAsStream( stream );
  streamToSocket( so, stream );
}

}
