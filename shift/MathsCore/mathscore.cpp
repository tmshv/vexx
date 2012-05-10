#include "MathsCore.h"
#include "styperegistry.h"
#include "mcmathsoperation.h"
#include "mcsource.h"
#include "mcimage.h"

#include "mcsimple.h"
#include "mcsimpleadd.h"
#include "mcsimplemultiply.h"

void initiateMathsCore()
  {
  STypeRegistry::addType(MCMathsOperation::staticTypeInformation());
  STypeRegistry::addType(MCSource::staticTypeInformation());
  STypeRegistry::addType(MCImage::staticTypeInformation());

  STypeRegistry::addType(MCSimple::staticTypeInformation());
  STypeRegistry::addType(MCSimpleAdd::staticTypeInformation());
  STypeRegistry::addType(MCSimpleMultiply::staticTypeInformation());
  }
