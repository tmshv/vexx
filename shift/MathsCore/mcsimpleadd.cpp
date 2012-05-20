#include "mcsimpleadd.h"
#include "spropertyinformationhelpers.h"

void computeAddOutput(const SPropertyInstanceInformation *, MCSimpleAdd *add)
  {
  FloatProperty::ComputeLock l(&add->output);

  *l.data() = add->inputA() + add->inputB();
  }

S_IMPLEMENT_PROPERTY(MCSimpleAdd, MathsCore)

void MCSimpleAdd::createTypeInformation(SPropertyInformationTyped<MCSimpleAdd> *info,
                                        const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    auto outInst = info->child(&MCSimpleAdd::output);
    outInst->setCompute<computeAddOutput>();
    }
  }

MCSimpleAdd::MCSimpleAdd()
  {
  }
