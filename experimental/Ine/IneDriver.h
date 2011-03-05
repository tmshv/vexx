#ifndef INEDRIVER_H
#define INEDRIVER_H

#include "IneGlobal.h"

class IneManager;

class IneDriver
  {
public:
  virtual QString name() const = 0;
  virtual void refreshUniverses(IneManager *) = 0;
  };

#endif // INEDRIVER_H
