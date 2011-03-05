#ifndef XNETWORK_H
#define XNETWORK_H

#include "XAddGlobal.h"
#include "XMap"
#include "XList"
#include "XNodeDefinition.h"

class XNodeInputObserver;
class XNodeOutputObserver;
class XNodeDirtyObserver;

class EKSADD_EXPORT XNetwork
  {
public:
  typedef xuint32 InstanceID;
  XNetwork();

  void addDefinition(const XNodeDefinition &);
  InstanceID createNode(XNodeDefinition::TypeID);
  void destroyNode(InstanceID);

  void connect(InstanceID outputNode, XNodeDefinition::OutputID, InstanceID inputNode, XNodeDefinition::InputID);
  void disconnect(InstanceID outputNode, XNodeDefinition::OutputID, InstanceID inputNode, XNodeDefinition::InputID);

  void setInput(InstanceID, XNodeDefinition::InputID, const XVariant &);
  XVariant &output(InstanceID, XNodeDefinition::OutputID);

  void addOutputObserver(InstanceID, XNodeOutputObserver *);
  void addDirtyObserver(InstanceID, XNodeDirtyObserver *);

  void removeOutputObserver(XNodeOutputObserver *);
  void removeDirtyObserver(XNodeDirtyObserver *);

private:
  class NodeInstance;
  void disconnect(NodeInstance *, XNodeDefinition::OutputID, NodeInstance *, XNodeDefinition::InputID);
  void prepareOutput(NodeInstance*, XNodeDefinition::OutputID);
  void dirtyInput(NodeInstance*, XNodeDefinition::InputID);
  XMap <XNodeDefinition::TypeID, XNodeDefinition*> _nodes;
  XMap <InstanceID, NodeInstance*> _instances;
  };

#endif // XNETWORK_H
