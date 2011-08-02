#include "UIStackedLayout.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(UIStackedLayout)

SPropertyInformation *UIStackedLayout::createTypeInformation()
  {
  return SPropertyInformation::create<UIStackedLayout>("UIStackedLayout");
  }

UIStackedLayout::UIStackedLayout()
{
}
