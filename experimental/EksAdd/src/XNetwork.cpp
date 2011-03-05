#include "XNetwork.h"
#include "XNodeObserver.h"
#include "XProperty"

class XNetwork::NodeInstance
  {
public:
  struct CalculationInstance
    {
    XNodeDefinition::CalculationFunction func;
    XVector <XVariant*> inputs;
    XVector <XVariant*> outputs;
    bool dirty;
    };

  struct Connection
    {
    Connection() : node( 0 ), id( -1 ) { }
    NodeInstance *node;
    xuint32 id;
    };

  // The definition this node is instanced from
  const XNodeDefinition *node;
  // the inputs for this node, inputs in CalculationInstances point at these
  XVector <XVariant> inputs;
  // the inputs for this node, outputs in CalculationInstances point at these
  XVector <XVariant> outputs;
  // calculation instances
  XVector <CalculationInstance> calculations;
  // Map from outputs to calculations
  XVector <CalculationInstance*> calculationMap;
  // connections into the inputs
  XVector <Connection> connectionsIn;
  // connections from the outputs
  XVector <XVector <Connection> > connectionsOut;
  // input observers
  XVector <XNodeInputObserver*> inputObservers;
  // output observers
  XVector <XNodeOutputObserver*> outputObservers;
  // dirty observers
  XVector <XNodeDirtyObserver*> dirtyObservers;

  NodeInstance(const XNodeDefinition *n)
    {
    // assign the definition pointer
    node = n;

    // copy the default values
    inputs = node->defaultInputs();
    outputs = node->defaultOutputs();

    // create the connection maps
    connectionsIn.fill(Connection(), inputs.size());
    connectionsOut.fill(XVector<Connection>(), inputs.size());

    // for each calculation in the definition
    calculations.reserve(node->calculations().size());
    const XNodeDefinition::Calculation *first = &node->calculations().first(),
                                       *calc = first,
                                       *calcEnd = first + node->calculations().size();
    for(; calc != calcEnd; ++calc)
      {
      // create instance from its definition
      CalculationInstance calcInst;
      calcInst.func = calc->func;
      calcInst.dirty = true;

      // set up the input pointer to point at the instanced nodes values
      const xuint32 *index = &calc->inputIDs.first(), *end = index + calc->inputIDs.size();
      for(; index != end; ++index)
        {
        calcInst.inputs << &inputs[*index];
        }

      // set up the output pointer to point at the instanced nodes values
      index = &calc->outputIDs.first(); end = index + calc->outputIDs.size();
      for(; index != end; ++index)
        {
        calcInst.outputs << &outputs[*index];
        }

      calculations << calcInst;
      }

    calculationMap.fill(0, outputs.size());
    calc = first;
    for(; calc != calcEnd; ++calc)
      {
      // set up the output pointer to point at the instanced nodes values
      const xuint32 *index = &calc->outputIDs.first(),
                    *end = index + calc->outputIDs.size();
      for(; index != end; ++index)
        {
        calculationMap[*index] = &calculations[calc-first];
        }
      }
    }
  };

XNetwork::XNetwork()
  {
  }

void XNetwork::addDefinition(const XNodeDefinition &node)
  {
  _nodes.insert(node.type(), new XNodeDefinition(node));
  }

XNetwork::InstanceID XNetwork::createNode(XNodeDefinition::TypeID type)
  {
  static xuint32 id = 0;
  _instances.insert(id, new NodeInstance(_nodes[type]));
  return id++;
  }

void XNetwork::destroyNode(InstanceID id)
  {
  NodeInstance *node = _instances[id];

  //for all the nodes in the network
  foreach(NodeInstance *poss, _instances)
    {
    if(poss == node)
      {
      continue;
      }

    // for each connection on the node
    NodeInstance::Connection *firstConn = &poss->connectionsIn.first(),
                             *conn = firstConn,
                             *lastConn = firstConn + poss->connectionsIn.size();
    for(; conn != lastConn; ++conn)
      {
      // if its connected to the deleting node
      if(conn->node == node)
        {
        disconnect(node, conn->id, poss, conn-firstConn);
        }
      }

    // for each connection on the node
    XVector<NodeInstance::Connection> *firstConnVector = &poss->connectionsOut.first(),
                                      *connVector = firstConnVector,
                                      *lastConnVector = firstConnVector + poss->connectionsOut.size();
    for(; connVector != lastConnVector; ++connVector)
      {
      if(connVector->size())
        {
        // for each connection on the output node
        NodeInstance::Connection *firstConn = &connVector->first(),
                                 *conn = firstConn,
                                 *lastConn = firstConn + connVector->size();
        for(; conn != lastConn; ++conn)
          {
          // if its the disconnecting connection
          if(conn->node == node)
            {
            disconnect(poss, conn-firstConn, node, conn->id);
            break;
            }
          }
        }
      }
    }

  delete node;
  _instances.remove(id);
  }

