#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "QObject"
#include "QString"

class QTcpServer;
class QTcpSocket;

class Debugger : public QObject
  {
  Q_OBJECT

  Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
  Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
  Q_PROPERTY(bool waitingForConnection READ waitingForConnection NOTIFY connectedChanged)
  Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
  Debugger();
  ~Debugger();

  const QString &host() const;
  void setHost(const QString &);

  int port() const;
  void setPort(int);

  bool waitingForConnection() const;
  bool connected() const;

  Q_INVOKABLE void listen();
  Q_INVOKABLE void stopListening();
  Q_INVOKABLE void disconnect();

  Q_INVOKABLE void sendData(QString data);

signals:
  void hostChanged();
  void portChanged();
  void connectedChanged();
  void newDataRevieved(QString data);

private slots:
  void newConnection();
  void newData();

private:
  QString _host;
  int _port;
  bool _waitingForConnection;

  QTcpServer* _server;
  QTcpSocket * _connection;
  };

#endif // DEBUGGER_H
