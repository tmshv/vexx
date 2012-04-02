#ifndef UISTACKEDLLAYOUT_H
#define UISTACKEDLLAYOUT_H

#include "UIGlobal.h"
#include "UILayout.h"

class UI_EXPORT UIStackedLayout : public UILayout
  {
  S_ENTITY(UIStackedLayout, UILayout, 0)
public:
  UIStackedLayout();
  };

S_PROPERTY_INTERFACE(UIStackedLayout)

#endif // UISTACKEDLLAYOUT_H
