#include "Interface.h"
#include "QApplication"
#include "QTcpSocket"
#include "QHostAddress"

#define X_NULL_ITEM_ID (ItemID)-1

Interface::Interface() : _readingLength(X_UINT64_SENTINEL)
  {
  _socket = new QTcpSocket(this);

  _socket->connectToHost(QHostAddress::LocalHost, 16161, QIODevice::ReadWrite);
  connect(_socket, SIGNAL(readyRead()), this, SLOT(requestReady()));
  connect(_socket, SIGNAL(connected()), this, SLOT(onConnected()));
  }

void Interface::requestReady()
  {
  qDebug() << "Recieved data for item [" << _socket->bytesAvailable() << "/" << _readingLength  << "]";

  bool complete;
  QDataStream str(_socket);
  do
    {
    complete = false;
    if( _readingLength == X_UINT64_SENTINEL )
      {
      str >> _readingLength;
      qDebug() << "Begin receiving data." << _readingLength << "bytes required.";
      }

    xAssert(_readingLength != 0);

    if( _readingLength != X_UINT64_SENTINEL && (xuint64)_socket->bytesAvailable() >= _readingLength )
      {
      XEnvironmentRequest request;
      str >> request;

      qDebug() << "Parse complete transmission for item" << request.type() << request.ID() << request.subType();

      controller()->setData(request, request.extraData());
      controller()->requestComplete(request.requestID());

      xAssertFail();
      complete = true;
      _readingLength = X_UINT64_SENTINEL;
      }
    } while(complete && _socket->bytesAvailable() != 0);
  }

void Interface::onConnected()
  {
  pollPendingRequests();
  }

void Interface::pollPendingRequests()
  {
  QDataStream str(_socket);

  while(_requests.size())
    {
    str << _requests.back().saveSize() << _requests.back();
    _requests.removeLast();
    }

  while(_syncRequests.size())
    {
    ItemRequest &r = _syncRequests.back();

    bool correct = false;
    QByteArray data = controller()->getData(r, &correct);

    xAssertFail();
    if(correct)
      {
      r.setExtraData(data);
      str << r.saveSize() << r;
      }
    _syncRequests.removeLast();
    }
  }

void Interface::poll()
  {
  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
  }

void Interface::requestItem( const ItemRequest &request )
  {
  _requests << request;
  pollPendingRequests();
  }

void Interface::syncItem( const ItemRequest &request )
  {
  _syncRequests << request;
  pollPendingRequests();
  }
