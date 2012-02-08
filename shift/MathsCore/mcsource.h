#ifndef MCSOURCE_H
#define MCSOURCE_H

#include "sentity.h"
#include "mcmathsoperation.h"

class MCSource : public SEntity
  {
  S_ENTITY(MCSource, SEntity, 0);

public:
  MCSource();

  MCMathsOperation output;
  };

#endif // MCSOURCE_H
