#include "mcsimplemultiply.h"

void computeAddOutput(const SPropertyInstanceInformation *, MCSimpleMultiply *add)
  {
  FloatProperty::ComputeLock l(&add->output);

  *l.data() = add->inputA() * add->inputB();
  }

S_IMPLEMENT_PROPERTY(MCSimpleMultiply)

void MCSimpleMultiply::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    FloatProperty::InstanceInformation *outInst = info->child(&MCSimpleMultiply::output);
    outInst->setCompute(computeAddOutput);
    }
  }

MCSimpleMultiply::MCSimpleMultiply()
  {
  }
