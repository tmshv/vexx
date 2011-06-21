#include "UIVerticalLayout.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(UIVerticalLayout)

const SPropertyInformation *UIVerticalLayout::createTypeInformation()
  {
  return SPropertyInformation::create<UIVerticalLayout>("UIVerticalLayout");
  }

UIVerticalLayout::UIVerticalLayout()
  {
  }
