#include "Server.h"
#include "QFile"
#include "QDir"
#include "QApplication"

Server::Server() : _readingLength( -1 )
  {
  _server = new QTcpServer(this);
  _server->listen(QHostAddress::LocalHost, 16161);

  qDebug() << "Listening on" << _server->serverAddress().toString() << ":" << _server->serverPort();
  connect(_server, SIGNAL(newConnection()), this, SLOT(onConnection()));
  }

void Server::onConnection()
  {
  while(QTcpSocket *socket = _server->nextPendingConnection())
    {
    connect(socket, SIGNAL(readyRead()), this, SLOT(requestAvailable()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnection()));

    _clientData.insert(socket, ClientDataHash());

    qDebug() << "Connection from" << socket->peerAddress().toString() << ":" << socket->peerPort();
    }
  }

void Server::onDisconnection()
  {
  QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
  _clientData.remove(socket);
  qDebug() << "Disconnection from" << socket->peerAddress().toString() << ":" << socket->peerPort();
  }

void Server::requestAvailable()
  {
  QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
  QDataStream str(socket);

  bool repeat = true;
  do
    {
    repeat = false;
    if( _readingLength == -1 )
      {
      str >> _readingLength;
      }

    qDebug() << "Request recieved [" << socket->bytesAvailable() << "/" << _readingLength << "]";
    if( socket->bytesAvailable() >= _readingLength )
      {
      QByteArray out;
      str >> out;

      // read out request
      XEnvironmentRequest request;
      QDataStream subStream( &out, QIODevice::ReadWrite );
      subStream >> request;

      if(!request.hasExtraData())
        {
        putItem(str, request);
        }
      else
        {
        setItem(request);
        syncClients(request, socket);
        }

      _readingLength = -1;
      repeat = true;
      }
    }
  while( repeat && socket->bytesAvailable() > 0 );
  }

void Server::putItem( QDataStream &stream, const XEnvironmentRequest &request )
  {
  XEnvironmentRequest sendRequest(request);
  sendRequest.setExtraData(getItem(request));

  stream << sendRequest.saveSize() << sendRequest;
  }

void Server::setItem( const XEnvironmentRequest &r )
  {
  qDebug() << "Set item " << r.type() << r.ID() << "level" << r.subType();

  cacheData(r, r.extraData());

  QString dir = getItemDirectory(r.type(), r.ID());
  QString filename = getItemFilename(r.subType());

  xAssert(!prepareItemDirectory(dir));

  QFile file(dir + filename);
  if(file.open(QIODevice::WriteOnly))
    {
    QDataStream str(&file);
    str << r.extraData();
    file.close();
    }
  }

void Server::syncClients( const XEnvironmentRequest &request, QTcpSocket *sender )
  {
  foreach(QTcpSocket *socket, _clientData.keys())
    {
    if(socket != sender)
      {
      const ClientDataHash &data(_clientData[socket]);
      if(data.contains(request.type()) && data[request.type()].contains(request.ID()))
        {
        QDataStream str(socket);
        putItem(str, request);
        }
      }
    }
  }

QByteArray Server::getSpecialData(const XEnvironmentRequest &request) const
  {
  QByteArray arr;
  QDataStream str(&arr, QIODevice::WriteOnly);
  if(request.subType() == XEnvironment::CreateItem)
    {
    ItemID firstFreeId = X_UINT64_SENTINEL;
    QString dir;

    do
      {
      firstFreeId++;
      dir = getItemDirectory(request.ID(), firstFreeId);
      } while(QDir::root().exists(dir));

    xAssert(prepareItemDirectory(dir));
    str << firstFreeId;

    QString filename = getItemFilename(request.subType());

    QFile file(dir + filename);
    if(file.open(QIODevice::WriteOnly))
      {
      file.close();
      }
    }
  return arr;
  }

bool Server::prepareItemDirectory(const QString &dir) const
  {
  if(!QDir::root().exists(dir))
    {
    return QDir::root().mkpath(dir);
    }
  return false;
  }

QString Server::getDataDirectory() const
  {
  return QApplication::applicationDirPath() + QDir::separator() + "EnvironmentData" + QDir::separator();
  }

QString Server::getItemFilename(xuint16 subtype) const
  {
  return QString::number(subtype);
  }

QString Server::getItemDirectory(xuint16 type, ItemID id) const
  {
  return getDataDirectory() + QDir::separator() + QString::number(type) +
    QDir::separator() + QString::number(id) + QDir::separator();
  }

void Server::cacheData(const XEnvironmentRequest &request, const QByteArray &a) const
  {
  if(a.length())
    {
    _data[request.type()][request.ID()][request.subType()] = a;
    }
  }

const QByteArray Server::cachedData(const XEnvironmentRequest &request, bool *correct) const
  {
  if(_data.contains(request.type()))
    {
    const IDHash &idHash(_data[request.type()]);
    if(idHash.contains(request.ID()))
      {
      const SubTypeHash &subTypeHash(idHash[request.ID()]);
      if(subTypeHash.contains(request.subType()))
        {
        return subTypeHash[request.subType()];
        }
      }
    }
  }

QByteArray Server::getItem(const XEnvironmentRequest &req) const
  {
  if(req.type() == XEnvironment::SpecialType)
    {
    return getSpecialData(req);
    }

  bool correct = false;
  const QByteArray &cached = cachedData(req, &correct);
  if(correct)
    {
    return cached;
    }

  QByteArray arr;
  QFile file(getItemDirectory(req.type(), req.ID()) + getItemFilename(req.subType()));
  if(file.open(QIODevice::ReadOnly))
    {
    QDataStream str(&file);
    str >> arr;

    cacheData(req, arr);
    return arr;
    }

  return QByteArray();
  }
