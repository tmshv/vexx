#include "debugger.h"
#include "QDebug"
#include "QTcpServer"
#include "QTcpSocket"

Debugger::Debugger() : _host("localhost"), _port(4566), _waitingForConnection(false)
  {
  _connection = 0;
  _server = new QTcpServer;
  connect(_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
  }

Debugger::~Debugger()
  {
  delete _connection;
  delete _server;
  }

const QString &Debugger::host() const
  {
  return _host;
  }

void Debugger::setHost(const QString &h)
  {
  _host = h;
  emit hostChanged();
  qDebug() << "Change Host" << h;
  }

int Debugger::port() const
  {
  return _port;
  }

void Debugger::setPort(int p)
  {
  _port = p;
  emit portChanged();
  qDebug() << "Change Port" << p;
  }

void Debugger::newConnection()
  {
  if(!_connection)
    {
    _connection = _server->nextPendingConnection();
    if(_connection)
      {
      qDebug() << "New Connection";
      emit connectedChanged();
      connect(_connection, SIGNAL(readyRead()), this, SLOT(newData()));
      }
    }

  while(_server->hasPendingConnections())
    {
    delete _server->nextPendingConnection();
    }
  }

void Debugger::newData()
  {
  Q_ASSERT(_connection);
  qDebug() << "New Data";
  if(_connection)
    {
    emit newDataRevieved(_connection->readAll());
    }
  }

bool Debugger::waitingForConnection() const
  {
  return _waitingForConnection;
  }

bool Debugger::connected() const
  {
  return _connection != 0;
  }

void Debugger::listen()
  {
  _waitingForConnection = true;

  QHostAddress address(_host);
  _server->listen(address, _port);

  emit connectedChanged();
  }

void Debugger::stopListening()
  {
  _waitingForConnection = false;

  _server->close();

  emit connectedChanged();
  }

void Debugger::disconnect()
  {
  delete _connection;
  _connection = 0;

  _server->close();
  }

void Debugger::sendData(QString data)
  {
  if(_connection)
    {
    _connection->write(data.toUtf8());
    }
  }
