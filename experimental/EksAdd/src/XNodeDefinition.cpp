#include "XNodeDefinition.h"
#include "XNetwork.h"

XNodeDefinition::XNodeDefinition(TypeID type) : _type(type)
  {
  _calculations.reserve(5);
  }

xuint32 XNodeDefinition::addInput( const XVariant &in )
  {
  _defaultInputs << in;
  return _defaultInputs.size() - 1;
  }

xuint32 XNodeDefinition::addOutput( const XVariant &out )
  {
  _defaultOutputs << out;
  return _defaultOutputs.size() - 1;
  }

void XNodeDefinition::addCalculation( CalculationFunction func,
                            const Inputs &in,
                            const Outputs &out )
  {
  Calculation calc;
  calc.func = func;
  calc.inputIDs = in;
  calc.outputIDs = out;

  XVector<InputID> inVec(in.toVector());
  XVector<OutputID> outVec(out.toVector());

  foreach(const InputID &input, inVec)
    {
    _inputMap[input] << outVec;
    }

  foreach(const OutputID &output, outVec)
    {
    _outputMap[output] << inVec;
    }

  _calculations << calc;
  }
