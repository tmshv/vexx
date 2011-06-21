#include "UILayout.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(UILayout)

const SPropertyInformation *UILayout::createTypeInformation()
  {
  return SPropertyInformation::create<UILayout>("UILayout");
  }

UILayout::UILayout()
  {
  }
