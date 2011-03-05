#ifndef ENTTECUSBPROINTERFACE_H
#define ENTTECUSBPROINTERFACE_H

#include "EnttecUSBProGlobal.h"
#include "IneInterface.h"

class IneManager;
class EnttecUSBProDriver;

class EnttecUSBProInterface : public IneInterface
  {
public:
  EnttecUSBProInterface(IneManager *manager, EnttecUSBProDriver *driver, int deviceNumber, bool keepOpen);
  ~EnttecUSBProInterface();

  virtual void setChannels(xuint16 startChannel, const xuint8 *data, xuint16 length);
  virtual void beginOutputChanged();
  virtual void endOutputChanged();
  virtual IneDriver *driver() const;
  virtual QString name() const;

private:
  bool open();
  void close();

  int _deviceNumber;
  bool _keepOpen;
  xuint8 _startCode;
  xuint8 _channels[512];
  IneManager *_manager;
  EnttecUSBProDriver *_driver;
  };

#endif // ENTTECUSBPROINTERFACE_H
