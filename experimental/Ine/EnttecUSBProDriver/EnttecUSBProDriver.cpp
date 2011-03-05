#include "EnttecUSBProDriver.h"
#include "EnttecUSBProInterface.h"
#include "IneManager.h"
#include "pro_driver.h"
#include "QString"

void EnttecUSBProDriver::refreshUniverses(IneManager *manager)
  {
  int numDevices = FTDI_ListDevices();

  for(int i=0; i<numDevices; ++i)
    {
    manager->addUniverse(new EnttecUSBProInterface(manager, this, i, numDevices == 1));
    }
  }

QString EnttecUSBProDriver::name() const
  {
  return "Enttec USB Pro";
  }
