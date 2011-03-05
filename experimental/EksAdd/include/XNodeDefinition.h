#ifndef XNODE_H
#define XNODE_H

#include "XAddGlobal.h"
#include "XList"
#include "XMap"
#include "XVector"
#include "XProperty"
#include "XVariant"

class XNetwork;

#define X_NODE(node, parent)

class EKSADD_EXPORT XNodeDefinition
  {
public:
  typedef xuint32 InputID;
  typedef xuint32 OutputID;
  typedef xuint32 TypeID;
  typedef XList<InputID> Inputs;
  typedef XList<OutputID> Outputs;

  typedef void (*CalculationFunction)(const XVariant **, XVariant **);
  struct Calculation
    {
    CalculationFunction func;
    Inputs inputIDs;
    Outputs outputIDs;
    };

properties:
  XRORefProperty(TypeID, type);
  XRORefProperty(XVector <Calculation>, calculations);
  XRORefProperty(XVariantVector, defaultInputs);
  XRORefProperty(XVariantVector, defaultOutputs);

  typedef XMap<OutputID, XVector<InputID> > OutputMapLookup;
  XRORefProperty(OutputMapLookup, outputMap);

  typedef XMap<InputID, XVector<OutputID> > InputMapLookup;
  XRORefProperty(InputMapLookup, inputMap);

public:
  XNodeDefinition(TypeID type);

  xuint32 addInput( const XVariant & );
  xuint32 addOutput( const XVariant & );

  void addCalculation( CalculationFunction, const Inputs &, const Outputs & );
  };

#endif // XNODE_H