void XNetwork::connect(InstanceID output, XNodeDefinition::OutputID outID,
                       InstanceID input, XNodeDefinition::InputID inID)
  {
  NodeInstance *oNode = _instances[output];
  NodeInstance *iNode = _instances[input];
  xAssert(oNode);
  xAssert(iNode);
  xAssert(outID < (xuint32)oNode->inputs.size());
  xAssert(inID < (xuint32)iNode->inputs.size());

  // if its not already connected
  if(iNode->connectionsIn[inID].node == 0)
    {
    // dirty the inputs
    const xuint32 *index = &iNode->node->inputMap()[inID].first(), *end = index + iNode->node->inputMap()[inID].size();
    for(; index != end; ++index)
      {
      iNode->calculationMap[*index]->dirty = true;
      }

    // set up the connection maps
    NodeInstance::Connection inCon;
    inCon.node = oNode;
    inCon.id = outID;
    iNode->connectionsIn[inID] = inCon;

    NodeInstance::Connection outCon;
    outCon.node = iNode;
    outCon.id = inID;
    oNode->connectionsOut[outID] << outCon;

    // set up the value pointers to point at the correct outputs
    const XNodeDefinition::Calculation *first = &iNode->node->calculations().first();
    const XNodeDefinition::Calculation *calc = first, *calcEnd = first + iNode->node->calculations().size();
    for(; calc != calcEnd; ++calc)
      {
      iNode->calculations[calc-first].inputs[inID] = &oNode->outputs[outID];
      }
    }
  else
    {
    xAssert(0 && "Node already connected");
    }
  }

void XNetwork::disconnect(InstanceID output, XNodeDefinition::OutputID outID,
                       InstanceID input, XNodeDefinition::InputID inID)
  {
  NodeInstance *oNode = _instances[output];
  NodeInstance *iNode = _instances[input];
  disconnect(oNode, outID, iNode, inID);
  }

void XNetwork::disconnect(NodeInstance *oNode, XNodeDefinition::OutputID outID,
                       NodeInstance *iNode, XNodeDefinition::InputID inID)
  {
  xAssert(oNode);
  xAssert(iNode);
  xAssert(outID < (xuint32)oNode->inputs.size());
  xAssert(inID < (xuint32)iNode->inputs.size());

  // Copy on disconnect causes this to not be necessary, but it might be?
  // for each output connected to the input, dirty it.
  //const xuint32 *index = &iNode->node->inputMap()[inID].first(), *end = index + iNode->node->inputMap()[inID].size();
  //for(; index != end; ++index)
  //  {
  //  iNode->calculationMap[*index]->dirty = true;
  //  }

  // set the connection to null
  NodeInstance::Connection con;
  iNode->connectionsIn[inID] = con;

  // for each connection on the output node
  NodeInstance::Connection *firstConn = &oNode->connectionsOut[outID].first(),
                           *conn = firstConn,
                           *lastConn = firstConn + oNode->connectionsOut[outID].size();
  for(; conn != lastConn; ++conn)
    {
    // if its the disconnecting connection
    if(conn->node == iNode)
      {
      // remove from the output list
      oNode->connectionsOut[outID].remove(conn-firstConn);
      }
    }

  // for each calculation set on the input (destination) node
  const XNodeDefinition::Calculation *firstCalc = &iNode->node->calculations().first(),
                                     *calc = firstCalc,
                                     *calcEnd = firstCalc + iNode->node->calculations().size();
  for(; calc != calcEnd; ++calc)
    {
    // copy old value to default location
    XVariant &var = iNode->inputs[inID];
    XVariant *var2 = iNode->calculations[calc-firstCalc].inputs[inID];
    var = *var2;
    // reset to default location
    iNode->calculations[calc-firstCalc].inputs[inID] = &iNode->inputs[inID];
    }
  }

