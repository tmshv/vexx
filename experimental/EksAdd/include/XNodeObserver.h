#ifndef XNODEOBSERVER_H
#define XNODEOBSERVER_H

#include "XAddGlobal.h"
#include "XNetwork.h"
#include "XSet"

class EKSADD_EXPORT XNodeOutputObserver
  {
public:
  virtual ~XNodeOutputObserver();
  virtual void onCalculation(XNetwork *net, XNetwork::InstanceID id, XNodeDefinition::OutputID output) = 0;

private:
  XSet<XNetwork*> _networks;
  friend class XNetwork;
  };

class EKSADD_EXPORT XNodeDirtyObserver
  {
public:
  virtual ~XNodeDirtyObserver();
  virtual void onDirty(XNetwork *net, XNetwork::InstanceID id, XNodeDefinition::OutputID output) = 0;

private:
  XSet<XNetwork*> _networks;
  friend class XNetwork;
  };

#endif // XNODEOBSERVER_H
