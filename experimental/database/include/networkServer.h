#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "utility.h"
#include "QObject"
#include "QMainWindow"
#include "binaryStream.h"

#define SERVERWITHLOG

class QTcpServer;
class QTcpSocket;

namespace Ui
{
  class ServerLogWindow;
}

namespace db
{
#ifdef SERVERWITHLOG
class serverLog : public QMainWindow
{
  Q_OBJECT
public:
  serverLog( );
  void addMessage( QString );
  unsigned int requests;
  unsigned int connections;
public slots:
  void refreshStatus();
private:
  QTimer *timer;
  Ui::ServerLogWindow *log;
};
#endif
class DATABASE_EXPORT networkServer : public QObject
{
  Q_OBJECT
public:
  networkServer( QObject *, unsigned int );
protected:
  virtual void onStream( unsigned int, binaryStream );
  void sendStream( unsigned int, binaryStream );
  void addMessage( QString in )
    {
#ifdef SERVERWITHLOG
    log->addMessage( in );
#endif
    }
protected slots:
  void onConnection();
  void onDisconnect();
  void onData();
private:
  QTcpServer *server;
  std::vector <QTcpSocket *> sockets;
#ifdef SERVERWITHLOG
  serverLog *log;
#endif
};
}

#endif
