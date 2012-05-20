#include "mcsimple.h"
#include "spropertyinformationhelpers.h"

S_IMPLEMENT_PROPERTY(MCSimple, MathsCore)

void MCSimple::createTypeInformation(SPropertyInformationTyped<MCSimple> *info,
                                     const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    FloatProperty::InstanceInformation *outInst = info->add(&MCSimple::output, "output");

    FloatProperty::InstanceInformation *aInst = info->add(&MCSimple::inputA, "inputA");
    FloatProperty::InstanceInformation *bInst = info->add(&MCSimple::inputB, "inputB");

    aInst->setAffects(outInst);
    bInst->setAffects(outInst);
    }
  }

MCSimple::MCSimple()
  {
  }
