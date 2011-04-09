#ifndef APPLICATION_H
#define APPLICATION_H

#include "QObject"
#include "Interface.h"
#include "XEnvironment.h"

class Application : public QObject, private XEnvironment::Listener
  {
  Q_OBJECT

XProperties:
  XRefProperty(Interface, dataInterface);
  XRefProperty(XEnvironment, environment);

public:
  Application();

  void createContainer(XEnvironment::ItemID parent);

signals:
  void dataChanged(const XEnvironment::Request &request);
  void requestComplete(const XEnvironment::Request &request);

private:
  virtual void onRequestComplete(const XEnvironmentRequest &request);
  };

#endif // APPLICATION_H
