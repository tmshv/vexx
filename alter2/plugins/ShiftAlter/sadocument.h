#ifndef SDOCUMENT_H
#define SDOCUMENT_H

#include "saglobal.h"
#include "sentity.h"

class SHIFTALTER_EXPORT SDocument : public SEntity
  {
  S_ENTITY(SDocument, SEntity, 0)

public:
  SDocument();

  SPropertyArray transientData;
  };

#endif // SDOCUMENT_H
