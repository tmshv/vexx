#include "QDebug"
#include "QStringList"
#include "acore.h"
#include "QApplication"

#include "InePlugin.h"
#include "IneDriver.h"
#include "IneInterface.h"

class TestIneInterface : public IneInterface
  {
  IneDriver *_driver;
public:
  TestIneInterface(IneDriver *d) : _driver(d)
    {
    }
  virtual QString name() const
    {
    return "Test Interface";
    }
  virtual IneDriver *driver() const
    {
    return _driver;
    }
  virtual void setChannels(xuint16 startChannel, const xuint8 *data, xuint16 length)
    {
    qDebug() << "\t" << "Set Channels" << startChannel << "-" << startChannel+length << "with" << data[0];
    }
  virtual void beginOutputChanged()
    {
    qDebug() << "BeginChange";
    }
  virtual void endOutputChanged()
    {
    qDebug() << "EndChange";
    }
  };

class TestIneDriver : public IneDriver
  {
  void refreshUniverses(IneManager *m)
    {
    m->addUniverse(new TestIneInterface(this));
    }
  QString name() const
    {
    return "Test Driver";
    }
  };


int main(int argc, char *argv[])
  {
  ACore app( argc, argv );

  app.addDirectory(QApplication::applicationDirPath());
  app.load("UI");
  //app.load("Shift");

  return app.execute();
  }
