#ifndef SYIMAGEBASE_H
#define SYIMAGEBASE_H

#include "syglobal.h"
#include "sbaseproperties.h"
#include "sarrayproperty.h"

class SyImageBase : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(SyImageBase, SPropertyContainer, 0);
public:
  SyImageBase();

  SFloatArrayProperty test;
  };

#endif // SYIMAGEBASE_H
