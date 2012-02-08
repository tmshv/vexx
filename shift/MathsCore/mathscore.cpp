#include "MathsCore.h"
#include "styperegistry.h"
#include "mcmathsoperation.h"
#include "mcsource.h"
#include "mcimage.h"

void initiateMathsCore()
  {
  STypeRegistry::addType(MCMathsOperation::staticTypeInformation());
  STypeRegistry::addType(MCSource::staticTypeInformation());
  STypeRegistry::addType(MCImage::staticTypeInformation());
  }
