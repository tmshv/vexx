#include "EnttecUSBProInterface.h"
#include "EnttecUSBProDriver.h"
#include "QString"
#include "IneManager.h"
#include "pro_driver.h"

EnttecUSBProInterface::EnttecUSBProInterface(IneManager *manager, EnttecUSBProDriver *driver, int deviceNumber, bool keepOpen) : _deviceNumber(deviceNumber), _keepOpen(keepOpen), _startCode(0), _manager(manager), _driver(driver)
  {
  if(_keepOpen)
    {
    if(!open())
      {
      _manager->error(QString("Couldn't open ENTTEC Pro USB at ID %1").arg(_deviceNumber));
      }
    }
  }

EnttecUSBProInterface::~EnttecUSBProInterface()
  {
  if(_keepOpen)
    {
    close();
    }
  }

QString EnttecUSBProInterface::name() const
  {
  return QString("Port %1").arg(_deviceNumber);
  }

IneDriver *EnttecUSBProInterface::driver() const
  {
  return _driver;
  }


void EnttecUSBProInterface::setChannels(xuint16 startChannel, const xuint8 *data, xuint16 length)
  {
  xAssert((startChannel + length) < 512);

  // copy into internal buffer
  memcpy(&(_channels[startChannel]), data, length);
  }

void EnttecUSBProInterface::beginOutputChanged()
  {
  }

void EnttecUSBProInterface::endOutputChanged()
  {
  if(!_keepOpen)
    {
    if(!open())
      {
      _manager->error(QString("Couldn't open ENTTEC Pro USB at ID %1").arg(_deviceNumber));
      return;
      }
    }

  if(!FTDI_SendData(SET_DMX_TX_MODE, &_startCode, 513))
    {
    _manager->error(QString("Couldn't send DMX data to ENTTEC Pro USB at ID %1").arg(_deviceNumber));
    }

  if(!_keepOpen)
    {
    close();
    }
  }

bool EnttecUSBProInterface::open()
  {
  return FTDI_OpenDevice(_deviceNumber);
  }

void EnttecUSBProInterface::close()
  {
  FTDI_ClosePort();
  }
