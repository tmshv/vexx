#include "networkServer.h"
#include "ui_networkServerLog.h"
#include "QTcpServer"
#include "QTcpSocket"
#include "QTimer"

namespace db
{
#ifdef SERVERWITHLOG
serverLog::serverLog( ) : requests( 0 ), connections( 0 ), log( new Ui::ServerLogWindow )
{
  log->setupUi(this);
  timer = new QTimer( this );
  connect( timer, SIGNAL(timeout()), this, SLOT(refreshStatus()) );
  refreshStatus();
  addMessage( "Server log up" );
  timer->start( 1000 );
  show();
}
void serverLog::addMessage( QString in )
  {
  log->textEdit->setText( in + "\n" + log->textEdit->toPlainText() );
  }
void serverLog::refreshStatus()
  {
  QString reqs;
  QString conn;
  reqs.setNum( requests );
  requests = 0;
  conn.setNum( connections );
  log->statusBar->showMessage( conn + " connections, " + reqs + " handled in the last second" );
  }
#endif

networkServer::networkServer( QObject *parent, unsigned int port )
#ifdef SERVERWITHLOG
    : log(new serverLog)
#endif
  {
  server = new QTcpServer( parent );
  server->listen( QHostAddress::Any, port );
  connect( server, SIGNAL(newConnection()), this, SLOT(onConnection()) );
  QString p;
  p.setNum( port );
  addMessage( "Network server up, listening on port " + p );
  }

void networkServer::onConnection()
  {
  while( server->hasPendingConnections() )
    {
    QTcpSocket *socket = server->nextPendingConnection();

    QString port;
    port.setNum( socket->peerPort() );

    addMessage( "New connection from " + socket->peerAddress().toString() + " " + port );

    sockets.push_back( socket );
    connect( socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()) );
    connect( socket, SIGNAL(readyRead()), this, SLOT(onData()) );
    }
  }

void networkServer::onDisconnect()
  {
  for( unsigned int x=0; x<sockets.size(); x++ )
    {
    if( sockets[x]->state() == QAbstractSocket::UnconnectedState )
      {
      QString port;
      port.setNum( sockets[x]->peerPort() );
      addMessage( "Disconnect from " + sockets[x]->peerAddress().toString() + " " + port );
      sockets.erase( sockets.begin() + x );
      }
    }
  }

void networkServer::onData()
  {
  for( unsigned int x=0; x<sockets.size(); x++ )
    {
    qint64 bytes = 0;
    if( ( bytes = sockets[x]->bytesAvailable() ) > 0 )
      {
      db::binaryStream stream;
      stream.resize( bytes );
      sockets[x]->read( stream.getDataPtr(), bytes );

      onStream( x, stream );
      }
    }
  }

void networkServer::onStream( unsigned int, binaryStream )
  {
  }

void networkServer::sendStream( unsigned int x, binaryStream stream )
  {
  assert( x < sockets.size() );
  sockets[x]->write( stream.getDataPtr(), stream.size() );
  }
}
