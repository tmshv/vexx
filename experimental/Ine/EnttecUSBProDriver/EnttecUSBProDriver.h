#ifndef ENTTECUSBPRODRIVER_H
#define ENTTECUSBPRODRIVER_H

#include "EnttecUSBProGlobal.h"
#include "IneDriver.h"

class ENTTEXUSBPRO_EXPORT EnttecUSBProDriver : public IneDriver
  {
public:
  void refreshUniverses(IneManager *);
  QString name() const;
  };

#endif // ENTTECUSBPRODRIVER_H
