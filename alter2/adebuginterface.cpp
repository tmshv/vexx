#include "adebuginterface.h"
#include "XAssert"
#include "QTcpSocket"

ADebugInterface::ADebugInterface(const QString &type) : _type(type)
  {
  _socket = new QTcpSocket;

  _socket->connectToHost("localhost", 4566);
  if(_socket->state() == QTcpSocket::UnconnectedState)
    {
    xAssertFail();
    delete _socket;
    }
  }

ADebugInterface::~ADebugInterface()
  {
  delete _socket;
  }

QIODevice *ADebugInterface::device()
  {
  return _socket;
  }

bool ADebugInterface::readyForData() const
  {
  return _socket->state() == QTcpSocket::ConnectedState;
  }

