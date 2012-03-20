#ifndef SREFERENCEENTITY_H
#define SREFERENCEENTITY_H

#include "sentity.h"

class SHIFT_EXPORT SReferenceEntity : public SEntity
  {
  S_ENTITY(SReferenceEntity, SEntity, 0);

public:
  SReferenceEntity();
  };

S_PROPERTY_INTERFACE(SReferenceEntity)

#endif // SREFERENCEENTITY_H
