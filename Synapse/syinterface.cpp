#include "syinterface.h"

SyInterface::SyInterface()
  {
  }

QObject *SyInterface::createViewer()
  {
  return new SyViewer;
  }