void XNetwork::setInput(XNetwork::InstanceID id, XNodeDefinition::InputID input, const XVariant &var)
  {
  NodeInstance *inst = _instances[id];
  xAssert(inst);
  xAssert(input < (xuint32)inst->inputs.size());

  // set the value, then dirty the connected nodes
  inst->inputs[input] = var;
  dirtyInput(inst, input);
  }

void XNetwork::dirtyInput(NodeInstance *inst, XNodeDefinition::InputID input)
  {
  // for each ioutput connected to this input, dirty it
  const xuint32 *index = &inst->node->inputMap()[input].first(), *end = index + inst->node->inputMap()[input].size();
  for(; index != end; ++index)
    {
    if(!inst->calculationMap[*index]->dirty)
      {
      // trigger the observers
      if(inst->dirtyObservers.size())
        {
        XNodeDirtyObserver **obs = &inst->dirtyObservers.first(), **obsEnd = obs + inst->dirtyObservers.size();
        for(; obs != obsEnd; ++obs)
          {
          (*obs)->onDirty(this, _instances.key(inst), *index);
          }
        }

      inst->calculationMap[*index]->dirty = true;

      // if there are connections
      if(inst->connectionsOut[*index].size())
        {
        // dirty them
        NodeInstance::Connection *conn = &inst->connectionsOut[*index].first(),
                                 *lastConn = conn + inst->connectionsOut[*index].size();
        for(; conn != lastConn; ++conn)
          {
          dirtyInput(conn->node, conn->id);
          }
        }
      }
    }
  }

XVariant &XNetwork::output(InstanceID id, XNodeDefinition::OutputID output)
  {
  NodeInstance *inst = _instances[id];
  xAssert(inst);
  xAssert(output < (xuint32)inst->outputs.size());

  // prepare the output, then return it
  prepareOutput(inst, output);
  return inst->outputs[output];
  }

void XNetwork::prepareOutput(NodeInstance *inst, XNodeDefinition::OutputID output)
  {
  NodeInstance::CalculationInstance *calc = inst->calculationMap[output];
  if(calc->dirty)
    {
    // for each input connected to the output, compute the node
    const xuint32 *index = &inst->node->outputMap()[output].first(), *end = index + inst->node->outputMap()[output].size();
    for(; index != end; ++index)
      {
      const NodeInstance::Connection &connection = inst->connectionsIn[*index];
      // if this input has a connection, prepare it.
      if(connection.node)
        {
        prepareOutput(connection.node, connection.id);
        }
      }

    if(inst->outputObservers.size())
      {
      XNodeOutputObserver **obs = &inst->outputObservers.first(), **obsEnd = obs + inst->outputObservers.size();
      for(; obs != obsEnd; ++obs)
        {
        (*obs)->onCalculation(this, _instances.key(inst), output);
        }
      }

    // call the compute function
    calc->func((const XVariant **)&calc->inputs[0], &calc->outputs[0]);

    // node is not dirty any more
    calc->dirty = false;
    }
  }

void XNetwork::addOutputObserver(InstanceID id, XNodeOutputObserver *obs)
  {
  obs->_networks << this;

  NodeInstance *inst = _instances[id];
  inst->outputObservers << obs;
  }

void XNetwork::addDirtyObserver(InstanceID id, XNodeDirtyObserver *obs)
  {
  obs->_networks << this;

  NodeInstance *inst = _instances[id];
  inst->dirtyObservers << obs;
  }

void XNetwork::removeOutputObserver(XNodeOutputObserver *obs)
  {
  foreach(NodeInstance *poss, _instances)
    {
    for(int i=0; i<poss->outputObservers.size(); ++i)
      {
      if(poss->outputObservers[i] == obs)
        {
        poss->outputObservers.remove(i);
        }
      }
    }
  }

void XNetwork::removeDirtyObserver(XNodeDirtyObserver *obs)
  {
  foreach(NodeInstance *poss, _instances)
    {
    for(int i=0; i<poss->dirtyObservers.size(); ++i)
      {
      if(poss->dirtyObservers[i] == obs)
        {
        poss->dirtyObservers.remove(i);
        }
      }
    }
  }
