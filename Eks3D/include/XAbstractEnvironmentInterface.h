#ifndef XABSTRACTENVIRONMENTINTERFACE_H
#define XABSTRACTENVIRONMENTINTERFACE_H

#include "XGlobal"
#include "XProperty"
#include "XEnvironmentRequest.h"

class XEnvironmentItemDefinition;
class XEnvironmentItem;
class XEnvironment;

class EKS3D_EXPORT XAbstractEnvironmentInterface
  {
public:
  typedef XEnvironmentRequest ItemRequest;

XProperties:
  XProperty(XEnvironment *, controller, setController)

public:
  XAbstractEnvironmentInterface();

  virtual void poll();

  virtual void requestItem( const ItemRequest & ) = 0;
  virtual void syncItem( const ItemRequest & ) = 0;
  };

#endif // XABSTRACTENVIRONMENTINTERFACE_H
