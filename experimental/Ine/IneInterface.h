#ifndef INEINTERFACE_H
#define INEINTERFACE_H

#include "IneGlobal.h"

class IneDriver;

class IneInterface
  {
public:
  virtual QString name() const = 0;

  virtual IneDriver *driver() const = 0;
  virtual void setChannels(xuint16 startChannel, const xuint8 *data, xuint16 length) = 0;
  virtual void beginOutputChanged() = 0;
  virtual void endOutputChanged() = 0;
  };

#endif // INEINTERFACE_H
