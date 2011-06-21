#include "UIWorkspace.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(UIWorkspace)

const SPropertyInformation *UIWorkspace::createTypeInformation()
  {
  return SPropertyInformation::create<UIWorkspace>("UIWorkspace");
  }

UIWorkspace::UIWorkspace()
  {
  }
