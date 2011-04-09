#include "syimagebase.h"

S_PROPERTY_CONTAINER_DEFINITION(SyImageBase, SPropertyContainer)
    S_PROPERTY_DEFINITION(SFloatArrayProperty, test)
S_PROPERTY_CONTAINER_END_DEFINITION(SyImageBase, SPropertyContainer, saveContainer, loadContainer)

SyImageBase::SyImageBase()
  {
  }
