#ifndef SERVER_H
#define SERVER_H

#include "QObject"
#include "QTcpServer"
#include "QTcpSocket"
#include "XEnvironment.h"
#include "XEnvironmentRequest.h"
#include "XEnvironmentArea.h"
#include "XColladaFile.h"

class Server : public QObject
  {
  Q_OBJECT

public:
  typedef XEnvironmentID ItemID;
  typedef XEnvironmentRequest Request;
  typedef XEnvironmentArea::SubType SubType;
  typedef QList <ItemID> ItemList;

  Server();

private slots:
  void onConnection();
  void onDisconnection();
  void requestAvailable();

private:
  QByteArray getTexture(ItemID id) const;
  QByteArray getShader(ItemID id) const;
  QByteArray getWorld(ItemID id, xuint16 subtype) const;

  void putItem( QDataStream &stream, const Request &request );
  void setItem( const Request & );
  void syncClients( const Request &, QTcpSocket *sender );
  QByteArray getSpecialData(const Request &level) const;

  bool prepareItemDirectory(const QString &dir) const;

  const QByteArray cachedData(const Request &request, bool *correct) const;
  void cacheData(const Request &request, const QByteArray &a) const;

  QString getItemFilename(SubType subtype) const;
  QString getDataDirectory() const;
  QString getItemDirectory(xuint16, ItemID) const;

  QByteArray getItem(const Request &req) const;

  typedef QHash <SubType, QByteArray> SubTypeHash;
  typedef QHash <ItemID, SubTypeHash> IDHash;
  typedef QHash <xuint16, IDHash> TypeHash;

  mutable TypeHash _data;

  QTcpServer *_server;
  int _readingLength;

  typedef QHash<xuint16, ItemList> ClientDataHash;
  QHash <QTcpSocket*, ClientDataHash> _clientData;
  };

#endif // SERVER_H
