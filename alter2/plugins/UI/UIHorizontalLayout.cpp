#include "UIHorizontalLayout.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(UIHorizontalLayout)

SPropertyInformation *UIHorizontalLayout::createTypeInformation()
  {
  return SPropertyInformation::create<UIHorizontalLayout>("UIHorizontalLayout");
  }

UIHorizontalLayout::UIHorizontalLayout()
  {
  }
